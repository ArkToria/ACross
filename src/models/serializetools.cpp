#include "serializetools.h"

using namespace across::config;

SerializeTools::SerializeTools()
{

}

std::optional<SIP008> SerializeTools::sip008Parser(const std::stringstream &data_stream)
{
    JsonTools json_tools;
    if (!json_tools.setDataStream(data_stream)) {
        return {};
    }

    auto root = json_tools.getRoot();
    SIP008 sip008;

    sip008.version = root["version"].asInt();
    sip008.username = root["username"].asString();
    sip008.user_uuid = root["user_uuid"].asString();
    sip008.bytes_used = root["bytes_used"].asLargestUInt();
    sip008.bytes_remaining = root["bytes_remaining"].asLargestUInt();

    for (auto& item: root["servers"]) {
        sip008.servers.emplace_back(SIP008::Server{
            .id = item["id"].asString(),
            .remarks = item["remarks"].asString(),
            .server = item["server"].asString(),
            .server_port = item["server_port"].asUInt(),
            .password = item["password"].asString(),
            .method = item["method"].asString(),
            .plugin = item["plugin"].asString(),
            .plugin_opts = item["plugin_opts"].asString(),
        });
    }

    return sip008;
}

std::optional<SIP008::Server> SerializeTools::sip002Decode(const QUrl &url)
{
    SIP008::Server server;

    QString user_info = QByteArray::fromBase64(url.userInfo().toUtf8());

    server.method = user_info.split(":").first().toStdString();
    server.password = user_info.split(":").last().toStdString();
    server.server = url.host().toStdString();
    server.server_port = url.port();
    server.remarks = url.fragment(QUrl::FullyDecoded).toStdString();

    if (server.method.empty() || server.password.empty() || server.server.empty()) {
        return {};
    }

    return server;
}

std::optional<URLMetaObject> SerializeTools::trojanDecode(const QUrl &url)
{
    // url scheme: trojan://<password>@<host>:<port>?sni=<server_name>&allowinsecure=<allow_insecure>&alpn=h2%0Ahttp/1.1#<name>
    do {
        OutboundObject outbound_object;

        TrojanObject::OutboundSettingObject outbound_setting;
        outbound_setting.address = url.host().toStdString();
        outbound_setting.port = url.port();
        outbound_setting.password = url.userInfo().toStdString();

        outbound_object.appendTrojanObject(outbound_setting);

        // query need to be verified
        if (url.hasQuery()) {
            QUrlQuery url_query(url);
            TransportObject::OutboundStreamObject outbound_stream;
            TransportObject::TLSObject tls;

            if (url_query.hasQueryItem("sni")) {
                tls.server_name = url_query.queryItemValue("sni").toStdString();
            }

            if (url_query.hasQueryItem("allowinsecure")) {
                tls.allow_insecure = url_query.queryItemValue("allowinsecure") == "true";
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
                    tls.appendAlpn(alpn.toStdString());
                } else {
                    for (auto &item : values) {
                        tls.appendAlpn(item.toStdString());
                    }
                }
            }

            outbound_stream.setTLSSetting(tls);
            outbound_object.setTransportStreamObject(outbound_stream);
        }

        return URLMetaObject {
            url.fragment(QUrl::FullyDecoded).toStdString(),
                    outbound_setting.address,
                    outbound_setting.port,
                    outbound_setting.password,
                    outbound_object
        };
    } while (false);

    return {};
}

std::optional<URLMetaObject> SerializeTools::vmessBase64Decode(const std::string &data)
{
    do {
        QString info = QString().fromStdString(data).split("://").takeLast();
        if (info.isEmpty()) {
            break;
        }

        QString base64_str = QByteArray::fromBase64(info.toUtf8());

        JsonTools json_tools;
        if (!json_tools.setData(base64_str.toStdString())) {
            break;
        }
        auto root = json_tools.getRoot();

        VMessObject::UserObject user_object;
        user_object.id = root["id"].asString();
        user_object.alter_id = root["aid"].asInt();

        VMessObject::OutboundSettingObject outbound_setting;
        outbound_setting.address = root["add"].asString();
        outbound_setting.port = root["port"].asInt();
        outbound_setting.appendUserObject(user_object);

        TransportObject::OutboundStreamObject outbound_stream;
        auto network = magic_enum::enum_cast<TransportObject::Network>(root["net"].asString());
        if (!network.has_value()) {
            break;
        } else {
            outbound_stream.network = network.value();
        }

        TransportObject::WebSocketObject websocket;

        switch (outbound_stream.network) {
        case TransportObject::Network::ws:
            websocket.setHost(root["host"].asString());
            websocket.path = root["path"].asString();
            outbound_stream.setWebsocketSetting(websocket);
        default:
            break;
        }

        if (!root["tls"].empty()) {
            outbound_stream.security = root["tls"].asString();
        }

        if (!root["sni"].empty()) {
            TransportObject::TLSObject tls;
            tls.server_name = root["sni"].asString();
            outbound_stream.setTLSSetting(tls);
        }

        OutboundObject outbound_object;
        outbound_object.appendVMessObject(outbound_setting);
        outbound_object.setTransportStreamObject(outbound_stream);

        return URLMetaObject {
            root["ps"].asString(),
                    outbound_setting.address,
                    outbound_setting.port,
                    user_object.id,
                    outbound_object
        };
    } while (false);

    return {};
}
