#ifndef JSONTOOLS_H
#define JSONTOOLS_H

#include "cryptotools.h"
#include "magic_enum.hpp"
#include "json/json.h"
#include <QString>
#include <QUrl>
#include <algorithm>
#include <list>

namespace across {
namespace config {
static inline bool
checkJsonArray(Json::Value& array)
{
  if (!array.isNull() && !array.empty() && array.isArray())
    return true;
  return false;
}

struct LogObject
{
  enum LogLevel
  {
    debug,
    info,
    warning,
    error,
    none
  };

  std::string access_log_path = "none"; // [v4.20+] support close log
  std::string error_log_path = "";

  void setLogLevel(LogLevel level);
  void setObject(Json::Value& root);

private:
  std::string log_level = "warning";
};

struct APIObject
{
  const QString tag = "ACROSS_API";
  Json::Value services = Json::arrayValue;

  APIObject();
  void setObject(Json::Value& root);
};

struct TransportObject
{
  enum Network : int
  {
    ws,
    quic,
    tcp,
    kcp,
    http
  };

  struct CertificateObject
  {
    std::string usage = "encipherment";
    std::string certificate_file = "";
    std::string key_file = "";

    void appendCertificate(const std::string& value);
    void appendKey(const std::string& value);
    Json::Value toObject();

  private:
    Json::Value certificate_objects = Json::arrayValue;
    Json::Value key_objects = Json::arrayValue;
  };

  struct TLSObject
  {
    std::string server_name = "";
    bool allow_insecure = false;
    bool disable_system_root = false;

    void appendAlpn(const std::string& value);
    void appendCertificateObject(Json::Value certificate);
    void appendCertificateObject(CertificateObject certificate);
    Json::Value toObject();

  private:
    Json::Value alpn_object = Json::arrayValue;
    Json::Value certificate_objects = Json::arrayValue;
  };

  struct WebSocketObject
  {
    bool accept_proxy_protocol = false;
    std::string path = "";
    int max_early_data = 0;
    bool use_browser_forwarding = false;
    std::string early_data_header_name = "";

    void setHost(const std::string& value);
    Json::Value toObject();

  private:
    std::map<std::string, std::string> headers;
  };

  struct OutboundStreamObject
  {
    Network network = Network::tcp;
    std::string security = "tls";
    Json::Value tls_setting;
    Json::Value websocket_setting;

    void setTLSSetting(Json::Value tls_object);
    void setTLSSetting(TLSObject tls_object);
    void setWebsocketSetting(Json::Value websocket_object);
    void setWebsocketSetting(WebSocketObject websocket_object);
    Json::Value toObject();
  };
};

struct SIP008
{
  struct Server
  {
    std::string id = "";
    std::string remarks = "";
    std::string server = "";
    uint server_port = 0;
    std::string password = "";
    std::string method = "";
    std::string plugin = "";
    std::string plugin_opts = "";

    // Other custom fields
    std::string email = "";
  };

  uint version = 1;
  std::vector<Server> servers;
  unsigned long long bytes_used = 0;
  unsigned long long bytes_remaining = 0;

  // Other custom fields
  std::string username = "";
  std::string user_uuid = "";

  Json::Value toObject(int index = 0);
};

struct DokodemoDoorObject
{
  struct InboundSettingObject
  {
    std::string address = "127.0.0.1";
    uint port = 15492;
    std::string network = "tcp";
    int timeout = 300;
    bool follow_redirect = false;
    int user_level = 0;

    Json::Value toObject();
  };
};

struct HttpObject
{
  struct AccountObject
  {
    std::string username = "";
    std::string password = "";

    Json::Value toObject();
  };

  struct InboundSettingObject
  {
    uint timeout = 0;
    bool allow_transparent = false;
    int user_level = 0;

    void appendAccountObject(AccountObject account);
    void appendAccountObject(Json::Value account);
    Json::Value toObject();

  private:
    Json::Value accounts = Json::arrayValue;
  };

  struct OutboundSettingObject
  {
    std::string address;
    uint port;

    void appendUserObject(AccountObject user);
    void appendUserObject(Json::Value user);
    Json::Value toObject();

  private:
    Json::Value users = Json::arrayValue;
  };
};

struct SocksObject
{
  struct AccountObject
  {
    std::string username = "";
    std::string password = "";

    Json::Value toObject();
  };

  struct InboundSettingObject
  {
    std::string auth = "noauth";
    bool udp = false;
    std::string ip = ""; // [v4.34.0+] default empty
    int user_level = 0;

    void appendAccountObject(AccountObject account);
    void appendAccountObject(Json::Value account);
    Json::Value toObject();

  private:
    Json::Value accounts = Json::arrayValue;
  };

  struct OutboundSettingObject
  {
    std::string address = "";
    uint port = 0;

    void appendUserObject(AccountObject user);
    void appendUserObject(Json::Value user);
    Json::Value toObject();

  private:
    Json::Value users = Json::arrayValue;
  };
};

struct ShadowsocksObject
{
  static bool checkMethod(const std::string& method);

  struct InboundSettingObject
  {
    std::string email;
    std::string password;
    int level = 0;
    bool iv_check = false;

    bool setNetwork(const std::string& type);
    bool setMethod(const std::string& method);
    Json::Value toObject();

  private:
    std::string network = "tcp";
    std::string method;
  };

  struct OutboundSettingObject
  {
    std::string email;
    std::string address;
    uint port = 0;
    std::string password;
    uint level = 0;
    bool iv_check = false;

    bool setMethod(const std::string& method);
    bool fromSIP008Server(const SIP008::Server& server);
    Json::Value toObject();

  private:
    std::string method;
  };
};

struct VMessObject
{
  struct ClientObject
  {
    std::string id = "";
    int level = 0;
    int alter_id = 0;
    std::string email;

    Json::Value toObject();
  };

  struct DefaultObject
  {
    int level = 0;
    int alter_id = 0;

    Json::Value toObject();
  };

  struct DetourObject
  {
    std::string tag = "";

    Json::Value toObject();
  };

  struct InboundSettingObject
  {
    Json::Value detour_object;
    Json::Value default_object;
    bool disable_insecure_encryption = false;

    void appendClientObject(Json::Value client);
    void appendClientObject(ClientObject client);
    Json::Value toObject();

  private:
    Json::Value client_objects = Json::arrayValue;
  };

  struct UserObject
  {
    std::string id = "";
    int alter_id = 0; // [v4.28.1+] default enable VMessAEAD
    int level = 0;

    bool setMethod(const std::string& method);
    Json::Value toObject();

  private:
    std::string method = "auto";
  };

  struct OutboundSettingObject
  {
    std::string address = "";
    uint port = 0;

    void appendUserObject(Json::Value user);
    void appendUserObject(UserObject user);
    Json::Value toObject();

  private:
    Json::Value user_objects = Json::arrayValue;
  };
};

struct TrojanObject
{
  struct ClientObject
  {
    std::string password = "";
    std::string email = "";
    int level = 0;

    Json::Value toObject();
  };

  struct FallbackObject
  {
    std::string alpn = "";
    std::string path = "";
    int dest = 80;
    int xver = 0;

    Json::Value toObject();
  };

  struct InboundSettingObject
  {
    void appendClientObject(Json::Value client);
    void appendClientObject(ClientObject client);
    void appendFallbackObject(Json::Value fallback);
    void appendFallbackObject(FallbackObject fallback);
    Json::Value toObject();

  private:
    Json::Value client_objects = Json::arrayValue;
    Json::Value fallback_objects = Json::arrayValue;
  };

  struct OutboundSettingObject
  {
    std::string address = "";
    uint port = 0;
    std::string password = "";
    std::string email = "";
    int level = 0;

    Json::Value toObject();
  } outbound_setting_object;

  TransportObject::CertificateObject certificate_object;
  TransportObject::TLSObject tls_object;
  TransportObject::OutboundStreamObject outbound_stream_object;
};

struct SniffingObject
{
  bool enabled = true;
  bool metadata_only = false;

  bool appendDestOverride(const std::string& type);
  Json::Value toObject();

private:
  Json::Value dest_override = Json::arrayValue;
};

struct AllocateObject
{
  bool setStrategy(const std::string& type);
  bool setRefresh(uint min);
  bool setConcurrency(uint num, std::string port);
  Json::Value toObject();

private:
  uint refresh = 5;
  std::string strategy = "random";
  uint concurrency = 0;
};

struct RuleObject
{
  // domain_matche: "linear" | "mph"
  std::string domain_matche = "mph";
  std::string type = "field";
  Json::Value domains = Json::arrayValue;
  Json::Value ip = Json::arrayValue;
  std::string port = "0";
  std::string source_port = "0";
  // network: "tcp" | "udp" | "tcp,udp"
  std::string network = "tcp";
  Json::Value source = Json::arrayValue;
  Json::Value user = Json::arrayValue;
  Json::Value inbound_tag = Json::arrayValue;
  // protocol: "http" | "tls" | "bittorrent"
  Json::Value protocol = Json::arrayValue;
  std::string attrs = "";
  std::string outbound_tag = "";
  std::string balancer_tag = "";

  Json::Value toObject();
};

struct StrategyObject
{
  // "random" | "leastPing"
  std::string type = "random";

  Json::Value toObject();
};

struct BalancerObject
{
  std::string tag = "balancer";
  Json::Value selector = Json::arrayValue;

  void setStrategyObject(StrategyObject strategy_object);
  void setStrategyObject(Json::Value strategy_object);
  Json::Value toObject();

private:
  Json::Value strategy;
};

struct RoutingObject
{
  // "AsIs" | "IPIfNonMatch" | "IPOnDemand"
  std::string domain_strategy = "AsIs";

  // "linear" | "mph"
  std::string domain_matche = "mph";

  void appendRuleObject(RuleObject rule_object);
  void appendRuleObject(Json::Value rule_object);
  void appendBalancerObject(BalancerObject balancer_object);
  void appendBalancerObject(Json::Value balancer_object);
  Json::Value toObject();
  void setObject(Json::Value& root);

private:
  Json::Value rules = Json::arrayValue;
  Json::Value balancers = Json::arrayValue;
};

struct LevelObject
{
  int level = 0;
  int handshake = 4;
  int conn_idle = 300;
  int uplink_only = 2;
  int downlink_only = 5;
  bool stats_user_uplink = true;
  bool stats_user_downlink = true;
  int buffer_size = 10240;

  Json::Value toObject();
};

struct LevelPolicyObject
{
  void insertLevelObject(LevelObject& level_object);
  void insertLevelObject(Json::Value& level_object, int level);
  Json::Value toObject();

private:
  Json::Value level_policy_object;
};

struct SystemPolicyObject
{
  bool stats_inbound_uplink = true;
  bool stats_inbound_downlink = true;
  bool stats_outbound_uplink = true;
  bool stats_outbound_downlink = true;

  Json::Value toObject();
};

struct PolicyObject
{
  void setLevelPolicyObject(Json::Value& level_policy_object);
  void setLevelPolicyObject(LevelPolicyObject& level_policy_object);
  void setSystemPolicyObject(Json::Value& system_policy_object);
  void setSystemPolicyObject(SystemPolicyObject& system_policy_object);
  void setObject(Json::Value& root);

private:
  Json::Value level_policy_object;
  Json::Value system_policy_object;
};

struct Stats
{
  void setObject(Json::Value& root);
};

struct InboundObject
{
  std::string listen = "127.0.0.1";
  std::string port = "1080";
  std::string protocol = "";
  Json::Value settings;
  Json::Value stream_settings;
  std::string tag = "";

  void setSniffingObject(Json::Value sniffing_object);
  void setSniffingObject(SniffingObject sniffing_object);

  void setAllocate(const std::string& strategy, int refresh, uint concurrency);
  AllocateObject createAllocateObject(const std::string& strategy,
                                      int refresh,
                                      uint concurrency);
  void setAllocateObject(Json::Value allocate_object);
  void setAllocateObject(AllocateObject allocate_object);
  Json::Value toObject();

private:
  Json::Value sniffing;
  Json::Value allocate;
};

struct InboundObjects
{
  void appendInboundObject(Json::Value inbound_object);
  void appendInboundObject(InboundObject inbound_object);
  void setObject(Json::Value& root);

private:
  Json::Value inbounds = Json::arrayValue;
};

struct ProxySettingsObject
{
  std::string tag = "";
  bool transport_layer = false;

  Json::Value toObject();
};

struct MuxObject
{
  bool enabled = false;
  int concurrency = 8;

  Json::Value toObject();
};

struct OutboundObject
{
  std::string send_through = "0.0.0.0";
  std::string protocol = "";
  std::string tag = "PROXY";
  Json::Value servers = Json::arrayValue;
  Json::Value stream_settings;
  Json::Value proxy_settings;
  Json::Value mux;

  void setProxySettingsObject(Json::Value proxy_settings_object);
  void setProxySettingsObject(ProxySettingsObject proxy_settings_object);
  void setMuxObject(Json::Value mux_object);
  void setMuxObject(MuxObject mux_object);
  void appendShadowsocksObject(
    ShadowsocksObject::OutboundSettingObject& shadowsocks_object);
  void appendTrojanObject(TrojanObject::OutboundSettingObject& trojan_object);
  void appendVMessObject(VMessObject::OutboundSettingObject& vmess_object);
  void setTransportStreamObject(
    TransportObject::OutboundStreamObject& trojan_stream_object);
  Json::Value toObject();
};

struct OutboundObjects
{
  void appendOutboundObject(Json::Value outbound_object);
  void appendOutboundObject(OutboundObject outbound_object);
  void setObject(Json::Value& root);

private:
  Json::Value outbounds = Json::arrayValue;
};

enum SchemeType : int
{
  ss,
  vmess,
  trojan
};

struct URLMetaObject
{
  std::string name = "";
  std::string address = "";
  uint port = 0;
  std::string password = "";

  across::config::OutboundObject outbound_object;
};

class JsonTools
{
public:
  JsonTools();

  ~JsonTools();

  bool setData(const std::string& data_str);

  Json::Value getRoot();

  static bool jsonParse(const std::string& raw_json,
                        Json::Value& result,
                        Json::String& err_msg);

private:
  std::string m_data = "";
  Json::Value m_root;
};
}
}

#endif // JSONTOOLS_H
