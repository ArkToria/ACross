#include "serializetools.h"

using namespace across;

std::optional<std::vector<URLMetaObject>>
SerializeTools::sip008Parser(const std::string &url_str) {
    Json root;
    try {
        root = Json::parse(url_str);
    } catch (Json::exception &e) {
        qDebug() << e.what();
        return {};
    }

    if (root.is_null() || root.empty() || !root.contains("version")) {
        return {};
    }

    if (!root.contains("servers"))
        return {};

    std::vector<URLMetaObject> meta_objects;

    for (auto &item : root["servers"]) {
        URLMetaObject meta;
        auto outbound = &meta.outbound;
        outbound->set_protocol("shadowsocks");
        outbound->set_sendthrough("0.0.0.0");

        auto shadowsocks = outbound->mutable_settings()->mutable_shadowsocks();
        auto server = shadowsocks->add_servers();

        if (item.contains("remarks"))
            meta.name = item["remarks"];

        if (item.contains("server") && item.contains("server_port") &&
            item.contains("password") && item.contains("method")) {
            server->set_address(item["server"]);
            server->set_port(item["server_port"]);
            server->set_password(item["password"]);
            server->set_method(item["method"]);
        } else {
            continue;
        }

        meta_objects.emplace_back(meta);
    }

    return meta_objects;
}

std::optional<URLMetaObject>
SerializeTools::sip002Decode(const std::string &url_str) {
    // url scheme:
    // ss://<websafe-base64-encode-utf8(method:password)>@hostname:port/?plugin"#"tag

    QUrl url(url_str.c_str());

    URLMetaObject meta;
    meta.name = url.fragment(QUrl::FullyDecoded).toStdString();

    auto outbound = &meta.outbound;
    outbound->set_protocol("shadowsocks");
    outbound->set_sendthrough("0.0.0.0");

    auto shadowsocks = outbound->mutable_settings()->mutable_shadowsocks();
    auto server = shadowsocks->add_servers();

    QString user_info = QByteArray::fromBase64(url.userInfo().toUtf8());
    if (user_info.isEmpty())
        return {};

    server->set_address(url.host().toStdString());
    server->set_port(url.port());
    server->set_password(user_info.split(":").last().toStdString());
    server->set_method(user_info.split(":").first().toStdString());

    return meta;
}

std::optional<QUrl> SerializeTools::sip002Encode(const URLMetaObject &meta) {
    // url scheme:
    // ss://<websafe-base64-encode-utf8(method:password)>@hostname:port/?plugin"#"tag

    QUrl url;
    auto outbound = &meta.outbound;
    auto setting = outbound->settings().shadowsocks();
    const auto& server = setting.servers(0);

    QString user_info =
        QString("%1:%2")
            .arg(server.method().c_str(), server.password().c_str())
            .toUtf8()
            .toBase64(QByteArray::Base64UrlEncoding);

    url.setScheme("ss");
    url.setHost(server.address().c_str());
    url.setPort(server.port());
    url.setUserInfo(user_info);
    url.setFragment(meta.name.c_str());

    return url;
}

std::optional<URLMetaObject>
SerializeTools::trojanDecode(const std::string &url_str) {
    // url scheme:
    // trojan://<password>@<host>:<port>?sni=<server_name>&allowinsecure=<allow_insecure>&alpn=h2%0Ahttp/1.1#<name>

    QUrl url(url_str.c_str());

    URLMetaObject meta;
    meta.name = url.fragment(QUrl::FullyDecoded).toStdString();

    auto outbound = &meta.outbound;
    outbound->set_protocol("trojan");
    outbound->set_sendthrough("0.0.0.0");

    auto trojan = outbound->mutable_settings()->mutable_trojan();
    auto server = trojan->add_servers();

    if (url.host().isEmpty() || url.userInfo().isEmpty() ||
        !url.scheme().contains("trojan"))
        return {};

    server->set_address(url.host().toStdString());
    server->set_port(url.port());
    server->set_password(url.userInfo().toStdString());

    // query need to be verified
    if (url.hasQuery()) {
        QUrlQuery url_query(url);

        auto stream = outbound->mutable_streamsettings();
        auto tls = stream->mutable_tlssettings();

        if (url_query.hasQueryItem("sni")) {
            tls->set_servername(url_query.queryItemValue("sni").toStdString());
        }

        if (url_query.hasQueryItem("allowinsecure")) {
            if (url_query.queryItemValue("allowinsecure")
                    .toLower()
                    .contains("true"))
                tls->set_allowinsecure(true);
        }

        if (url_query.hasQueryItem("alpn")) {
            auto alpn = url_query.queryItemValue("alpn", QUrl::FullyDecoded);

            QStringList values;

            if (alpn.contains(",")) {
                values = alpn.split(",");
            } else if (alpn.contains("\n")) {
                values = alpn.split("\n");
            }

            if (values.empty()) {
                tls->add_alpn(alpn.toStdString());
            } else {
                for (auto &item : values) {
                    tls->add_alpn(item.toStdString());
                }
            }
        }
    }

    return meta;
}

std::optional<QUrl> SerializeTools::trojanEncode(const URLMetaObject &meta) {
    // url scheme:
    // trojan://<password>@<host>:<port>?sni=<server_name>&allowinsecure=<allow_insecure>&alpn=h2%0Ahttp/1.1#<name>

    QUrl url;
    QUrlQuery query;

    auto outbound = &meta.outbound;
    auto setting = outbound->settings().trojan();
    const auto& server = setting.servers(0);

    url.setScheme(outbound->protocol().c_str());
    url.setHost(server.address().c_str());
    url.setPort(server.port());
    url.setUserInfo(server.password().c_str());
    url.setFragment(meta.name.c_str());

    if (outbound->has_streamsettings()) {
        const auto& stream = outbound->streamsettings();

        if (stream.has_tlssettings()) {
            const auto& tls = stream.tlssettings();
            query.addQueryItem("sni", tls.servername().c_str());

            if (tls.allowinsecure()) {
                query.addQueryItem("allowinsecure", "true");
            } else {
                query.addQueryItem("allowinsecure", "false");
            }

            QString alpn_string;
            auto alpn_size = tls.alpn_size();
            for (auto &alpn : tls.alpn()) {
                if (!alpn.empty())
                    alpn_string.append(alpn.c_str());

                if (alpn_size > 1) {
                    alpn_string.append("%0A");
                    alpn_size--;
                }
            }
            if (!alpn_string.isEmpty())
                query.addQueryItem("alpn", alpn_string);
        }
    }

    url.setQuery(query);
    return url;
}

std::optional<URLMetaObject>
SerializeTools::vmessBase64Decode(const std::string &url_str) {
    // url scheme:
    // vmess://<base64EncodeJson>
    // {
    //     "v": "2",
    //     "ps": "Names",
    //     "add": "111.111.111.111",
    //     "port": "32000",
    //     "id": "1386f85e-657b-4d6e-9d56-78badb75e1fd",
    //     "aid": "100",
    //     "scy": "zero",
    //     "net": "tcp",
    //     "type": "none",
    //     "host": "www.bbb.com",
    //     "path": "/",
    //     "tls": "tls",
    //     "sni": "www.ccc.com"
    //  }

    QString info = QString::fromStdString(url_str).split("://").takeLast();
    if (info.isEmpty())
        return {};

    QString base64_str = QByteArray::fromBase64(info.toUtf8());

    Json root;
    try {
        root = Json::parse(base64_str.toStdString());
    } catch (Json::exception &e) {
        qDebug() << e.what();
        return {};
    }

    if (root.empty())
        return {};

    URLMetaObject meta;
    auto outbound = &meta.outbound;
    outbound->set_protocol("vmess");
    outbound->set_sendthrough("0.0.0.0");

    auto vmess = outbound->mutable_settings()->mutable_vmess();
    auto server = vmess->add_vnext();
    auto user = server->add_users();
    auto stream = outbound->mutable_streamsettings();

    if (root.contains("ps"))
        meta.name = root["ps"];

    if (root.contains("add") && root.contains("port")) {
        server->set_address(root["add"]);

        if (root["port"].is_number()) {
            server->set_port(root["port"].get<std::uint32_t>());
        } else if (root["port"].is_string()) {
            server->set_port(std::stoul(root["port"].get<std::string>()));
        } else {
            return {};
        }
    } else {
        return {};
    }

    if (root.contains("id"))
        user->set_id(root["id"]);
    else
        return {};

    user->set_alterid(0);
    if (root.contains("aid")) {
        if (root["aid"].is_number()) {
            user->set_alterid(root["aid"].get<int>());
        } else if (root["aid"].is_string()) {
            user->set_alterid(std::stoi(root["aid"].get<std::string>()));
        }
    }

    user->set_security("auto");
    if (root.contains("scy")) {
        user->set_security(root["scy"]);
    }

    if (root.contains("net")) {
        if (root["net"].get<std::string>() == "h2") {
            stream->set_network("http");
        } else {
            stream->set_network(root["net"].get<std::string>());
        }

        do {
            if (stream->network() == "tcp") {
                // TODO: fake header support
                //                auto tcp = stream->mutable_tcpsettings();

                //                if (root.contains("type")) {
                //                    auto header = tcp->mutable_header();
                //                    header->set_type(root["type"].get<std::string>());
                //                }
                break;
            }

            if (stream->network() == "http") {
                auto http2 = stream->mutable_httpsettings();

                if (root.contains("host")) {
                    auto content =
                        QString::fromStdString(root["host"].get<std::string>())
                            .trimmed()
                            .split(",");
                    for (const auto &host : content) {
                        if (!host.isEmpty())
                            http2->add_host(host.trimmed().toStdString());
                    }
                }

                if (root.contains("path")) {
                    http2->set_path(root["path"].get<std::string>());
                }
            }

            if (stream->network() == "ws") {
                auto websocket = stream->mutable_wssettings();

                if (root.contains("host")) {
                    auto headers = websocket->mutable_headers();
                    headers->insert({"Host", root["host"]});
                }

                if (root.contains("path")) {
                    websocket->set_path(root["path"]);
                }
                break;
            }

            if (stream->network() == "grpc") {
                auto grpc = stream->mutable_grpcsettings();

                if (root.contains("path")) {
                    grpc->set_servicename(root["path"]);
                }
                break;
            }

            if (stream->network() == "quic") {
                auto quic = stream->mutable_quicsettings();

                if (root.contains("type")) {
                    auto header = quic->mutable_header();
                    header->set_type(root["type"]);
                }

                if (root.contains("host")) {
                    quic->set_security(root["host"]);
                }

                if (root.contains("path")) {
                    quic->set_key(root["path"]);
                }
                break;
            }
        } while (false);
    }

    if (root.contains("tls")) {
        stream->set_security(root["tls"].get<std::string>());
    }

    if (root.contains("sni")) {
        auto serverName = root["sni"].get<std::string>();
        auto tls = stream->mutable_tlssettings();
        tls->set_servername(root["sni"].get<std::string>());
    }

    return meta;
}

std::optional<QUrl>
SerializeTools::vmessBase64Encode(const URLMetaObject &meta) {
    // url scheme:
    // vmess://<base64EncodeJson>
    // {
    //     "v": "2",
    //     "ps": "Names",
    //     "add": "111.111.111.111",
    //     "port": "32000",
    //     "id": "1386f85e-657b-4d6e-9d56-78badb75e1fd",
    //     "aid": "100",
    //     "scy": "zero",
    //     "net": "tcp",
    //     "type": "none",
    //     "host": "www.bbb.com",
    //     "path": "/",
    //     "tls": "tls",
    //     "sni": "www.ccc.com"
    //  }

    QUrl url;
    auto outbound = &meta.outbound;
    auto settings = outbound->settings().vmess();
    const auto& server = settings.vnext(0);
    const auto& user = server.users(0);
    const auto& stream = outbound->streamsettings();

    url.setScheme(outbound->protocol().c_str());

    Json root;
    root["v"] = "2";
    root["ps"] = meta.name;
    root["add"] = server.address();
    root["port"] = server.port();
    root["id"] = user.id();
    root["aid"] = user.alterid();

    root["sni"] = server.address();
    if (stream.has_tlssettings()) {
        root["sni"] = stream.tlssettings().servername();
    }

    if (user.security().empty())
        root["scy"] = "auto";
    else
        root["scy"] = user.security();

    if (stream.IsInitialized()) {
        if (stream.network() == "h2" || stream.network() == "http")
            root["net"] = "h2";
        else
            root["net"] = stream.network();

        do {
            if (stream.network() == "http" && stream.has_httpsettings()) {
                const auto &http2 = stream.httpsettings();
                std::string hosts;

                for (const auto &host : stream.httpsettings().host()) {
                    if (!host.empty()) {
                        hosts.append(host);
                        hosts.append(",");
                    }
                }

                hosts = hosts.substr(0, hosts.length() - 1);
                root["host"] = hosts;
                root["path"] = http2.path();
                break;
            }

            if (stream.network() == "ws" && stream.has_wssettings()) {
                const auto& websocket = stream.wssettings();
                root["host"] = websocket.headers().at("Host");
                root["path"] = websocket.path();
                break;
            }

            if (stream.network() == "grpc" && stream.has_grpcsettings()) {
                const auto& grpc = stream.grpcsettings();
                root["path"] = grpc.servicename();
                break;
            }

            if (stream.network() == "quic" && stream.has_quicsettings()) {
                const auto& quic = stream.quicsettings();
                root["type"] = quic.header().type();
                root["host"] = quic.security();
                root["path"] = quic.key();
                break;
            }
        } while (false);

        root["tls"] = stream.security();
    }

    // TODO: support fake stream type
    root["type"] = "none";

    url.setUserInfo(QString::fromStdString(root.dump()).toUtf8().toBase64());

    return url;
}

bool SerializeTools::decodeOutboundFromURL(NodeInfo &node,
                                           const std::string &url_str) {
    node.url = url_str.c_str();

    QUrl url(url_str.c_str());
    if (url.scheme().isEmpty())
        return false;

    if (url.scheme().contains("vmess"))
        return SerializeTools::setVMessOutboundFromBase64(node, url_str);

    if (url.scheme().contains("trojan"))
        return SerializeTools::setTrojanOutboundFromURL(node, url_str);

    if (url.scheme() == "ss") {
        return SerializeTools::setShadowsocksOutboundFromURL(node, url_str);
    }

    return false;
}

bool SerializeTools::setShadowsocksOutboundFromURL(NodeInfo &node,
                                                   const std::string &url_str) {
    auto meta = SerializeTools::sip002Decode(url_str);
    if (!meta.has_value())
        return false;

    auto outbound = meta->outbound;
    auto shadowsocks = outbound.settings().shadowsocks();
    const auto& server = shadowsocks.servers(0);

    node.protocol = EntryType::shadowsocks;
    node.name = meta->name.c_str();
    node.address = server.address().c_str();
    node.port = server.port();
    node.password = server.password().c_str();
    node.raw = MessageToJson(outbound).c_str();

    return true;
}

bool SerializeTools::setVMessOutboundFromBase64(NodeInfo &node,
                                                const std::string &data) {
    auto meta = SerializeTools::vmessBase64Decode(data);
    if (!meta.has_value())
        return false;

    auto outbound = meta->outbound;
    auto vmess = outbound.settings().vmess();
    const auto& server = vmess.vnext(0);
    const auto& user = server.users(0);

    node.protocol = EntryType::vmess;
    node.name = meta->name.c_str();
    node.address = server.address().c_str();
    node.port = server.port();
    node.password = user.id().c_str();
    node.raw = MessageToJson(outbound).c_str();

    return true;
}

bool SerializeTools::setTrojanOutboundFromURL(NodeInfo &node,
                                              const std::string &url_str) {
    auto meta = SerializeTools::trojanDecode(url_str);
    if (!meta.has_value())
        return false;

    auto outbound = meta->outbound;
    auto trojan = outbound.settings().trojan();
    const auto& server = trojan.servers(0);

    node.protocol = EntryType::trojan;
    node.name = meta->name.c_str();
    node.address = server.address().c_str();
    node.port = server.port();
    node.password = server.password().c_str();
    node.raw = MessageToJson(outbound).c_str();

    return true;
}

google::protobuf::util::JsonPrintOptions SerializeTools::defaultPrintOptions() {
    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = true;
    options.always_print_primitive_fields = false;
    options.preserve_proto_field_names = true;
    options.always_print_enums_as_ints = true;

    return options;
}

std::string
SerializeTools::MessageToJson(const google::protobuf::Message &message) {
    std::string json_str;
    google::protobuf::util::MessageToJsonString(message, &json_str,
                                                defaultPrintOptions());
    return json_str;
}

v2ray::config::V2rayConfig
SerializeTools::JsonToConfig(const std::string &json_str) {
    v2ray::config::V2rayConfig config;
    google::protobuf::util::JsonStringToMessage(json_str, &config);
    return config;
}

across::config::Config
SerializeTools::JsonToACrossConfig(const std::string &json_str) {
    across::config::Config config;
    google::protobuf::util::JsonStringToMessage(json_str, &config);
    return config;
}

v2ray::config::OutboundObject
SerializeTools::JsonToOutbound(const std::string &json_str) {
    v2ray::config::OutboundObject outbound;
    google::protobuf::util::JsonStringToMessage(json_str, &outbound);
    return outbound;
}

std::string
SerializeTools::ConfigToJson(v2ray::config::V2rayConfig &origin_config,
                             const QString &outbound_str) {
    Json root;
    try {
        root = Json::parse(MessageToJson(origin_config));
    } catch (Json::exception &e) {
        qDebug() << e.what();
        return "";
    }

    if (!root.contains("inbounds") || !root["inbounds"].is_array())
        return "";

    auto fix_format = [&root](const std::vector<std::string> &keys) {
        for (auto &key : keys) {
            for (size_t i = 0; i < root[key].size(); ++i) {
                auto protocol = QString::fromStdString(
                                    root[key][i]["protocol"].get<std::string>())
                                    .replace("-", "_")
                                    .toStdString();

                auto setting = root[key][i]["settings"][protocol];
                root[key][i]["settings"] = setting;
            }
        }
    };

    if (outbound_str.isEmpty()) {
        fix_format({"inbounds", "outbounds"});
    } else {
        fix_format({"inbounds"});

        Json outbound;
        try {
            outbound = Json::parse(outbound_str.toStdString());
        } catch (Json::exception &e) {
            qDebug() << e.what();
            fix_format({"outbounds"});
        }

        if (!outbound.is_null())
            root["outbounds"][0] = outbound;
    }

    return root.dump();
}
