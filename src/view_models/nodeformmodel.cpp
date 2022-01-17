#include "nodeformmodel.h"

using namespace across;

NodeFormModel::NodeFormModel(QObject *parent)
    : QObject(parent), p_list(nullptr) {}

void NodeFormModel::accept(const QVariantMap &values) {
    NodeInfo node;
    bool is_new = true;

    if (values.contains("nodeID")) {
        qint64 node_id = values.value("nodeID").toLongLong();

        for (auto &item : p_list->items()) {
            if (node_id == item.id) {
                node = item;
                is_new = false;
                break;
            }
        }
    }

    if (!values.contains("type"))
        return;

    auto result = false;
    switch (values.value("type").toInt()) {
    case 0:
        if (values.contains("url"))
            result = SerializeTools::decodeOutboundFromURL(
                node, values.value("url").toString().toStdString());
        break;
    case 1:
        if (values.contains("manual"))
            result = manualSetting(node, values.value("manual").toMap());
        break;
    case 2:
        if (values.contains("config"))
            result = setRawOutbound(node, values.value("config").toMap());
        break;
    }

    if (!result)
        return;

    if (is_new) {
        p_list->appendNode(node);
    } else {
        p_list->updateNode(node);
    }
}

QString NodeFormModel::refreshPreview(const QVariantMap &values) {
    NodeInfo node;

    if (!values.contains("type"))
        return "";

    if (values.contains("nodeID")) {
        qint64 node_id = values.value("nodeID").toLongLong();

        for (auto &item : p_list->items()) {
            if (node_id == item.id) {
                node = item;
                break;
            }
        }
    }

    auto result = false;
    switch (values.value("type").toInt()) {
    case 0:
        if (values.contains("url"))
            result = SerializeTools::decodeOutboundFromURL(
                node, values.value("url").toString().toStdString());
        break;
    case 1:
        if (values.contains("manual"))
            result = manualSetting(node, values.value("manual").toMap());
        break;
    case 2:
        if (values.contains("config"))
            result = setRawOutbound(node, values.value("config").toMap());
        break;
    }

    if (!result)
        return "";
    else
        return node.raw;
}

NodeList *NodeFormModel::list() const { return p_list; }

void NodeFormModel::setList(NodeList *newList) {
    if (newList == nullptr)
        return;

    p_list = newList;

    emit listChanged();
}

bool NodeFormModel::manualSetting(NodeInfo &node, const QVariantMap &values) {
    if (values.contains("name") && values.contains("address") &&
        values.contains("port") && values.contains("password") &&
        values.contains("protocol")) {
        node.name = values.value("name").toString();
        node.address = values.value("address").toString();
        node.port = values.value("port").toUInt();
        node.password = values.value("password").toString();
        node.protocol =
            magic_enum::enum_value<EntryType>(values.value("protocol").toInt());

        if (node.port == 0)
            node.port = 443;

        switch (node.protocol) {
        case EntryType::vmess:
            if (values.contains("vmess") && !values.value("vmess").isNull()) {
                auto vmess = values.value("vmess").toMap();
                return setVMessOutboud(node, vmess);
            }
            break;
        case EntryType::shadowsocks:
            if (values.contains("shadowsocks") &&
                !values.value("shadowsocks").isNull()) {
                auto shadowsocks = values.value("shadowsocks").toMap();
                return setShadowsocksOutbound(node, shadowsocks);
            }
        case EntryType::trojan:
            if (values.contains("trojan") && !values.value("trojan").isNull()) {
                auto trojan = values.value("trojan").toMap();
                return setTrojanOutbound(node, trojan);
            }
        default:
            break;
        }
    }

    return false;
}

bool NodeFormModel::setTrojanOutbound(NodeInfo &node,
                                      const QVariantMap &values) {
    if (values.isEmpty())
        return false;

    auto outbound = m_config.add_outbounds();
    outbound->set_protocol("trojan");
    outbound->set_sendthrough("0.0.0.0");
    outbound->set_tag("PROXY");

    auto setting = outbound->mutable_settings()->mutable_trojan();
    auto server = setting->add_servers();
    server->set_address(node.address.toStdString());
    server->set_password(node.password.toStdString());
    server->set_port(node.port);

    auto stream = outbound->mutable_streamsettings();

    if (values.contains("network"))
        stream->set_network(values.value("network").toString().toStdString());
    else
        return false;

    if (values.contains("security"))
        stream->set_security(values.value("security").toString().toStdString());
    else
        return false;

    auto tls = stream->mutable_tlssettings();
    tls->set_allowinsecure(false);

    if (values.contains("serverName"))
        tls->set_servername(
            values.value("serverName").toString().toStdString());

    if (values.contains("alpn")) {
        auto alpn_str = values.value("alpn").toString();
        if (alpn_str == "h2+http/1.1") {
            tls->add_alpn("h2");
            tls->add_alpn("http/1.1");
        } else {
            tls->add_alpn(alpn_str.toStdString());
        }
    }

    node.raw = SerializeTools::MessageToJson(*outbound).c_str();
    if (node.raw.isEmpty())
        return false;

    URLMetaObject meta = {
        .name = node.name.toStdString(),
        .outbound = *outbound,
    };

    node.url = SerializeTools::trojanEncode(meta)->toEncoded();
    if (node.url.isEmpty())
        return false;

    return true;
}

bool NodeFormModel::setShadowsocksOutbound(NodeInfo &node,
                                           const QVariantMap &values) {
    if (values.isEmpty())
        return false;

    auto outbound = m_config.add_outbounds();
    outbound->set_protocol("shadowsocks");
    outbound->set_sendthrough("0.0.0.0");
    outbound->set_tag("PROXY");

    auto setting = outbound->mutable_settings()->mutable_shadowsocks();
    auto server = setting->add_servers();
    server->set_address(node.address.toStdString());
    server->set_port(node.port);
    server->set_password(node.password.toStdString());

    if (values.contains("security"))
        server->set_method(values.value("security").toString().toStdString());
    else
        return false;

    if (values.contains("ivCheck"))
        server->set_ivcheck(values.value("ivCheck").toBool());
    else
        return false;

    node.raw = SerializeTools::MessageToJson(*outbound).c_str();
    if (node.raw.isEmpty())
        return false;

    URLMetaObject meta = {
        .name = node.name.toStdString(),
        .outbound = *outbound,
    };

    node.url = SerializeTools::sip002Encode(meta)->toEncoded();
    if (node.url.isEmpty())
        return false;

    return true;
}

bool NodeFormModel::setVMessOutboud(NodeInfo &node, const QVariantMap &values) {
    if (values.isEmpty())
        return false;

    auto outbound = m_config.add_outbounds();
    outbound->set_protocol("vmess");
    outbound->set_sendthrough("0.0.0.0");
    outbound->set_tag("PROXY");

    auto setting = outbound->mutable_settings()->mutable_vmess();
    auto server = setting->add_vnext();
    server->set_address(node.address.toStdString());
    server->set_port(node.port);

    auto user = server->add_users();
    user->set_id(node.password.toStdString());

    if (values.contains("alterID"))
        user->set_alterid(values.value("alterID").toInt());

    if (values.contains("security"))
        user->set_security(values.value("security").toString().toStdString());

    auto stream = outbound->mutable_streamsettings();
    if (values.contains("network")) {
        if (values.value("network").toString() == "h2")
            stream->set_network("http");
        else
            stream->set_network(
                values.value("network").toString().toStdString());
    }

    if (values.contains("enableTLS") && values.value("enableTLS").toBool()) {
        stream->set_security("tls");

        auto tls = stream->mutable_tlssettings();
        if (values.contains("sni") &&
            !values.value("sni").toString().isEmpty()) {
            tls->set_servername(values.value("sni").toString().toStdString());
        } else {
            tls->set_servername(server->address());
        }
    } else {
        stream->set_security("none");
    }

    do {
        if (stream->network() == "tcp") {
            // TODO: TCP support
            break;
        }

        if (stream->network() == "http") {
            auto http2 = stream->mutable_httpsettings();

            if (values.contains("host")) {
                for (const auto &host :
                     values.value("host").toString().trimmed().split(",")) {
                    http2->add_host(host.toStdString());
                }
            }

            if (values.contains("path")) {
                http2->set_path(values.value("path").toString().toStdString());
            }
            break;
        }

        if (stream->network() == "ws") {
            auto websocket = stream->mutable_wssettings();

            if (values.contains("host")) {
                auto headers = websocket->mutable_headers();
                headers->insert(
                    {"Host", values.value("host").toString().toStdString()});
            }

            if (values.contains("path")) {
                websocket->set_path(
                    values.value("path").toString().toStdString());
            }
            break;
        }

        if (stream->network() == "grpc") {
            auto grpc = stream->mutable_grpcsettings();

            if (values.contains("path")) {
                grpc->set_servicename(
                    values.value("path").toString().toStdString());
            }
            break;
        }

        if (stream->network() == "quic") {
            auto quic = stream->mutable_quicsettings();

            if (values.contains("type")) {
                auto header = quic->mutable_header();
                header->set_type(values.value("type").toString().toStdString());
            }

            if (values.contains("host")) {
                quic->set_security(
                    values.value("host").toString().toStdString());
            }

            if (values.contains("path")) {
                quic->set_key(values.value("path").toString().toStdString());
            }
            break;
        }
    } while (false);

    node.raw = SerializeTools::MessageToJson(*outbound).c_str();
    if (node.raw.isEmpty())
        return false;

    URLMetaObject meta = {
        .name = node.name.toStdString(),
        .outbound = *outbound,
    };

    node.url = SerializeTools::vmessBase64Encode(meta)->toEncoded();
    if (node.url.isEmpty())
        return false;

    return true;
}

bool NodeFormModel::setRawOutbound(NodeInfo &node, const QVariantMap &values) {
    if (values.isEmpty() || !values.contains("filePath"))
        return false;

    auto config_path = values.value("filePath").toString();
    if (config_path.isEmpty())
        return false;

    QString content;
    QString config_name;
    QFile config(QUrl(config_path).toLocalFile());

    if (!config.exists())
        return false;

    if (config.open(QIODevice::ReadOnly | QIODevice::Text)) {
        content = config.readAll();
        config.close();
    }

    if (values.contains("name") && !values.value("name").toString().isEmpty()) {
        config_name = values.value("name").toString();
    } else {
        config_name = QFileInfo(config).fileName();
    }

    Json root;

    try {
        root = Json::parse(content.toStdString());
    } catch (Json::exception &e) {
        qDebug() << e.what();
        return false;
    }

    if (!root.contains("outbounds"))
        return false;

    auto outbounds = root["outbounds"];
    for (auto &outbound : outbounds) {
        if (!outbound.contains("protocol"))
            continue;

        if (auto protocol = outbound["protocol"]; protocol == "vmess") {
            if (!outbound.contains("settings") ||
                !outbound["settings"].contains("vnext") ||
                !outbound["settings"]["vnext"].is_array())
                continue;

            auto server = outbound["settings"]["vnext"][0];
            if (server.is_null())
                continue;

            if (!server.contains("users") || !server["users"].is_array())
                continue;

            auto user = server["users"][0];
            if (user.is_null())
                continue;

            node.name = config_name;
            node.protocol = EntryType::vmess;
            node.address = server["address"].get<std::string>().c_str();
            node.port = server["port"].get<uint>();
            node.password = user["id"].get<std::string>().c_str();
            node.raw = outbound.dump().c_str();
            node.url = "custom configuration encoding to url is not supported";
            break;
        }

        if (auto protocol = outbound["protocol"];
            protocol == "trojan" || protocol == "shadowsocks") {
            if (!outbound.contains("settings") ||
                !outbound["settings"].contains("servers") ||
                !outbound["settings"]["servers"].is_array())
                continue;

            auto server = outbound["settings"]["servers"][0];
            if (server.is_null() || !server.contains("address") ||
                !server.contains("password") || !server.contains("port"))
                continue;

            if (protocol == "trojan")
                node.protocol = EntryType::trojan;
            else
                node.protocol = EntryType::shadowsocks;

            node.name = config_name;
            node.address = server["address"].get<std::string>().c_str();
            node.port = server["port"].get<uint>();
            node.password = server["password"].get<std::string>().c_str();
            node.raw = outbound.dump().c_str();
            node.url = QString(
                tr("custom configuration encoding to url is not supported"));
            break;
        }
    }

    if (node.address.isEmpty() || node.password.isEmpty())
        return false;

    return true;
}
