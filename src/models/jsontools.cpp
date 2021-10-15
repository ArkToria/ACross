#include "jsontools.h"

using namespace across;
using namespace across::config;

JsonTools::JsonTools() {}

JsonTools::~JsonTools() {}

bool
JsonTools::setData(const std::string& data_str)
{
  m_data = data_str;

  Json::string_t msg;

  auto err = jsonParse(m_data, m_root, msg);

  return err;
}

Json
JsonTools::getRoot()
{
  return m_root;
}

bool
JsonTools::jsonParse(const std::string& raw_json,
                     Json& result,
                     Json::string_t& err_msg)
{
  result = Json::parse(raw_json);
  return !result.is_null();
}

Json
SIP008::toObject(int index)
{
  Json root;

  do {
    if (index >= this->servers.size()) {
      break;
    }

    Json server_object;
    Json::array_t servers;

    auto node = this->servers.at(index);

    server_object["address"] = node.server;
    server_object["method"] = node.method;
    server_object["password"] = node.password;
    server_object["port"] = node.server_port;
    server_object["email"] = node.email;
    servers.emplace_back(server_object);

    root["protocol"] = "shadowsocks";
    root["settings"]["servers"] = servers;
  } while (false);

  return root;
}

void
LogObject::setLogLevel(LogLevel level)
{
  this->log_level = magic_enum::enum_name(level);
}

void
LogObject::setObject(Json& root)
{
  root["log"]["loglevel"] = this->log_level;
  root["log"]["access"] = this->access_log_path;
  root["log"]["error"] = this->error_log_path;
}

Json
HttpObject::AccountObject::toObject()
{
  Json root;

  root["user"] = this->username;
  root["pass"] = this->password;

  return root;
}

void
HttpObject::InboundSettingObject::appendAccountObject(AccountObject account)
{
  this->accounts.emplace_back(account.toObject());
}

void
HttpObject::InboundSettingObject::appendAccountObject(Json account)
{
  this->accounts.emplace_back(account);
}

Json
HttpObject::InboundSettingObject::toObject()
{
  Json root;

  root["timeout"] = this->timeout;
  root["allowTransparent"] = this->allow_transparent;
  root["userLevel"] = this->user_level;

  if (!this->accounts.empty()) {
    root["accounts"] = this->accounts;
  }

  return root;
}

void
HttpObject::OutboundSettingObject::appendUserObject(AccountObject user)
{
  this->users.emplace_back(user.toObject());
}

void
HttpObject::OutboundSettingObject::appendUserObject(Json user)
{
  this->users.emplace_back(user);
}

Json
HttpObject::OutboundSettingObject::toObject()
{
  Json root;

  root["address"] = this->address;
  root["port"] = this->port;

  if (!this->users.empty()) {
    root["users"] = this->users;
  }

  return root;
}

Json
SocksObject::AccountObject::toObject()
{
  Json root;

  root["user"] = this->username;
  root["pass"] = this->password;

  return root;
}

void
SocksObject::InboundSettingObject::appendAccountObject(AccountObject account)
{
  this->accounts.emplace_back(account.toObject());
  this->auth = "password";
}

void
SocksObject::InboundSettingObject::appendAccountObject(Json account)
{
  this->accounts.emplace_back(account);
  this->auth = "password";
}

Json
SocksObject::InboundSettingObject::toObject()
{
  Json root;

  if (!this->auth.empty()) {
    root["auth"] = this->auth;
  }

  if (!this->accounts.empty()) {
    root["accounts"] = this->accounts;
  }

  root["udp"] = this->udp;

  if (!this->ip.empty()) {
    root["ip"] = this->ip;
  }

  root["user_level"] = this->user_level;

  return root;
}

void
SocksObject::OutboundSettingObject::appendUserObject(AccountObject user)
{
  this->users.emplace_back(user.toObject());
}

void
SocksObject::OutboundSettingObject::appendUserObject(Json user)
{
  this->users.emplace_back(user);
}

Json
SocksObject::OutboundSettingObject::toObject()
{
  Json root;

  root["address"] = this->address;

  root["port"] = this->port;

  if (!this->users.empty()) {
    root["users"] = this->users;
  }

  return root;
}

bool
ShadowsocksObject::checkMethod(const std::string& method)
{
  const std::list<std::string> methods = {
    "aes-256-gcm", "aes-128-gcm", "chacha20-poly1305", "chacha20-ietf-poly1305",
    "none",        "plain"
  };

  auto iter = std::find(methods.begin(), methods.end(), method);

  if (iter != methods.end()) {
    return true;
  } else {
    return false;
  }
}

bool
ShadowsocksObject::InboundSettingObject::setNetwork(const std::string& type)
{
  const std::list<std::string> types = { "tcp", "udp", "tcp,udp" };

  auto iter = std::find(types.begin(), types.end(), type);

  if (iter != types.end()) {
    return true;
  } else {
    return false;
  }
}

bool
ShadowsocksObject::InboundSettingObject::setMethod(const std::string& method)
{
  bool result = checkMethod(method);

  if (result) {
    this->method = method;
  }

  return result;
}

Json
ShadowsocksObject::InboundSettingObject::toObject()
{
  Json root;

  root["method"] = this->method;
  root["password"] = this->password;
  root["network"] = this->network;
  root["level"] = this->level;
  root["ivCheck"] = this->iv_check;

  if (!this->email.empty()) {
    root["email"] = this->email;
  }

  return root;
}

bool
ShadowsocksObject::OutboundSettingObject::setMethod(const std::string& method)
{
  bool result = checkMethod(method);

  if (result) {
    this->method = method;
  }

  return result;
}

bool
ShadowsocksObject::OutboundSettingObject::fromSIP008Server(
  const SIP008::Server& server)
{
  this->address = server.server;
  this->port = server.server_port;
  this->password = server.password;
  this->email = server.email;

  auto result = setMethod(server.method);

  return result;
}

Json
ShadowsocksObject::OutboundSettingObject::toObject()
{
  Json root;

  root["method"] = this->method;
  root["password"] = this->password;
  root["level"] = this->level;
  root["ivCheck"] = this->iv_check;
  root["address"] = this->address;
  root["port"] = this->port;

  if (!this->email.empty()) {
    root["email"] = this->email;
  }

  return root;
}

Json
VMessObject::ClientObject::toObject()
{
  Json root;

  root["id"] = this->id;
  root["level"] = this->level;
  root["alterId"] = this->alter_id;

  if (!this->email.empty()) {
    root["email"] = this->email;
  }

  return root;
}

Json
VMessObject::DefaultObject::toObject()
{
  Json root;

  root["level"] = this->level;
  root["alterId"] = this->alter_id;

  return root;
}

Json
VMessObject::DetourObject::toObject()
{
  Json root;

  root["to"] = this->tag;

  return root;
}

void
VMessObject::InboundSettingObject::appendClientObject(Json client)
{
  this->client_objects.emplace_back(client);
}

void
VMessObject::InboundSettingObject::appendClientObject(ClientObject client)
{
  this->client_objects.emplace_back(client.toObject());
}

Json
VMessObject::InboundSettingObject::toObject()
{
  Json root;

  if (!client_objects.empty()) {
    root["clients"] = this->client_objects;
  }

  if (!default_object.is_null() && !default_object.empty()) {
    root["default"] = this->default_object;
  }

  if (!detour_object.is_null() && !detour_object.empty()) {
    root["detour"] = this->detour_object;
  }

  root["disableInsecureEncryption"] = this->disable_insecure_encryption;

  return root;
}

bool
VMessObject::UserObject::setMethod(const std::string& method)
{
  const std::list<std::string> methods = {
    "aes-128-gcm", "chacha20-poly1305", "auto", "none", "zero"
  };

  auto iter = std::find(methods.begin(), methods.end(), method);

  if (iter != methods.end()) {
    this->method = method;
    return true;
  } else {
    return false;
  }
}

Json
VMessObject::UserObject::toObject()
{
  Json root;

  root["id"] = this->id;
  root["alterId"] = this->alter_id;
  root["security"] = this->method;
  root["level"] = this->level;

  return root;
}

void
VMessObject::OutboundSettingObject::appendUserObject(Json user)
{
  this->user_objects.emplace_back(user);
}

void
VMessObject::OutboundSettingObject::appendUserObject(UserObject user)
{
  this->user_objects.emplace_back(user.toObject());
}

Json
VMessObject::OutboundSettingObject::toObject()
{
  Json root;

  root["address"] = this->address;
  root["port"] = this->port;
  root["users"] = this->user_objects;

  return root;
}

bool
SniffingObject::appendDestOverride(const std::string& type)
{
  const std::list<std::string> types = {
    "http", "tls", "fakedns", "fakedns+others"
  };

  if (type == "fakedns+others") {
    this->metadata_only = false; // [v4.38.0+] supported

    this->dest_override.emplace_back(type);

    return true;
  }

  auto iter = std::find(types.begin(), types.end(), type);

  if (iter != types.end()) {
    this->dest_override.emplace_back(type);

    return true;
  } else {
    return false;
  }
}

Json
SniffingObject::toObject()
{
  Json root;

  root["enabled"] = this->enabled;
  root["destOverride"] = this->dest_override;
  root["metadataOnly"] = this->metadata_only;

  return root;
}

bool
AllocateObject::setStrategy(const std::string& type)
{
  if (type == "random" || type == "always") {
    this->strategy = type;
    return true;
  } else {
    return false;
  }
}

bool
AllocateObject::setRefresh(uint min)
{
  if (this->strategy != "random") {
    return false;
  }

  if (min >= 2) {
    this->refresh = min;

    return true;
  } else {
    return false;
  }
}

bool
AllocateObject::setConcurrency(uint num, std::string port_info)
{
  bool result = false;

  do {
    auto found_env = port_info.find("env:");
    if (found_env != std::string::npos || num == 0) {
      this->concurrency = num; // pass through error to core
      result = true;
      break;
    }

    auto found_range = port_info.find("-"); // split string by '-'
    if (found_range != std::string::npos) {
      std::pair<int, int> port_range;

      port_range.first = std::stoi(port_info.substr(0, found_range));
      port_range.second =
        std::stoi(port_info.substr(found_range + 1, port_info.size()));

      if (port_range.second - port_range.first > 0 &&
          num < (port_range.second - port_range.first) / 3 && num >= 1) {
        this->concurrency = num;
        result = true;
        break;
      }
    }
  } while (false);

  return result;
}

Json
AllocateObject::toObject()
{
  Json root;

  root["strategy"] = this->strategy;
  root["refresh"] = this->refresh;
  root["concurrency"] = this->concurrency;

  return root;
}

AllocateObject
InboundObject::createAllocateObject(const std::string& strategy,
                                    int refresh,
                                    uint concurrency)
{
  AllocateObject allocate_object;

  allocate_object.setStrategy(strategy);
  allocate_object.setRefresh(refresh);
  allocate_object.setConcurrency(concurrency, this->port);

  return allocate_object;
}

void
InboundObject::setSniffingObject(Json sniffing_object)
{
  this->sniffing = sniffing_object;
}

void
InboundObject::setSniffingObject(SniffingObject sniffing_object)
{
  this->sniffing = sniffing_object.toObject();
}

void
InboundObject::setAllocate(const std::string& strategy,
                           int refresh,
                           uint concurrency)
{
  auto allocate_object = createAllocateObject(strategy, refresh, concurrency);

  setAllocateObject(allocate_object);
}

void
InboundObject::setAllocateObject(Json allocate_object)
{
  this->allocate = allocate_object;
}

void
InboundObject::setAllocateObject(AllocateObject allocate_object)
{
  this->allocate = allocate_object.toObject();
}

Json
InboundObject::toObject()
{
  Json root;

  if (this->listen.size() != 0) {
    root["listen"] = this->listen;
  }

  root["port"] = this->port;

  root["protocol"] = this->protocol;

  if (this->tag.size() != 0) {
    root["tag"] = this->tag;
  }

  if (!this->settings.is_null() && !this->settings.empty()) {
    root["settings"] = this->settings;
  }

  if (!this->stream_settings.is_null() && !this->stream_settings.empty()) {
    root["streamSettings"] = this->stream_settings;
  }

  if (!this->sniffing.is_null() && !this->sniffing.empty()) {
    root["sniffing"] = this->sniffing;
  }

  if (!this->allocate.is_null() && !this->allocate.empty() &&
      this->allocate["concurrency"] != 0) {
    root["allocate"] = this->allocate;
  }

  return root;
}

void
InboundObjects::appendInboundObject(Json inbound_object)
{
  this->inbounds.emplace_back(inbound_object);
}

void
InboundObjects::appendInboundObject(InboundObject inbound_object)
{
  this->inbounds.emplace_back(inbound_object.toObject());
}

void
InboundObjects::setObject(Json& root)
{
  root["inbounds"] = this->inbounds;
}

Json
ProxySettingsObject::toObject()
{
  Json root;

  root["tag"] = this->tag;

  root["transportLayer"] = this->transport_layer;

  return root;
}

Json
MuxObject::toObject()
{
  Json root;

  root["enabled"] = this->enabled;

  if (this->concurrency == -1 ||
      (this->concurrency >= 1 && this->concurrency <= 1024)) {
    root["concurrency"] = this->concurrency;
  } else {
    root["concurrency"] = 8; // default 8
  }

  return root;
}

void
OutboundObject::setProxySettingsObject(Json proxy_settings_object)
{
  this->proxy_settings = proxy_settings_object;
}

void
OutboundObject::setProxySettingsObject(
  ProxySettingsObject proxy_settings_object)
{
  this->proxy_settings = proxy_settings_object.toObject();
}

void
OutboundObject::setMuxObject(Json mux_object)
{
  this->mux = mux_object;
}

void
OutboundObject::setMuxObject(MuxObject mux_object)
{
  this->mux = mux_object.toObject();
}

void
OutboundObject::appendShadowsocksObject(
  ShadowsocksObject::OutboundSettingObject& shadowsocks_object)
{
  this->protocol = "shadowsocks";

  servers.emplace_back(shadowsocks_object.toObject());
}

void
OutboundObject::appendTrojanObject(
  TrojanObject::OutboundSettingObject& trojan_object)
{
  this->protocol = "trojan";

  servers.emplace_back(trojan_object.toObject());
}

void
OutboundObject::appendVMessObject(
  VMessObject::OutboundSettingObject& vmess_object)
{
  this->protocol = "vmess";

  servers.emplace_back(vmess_object.toObject());
}

void
OutboundObject::setTransportStreamObject(
  TransportObject::OutboundStreamObject& trojan_stream_object)
{
  this->stream_settings = trojan_stream_object.toObject();
}

Json
OutboundObject::toObject()
{
  Json root;

  root["sendThrough"] = this->send_through;

  root["protocol"] = this->protocol;

  if (!this->servers.empty()) {
    if (this->protocol == "vmess") {
      root["settings"]["vnext"] = this->servers;
    } else {
      root["settings"]["servers"] = this->servers;
    }
  }

  if (!this->tag.empty()) {
    root["tag"] = this->tag;
  }

  if (!this->stream_settings.is_null() && !this->stream_settings.empty()) {
    root["streamSettings"] = this->stream_settings;
  }

  if (!this->proxy_settings.is_null() && !this->proxy_settings.empty()) {
    root["proxySettings"] = this->proxy_settings;
  }

  if (!this->mux.is_null() && !this->mux.empty()) {
    root["mux"] = this->mux;
  }

  return root;
}

void
OutboundObjects::appendOutboundObject(Json outbound_object)
{
  this->outbounds.emplace_back(outbound_object);
}

void
OutboundObjects::appendOutboundObject(OutboundObject outbound_object)
{
  this->outbounds.emplace_back(outbound_object.toObject());
}

void
OutboundObjects::setObject(Json& root)
{
  root["outbounds"] = this->outbounds;
}

Json
TrojanObject::ClientObject::toObject()
{
  Json root;

  root["password"] = this->password;
  root["level"] = this->level;

  if (!this->email.empty()) {
    root["email"] = this->email;
  }

  return root;
}

Json
TrojanObject::FallbackObject::toObject()
{
  Json root;

  root["alpn"] = this->alpn;
  root["path"] = this->path;
  root["dest"] = this->dest;
  root["xver"] = this->xver;

  return root;
}

void
TrojanObject::InboundSettingObject::appendClientObject(Json client)
{
  this->client_objects.emplace_back(client);
}

void
TrojanObject::InboundSettingObject::appendClientObject(ClientObject client)
{
  this->client_objects.emplace_back(client.toObject());
}

void
TrojanObject::InboundSettingObject::appendFallbackObject(Json fallback)
{
  this->fallback_objects.emplace_back(fallback);
}

void
TrojanObject::InboundSettingObject::appendFallbackObject(
  FallbackObject fallback)
{
  this->fallback_objects.emplace_back(fallback.toObject());
}

Json
TrojanObject::InboundSettingObject::toObject()
{
  Json root;

  if (!client_objects.empty()) {
    root["clients"] = this->client_objects;
  }

  if (!fallback_objects.empty()) {
    root["fallbacks"] = this->fallback_objects;
  }

  return root;
}

Json
TrojanObject::OutboundSettingObject::toObject()
{
  Json root;

  root["address"] = this->address;
  root["port"] = this->port;
  root["password"] = this->password;
  root["level"] = this->level;

  if (!this->email.empty()) {
    root["email"] = this->email;
  }

  return root;
}

void
TransportObject::TLSObject::appendAlpn(const std::string& value)
{
  if (!value.empty()) {
    this->alpn_object.emplace_back(value);
  }
}

void
TransportObject::TLSObject::appendCertificateObject(Json certificate)
{
  this->certificate_objects.emplace_back(certificate);
}

void
TransportObject::TLSObject::appendCertificateObject(
  CertificateObject certificate)
{
  this->certificate_objects.emplace_back(certificate.toObject());
}

Json
TransportObject::TLSObject::toObject()
{
  Json root;

  root["serverName"] = this->server_name;
  root["allowInsecure"] = this->allow_insecure;
  root["disableSystemRoot"] = this->disable_system_root;

  if (!alpn_object.is_null() && !alpn_object.empty()) {
    root["alpn"] = this->alpn_object;
  }

  if (!certificate_objects.empty()) {
    root["certificates"] = this->certificate_objects;
  }

  return root;
}

void
TransportObject::CertificateObject::appendCertificate(const std::string& value)
{
  if (!value.empty()) {
    this->certificate_objects.emplace_back(value);
  }
}

void
TransportObject::CertificateObject::appendKey(const std::string& value)
{
  if (!value.empty()) {
    this->key_objects.emplace_back(value);
  }
}

Json
TransportObject::CertificateObject::toObject()
{
  Json root;

  root["usage"] = this->usage;
  root["certificateFile"] = this->certificate_file;
  root["keyFile"] = this->key_file;

  if (!this->certificate_objects.empty()) {
    root["certificate"] = this->certificate_objects;
  }

  if (!this->key_objects.empty()) {
    root["key"] = this->key_objects;
  }

  return root;
}

void
TransportObject::OutboundStreamObject::setTLSSetting(Json tls_object)
{
  this->tls_setting = tls_object;
}

void
TransportObject::OutboundStreamObject::setTLSSetting(TLSObject tls_object)
{
  this->tls_setting = tls_object.toObject();
}

void
TransportObject::OutboundStreamObject::setWebsocketSetting(
  Json websocket_object)
{
  this->websocket_setting = websocket_object;
}

void
TransportObject::OutboundStreamObject::setWebsocketSetting(
  WebSocketObject websocket_object)
{
  this->websocket_setting = websocket_object.toObject();
}

Json
TransportObject::OutboundStreamObject::toObject()
{
  Json root;

  root["network"] = magic_enum::enum_name(this->network).data();

  if (!this->security.empty()) {
    root["security"] = this->security;
  }

  if (!this->tls_setting.empty()) {
    root["tlsSettings"] = this->tls_setting;
  }
  if (!this->websocket_setting.empty()) {
    root["wsSettings"] = this->websocket_setting;
  }

  return root;
}

void
TransportObject::WebSocketObject::setHost(const std::string& value)
{
  if (value.empty()) {
    return;
  }

  this->headers["Host"] = value;
}

Json
TransportObject::WebSocketObject::toObject()
{
  Json root;

  root["acceptProxyProtocol"] = this->accept_proxy_protocol;
  root["path"] = this->path;
  root["useBrowserForwarding"] = this->use_browser_forwarding;

  if (!this->early_data_header_name.empty()) {
    root["earlyDataHeaderName"] = this->early_data_header_name;
  }

  if (this->max_early_data != 0) {
    root["maxEarlyData"] = this->max_early_data;
  }

  for (auto& item : this->headers) {
    root["headers"][item.first] = item.second;
  }

  return root;
}

APIObject::APIObject()
{
  services.clear();
  for (auto& item : { "ReflectionService",
                      "HandlerService",
                      "LoggerService",
                      "StatsService" }) {
    services.emplace_back(item);
  }
}

void
APIObject::setObject(Json& root)
{
  root["api"]["tag"] = this->tag.toStdString();
  root["api"]["services"] = this->services;
}

Json
DokodemoDoorObject::InboundSettingObject::toObject()
{
  Json root;

  root["address"] = this->address;
  root["port"] = this->port;
  root["network"] = this->network;
  root["timeout"] = this->timeout;
  root["followRedirect"] = this->follow_redirect;
  root["userLevel"] = this->user_level;

  return root;
}

Json
RuleObject::toObject()
{
  Json root;

  if (!this->domain_matche.empty())
    root["domainMatcher"] = this->domain_matche;

  if (!this->type.empty())
    root["type"] = this->type;

  if (!this->domains.empty())
    root["domains"] = this->domains;

  if (!this->ip.empty())
    root["ip"] = this->ip;

  if (!this->port.empty() && this->port != "0")
    root["port"] = this->port;

  if (!this->source_port.empty() && this->source_port != "0")
    root["sourcePort"] = this->source_port;

  if (!this->network.empty())
    root["network"] = this->network;

  if (!this->source.empty())
    root["source"] = this->source;

  if (!this->user.empty())
    root["user"] = this->user;

  if (!this->inbound_tag.empty())
    root["inboundTag"] = this->inbound_tag;

  if (!this->protocol.empty())
    root["protocol"] = this->protocol;

  if (!this->attrs.empty())
    root["attrs"] = this->attrs;

  if (!this->outbound_tag.empty())
    root["outboundTag"] = this->outbound_tag;

  if (!this->balancer_tag.empty())
    root["balancerTag"] = this->balancer_tag;

  return root;
}

void
RoutingObject::appendRuleObject(RuleObject rule_object)
{
  this->rules.emplace_back(rule_object.toObject());
}

void
RoutingObject::appendRuleObject(Json rule_object)
{
  this->rules.emplace_back(rule_object);
}

void
RoutingObject::appendBalancerObject(BalancerObject balancer_object)
{
  this->balancers.emplace_back(balancer_object.toObject());
}

void
RoutingObject::appendBalancerObject(Json balancer_object)
{
  this->balancers.emplace_back(balancer_object);
}

Json
RoutingObject::toObject()
{
  Json root;

  if (!this->domain_strategy.empty()) {
    root["domainStrategy"] = this->domain_strategy;
  }

  if (!this->domain_matche.empty()) {
    root["domainMatcher"] = this->domain_matche;
  }

  if (!this->rules.empty()) {
    root["rules"] = this->rules;
  }
  if (!this->balancers.empty()) {
    root["balancers"] = this->balancers;
  }

  return root;
}

void
RoutingObject::setObject(Json& root)
{
  root["routing"] = this->toObject();
}

Json
StrategyObject::toObject()
{
  Json root;

  root["type"] = this->type;

  return root;
}

void
BalancerObject::setStrategyObject(StrategyObject strategy_object)
{
  this->strategy = strategy_object.toObject();
}

void
BalancerObject::setStrategyObject(Json strategy_object)
{
  this->strategy = strategy_object;
}

Json
BalancerObject::toObject()
{
  Json root;

  root["tag"] = this->tag;
  root["selector"] = this->selector;
  root["strategy"] = this->strategy;

  return root;
}

void
Stats::setObject(Json& root)
{
  Json empty = Json::object();
  root["stats"] = empty;
}

Json
LevelObject::toObject()
{
  Json root;

  root["handshake"] = this->handshake;
  root["connIdle"] = this->conn_idle;
  root["uplinkOnly"] = this->uplink_only;
  root["downlinkOnly"] = this->downlink_only;
  root["statsUserUplink"] = this->stats_user_uplink;
  root["statsUserDownlink"] = this->stats_user_downlink;
  root["bufferSize"] = this->buffer_size;

  return root;
}

void
LevelPolicyObject::insertLevelObject(LevelObject& level_object)
{
  this->level_policy_object[std::to_string(level_object.level)] =
    level_object.toObject();
}

void
LevelPolicyObject::insertLevelObject(Json& level_object, int level)
{
  this->level_policy_object[std::to_string(level)] = level_object;
}

Json
LevelPolicyObject::toObject()
{
  return this->level_policy_object;
}

Json
SystemPolicyObject::toObject()
{
  Json root;

  root["statsInboundUplink"] = this->stats_inbound_uplink;
  root["statsInboundDownlink"] = this->stats_inbound_downlink;
  root["statsOutboundUplink"] = this->stats_outbound_uplink;
  root["statsOutboundDownlink"] = this->stats_outbound_downlink;

  return root;
}

void
PolicyObject::setLevelPolicyObject(Json& level_policy_object)
{
  this->level_policy_object = level_policy_object;
}

void
PolicyObject::setLevelPolicyObject(LevelPolicyObject& level_policy_object)
{
  this->level_policy_object = level_policy_object.toObject();
}

void
PolicyObject::setSystemPolicyObject(Json& system_policy_object)
{
  this->system_policy_object = system_policy_object;
}

void
PolicyObject::setSystemPolicyObject(SystemPolicyObject& system_policy_object)
{
  this->system_policy_object = system_policy_object.toObject();
}

void
PolicyObject::setObject(Json& root)
{
  if (!this->level_policy_object.is_null())
    root["policy"]["levels"] = this->level_policy_object;

  if (!this->system_policy_object.is_null())
    root["policy"]["system"] = this->system_policy_object;
}
