#ifndef SERIALIZETOOLS_H
#define SERIALIZETOOLS_H

#include "dbtools.h"

#include "google/protobuf/util/json_util.h"
#include "nlohmann/json.hpp"

#include <QDateTime>
#include <QString>
#include <QUrl>
#include <QUrlQuery>
#include <optional>
#include <string>

#include "across.grpc.pb.h"
#include "v2ray_config.grpc.pb.h"

namespace across {
using Json = nlohmann::json;

struct URLMetaObject
{
  std::string name = "";
  v2ray::config::OutboundObject outbound;
};

class SerializeTools
{
public:
  // Shadowsocks
  static std::optional<std::vector<URLMetaObject>> sip008Parser(
    const std::string& url_str);

  static std::optional<URLMetaObject> sip002Decode(const std::string& url_str);
  static std::optional<QUrl> sip002Encode(const URLMetaObject& meta);

  // Trojan
  static std::optional<URLMetaObject> trojanDecode(const std::string& url_str);
  static std::optional<QUrl> trojanEncode(const URLMetaObject& outbound);

  // VMESS
  static std::optional<URLMetaObject> vmessBase64Decode(
    const std::string& url_str);
  static std::optional<QUrl> vmessBase64Encode(const URLMetaObject& meta);

  // Decode From URL
  static bool decodeOutboundFromURL(NodeInfo& node, const std::string& url_str);
  static bool setShadowsocksOutboundFromURL(NodeInfo& node,
                                            const std::string& url_str);
  static bool setVMessOutboundFromBase64(NodeInfo& node,
                                         const std::string& url_str);
  static bool setTrojanOutboundFromURL(NodeInfo& node,
                                       const std::string& url_str);

  // Outbound Convert
  static google::protobuf::util::JsonPrintOptions defaultPrintOptions();
  static std::string MessageToJson(const google::protobuf::Message& message);
  static v2ray::config::V2rayConfig JsonToConfig(const std::string& json_str);
  static across::config::Config JsonToACrossConfig(const std::string& json_str);
  static v2ray::config::OutboundObject JsonToOutbound(
    const std::string& json_str);
  static std::string ConfigToJson(v2ray::config::V2rayConfig& origin_config,
                                  const QString& outbounds_str = "");
};
}

#endif // SERIALIZETOOLS_H
