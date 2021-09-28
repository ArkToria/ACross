#include "jsontools.h"

using namespace across;
using namespace across::config;

JsonTools::JsonTools() {}

JsonTools::~JsonTools() {}

bool
JsonTools::setData(const std::string& data_str)
{
  m_data = data_str;

  Json::String msg;

  auto err = jsonParse(m_data, m_root, msg);

  return err;
}

Json::Value
JsonTools::getRoot()
{
  return m_root;
}

bool
JsonTools::jsonParse(const std::string& raw_json,
                     Json::Value& result,
                     Json::String& err_msg)
{
  Json::CharReaderBuilder reader_builder;

  const std::unique_ptr<Json::CharReader> reader(
    reader_builder.newCharReader());
  const auto rawJsonLength = static_cast<int>(raw_json.length());
  bool err = reader->parse(
    raw_json.c_str(), raw_json.c_str() + rawJsonLength, &result, &err_msg);

  return err;
}

Json::Value
SIP008::toObject(int index)
{
  Json::Value root;

  do {
    if (index >= this->servers.size()) {
      break;
    }

    Json::Value server_object;
    Json::Value servers(Json::arrayValue);

    auto node = this->servers.at(index);

    server_object["address"] = node.server;
    server_object["method"] = node.method;
    server_object["password"] = node.password;
    server_object["port"] = node.server_port;
    server_object["email"] = node.email;
    servers.append(server_object);

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
LogObject::setObject(Json::Value& root)
{
  root["log"]["loglevel"] = this->log_level;
  root["log"]["access"] = this->access_log_path;
  root["log"]["error"] = this->error_log_path;
}

Json::Value
HttpObject::AccountObject::toObject()
{
  Json::Value root;

  root["user"] = this->username;
  root["pass"] = this->password;

  return root;
}

void
HttpObject::InboundSettingObject::appendAccountObject(AccountObject account)
{
  this->accounts.append(account.toObject());
}

void
HttpObject::InboundSettingObject::appendAccountObject(Json::Value account)
{
  this->accounts.append(account);
}

Json::Value
HttpObject::InboundSettingObject::toObject()
{
  Json::Value root;

  root["timeout"] = this->timeout;
  root["allowTransparent"] = this->allow_transparent;
  root["userLevel"] = this->user_level;

  if (!this->accounts.isNull() && !this->accounts.empty()) {
    root["accounts"] = this->accounts;
  }

  return root;
}

void
HttpObject::OutboundSettingObject::appendUserObject(AccountObject user)
{
  this->users.append(user.toObject());
}

void
HttpObject::OutboundSettingObject::appendUserObject(Json::Value user)
{
  this->users.append(user);
}

Json::Value
HttpObject::OutboundSettingObject::toObject()
{
  Json::Value root;

  root["address"] = this->address;
  root["port"] = this->port;

  if (!this->users.isNull() && !this->users.empty()) {
    root["users"] = this->users;
  }

  return root;
}

Json::Value
SocksObject::AccountObject::toObject()
{
  Json::Value root;

  root["user"] = this->username;
  root["pass"] = this->password;

  return root;
}

void
SocksObject::InboundSettingObject::appendAccountObject(AccountObject account)
{
  this->accounts.append(account.toObject());
  this->auth = "password";
}

void
SocksObject::InboundSettingObject::appendAccountObject(Json::Value account)
{
  this->accounts.append(account);
  this->auth = "password";
}

Json::Value
SocksObject::InboundSettingObject::toObject()
{
  Json::Value root;

  if (!this->auth.empty()) {
    root["auth"] = this->auth;
  }

  if (!this->accounts.isNull() && !this->accounts.empty()) {
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
  this->users.append(user.toObject());
}

void
SocksObject::OutboundSettingObject::appendUserObject(Json::Value user)
{
  this->users.append(user);
}

Json::Value
SocksObject::OutboundSettingObject::toObject()
{
  Json::Value root;

  root["address"] = this->address;

  root["port"] = this->port;

  if (!this->users.isNull() && !this->users.empty()) {
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

Json::Value
ShadowsocksObject::InboundSettingObject::toObject()
{
  Json::Value root;

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

Json::Value
ShadowsocksObject::OutboundSettingObject::toObject()
{
  Json::Value root;

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

Json::Value
VMessObject::ClientObject::toObject()
{
  Json::Value root;

  root["id"] = this->id;
  root["level"] = this->level;
  root["alterId"] = this->alter_id;

  if (!this->email.empty()) {
    root["email"] = this->email;
  }

  return root;
}

Json::Value
VMessObject::DefaultObject::toObject()
{
  Json::Value root;

  root["level"] = this->level;
  root["alterId"] = this->alter_id;

  return root;
}

Json::Value
VMessObject::DetourObject::toObject()
{
  Json::Value root;

  root["to"] = this->tag;

  return root;
}

void
VMessObject::InboundSettingObject::appendClientObject(Json::Value client)
{
  this->client_objects.append(client);
}

void
VMessObject::InboundSettingObject::appendClientObject(ClientObject client)
{
  this->client_objects.append(client.toObject());
}

Json::Value
VMessObject::InboundSettingObject::toObject()
{
  Json::Value root;

  if (!client_objects.isNull() && !client_objects.empty()) {
    root["clients"] = this->client_objects;
  }

  if (!default_object.isNull() && !default_object.empty()) {
    root["default"] = this->default_object;
  }

  if (!detour_object.isNull() && !detour_object.empty()) {
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

Json::Value
VMessObject::UserObject::toObject()
{
  Json::Value root;

  root["id"] = this->id;
  root["alterId"] = this->alter_id;
  root["security"] = this->method;
  root["level"] = this->level;

  return root;
}

void
VMessObject::OutboundSettingObject::appendUserObject(Json::Value user)
{
  this->user_objects.append(user);
}

void
VMessObject::OutboundSettingObject::appendUserObject(UserObject user)
{
  this->user_objects.append(user.toObject());
}

Json::Value
VMessObject::OutboundSettingObject::toObject()
{
  Json::Value root;

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

    this->dest_override.append(type);

    return true;
  }

  auto iter = std::find(types.begin(), types.end(), type);

  if (iter != types.end()) {
    this->dest_override.append(type);

    return true;
  } else {
    return false;
  }
}

Json::Value
SniffingObject::toObject()
{
  Json::Value root;

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

Json::Value
AllocateObject::toObject()
{
  Json::Value root;

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
InboundObject::setSniffingObject(Json::Value sniffing_object)
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
InboundObject::setAllocateObject(Json::Value allocate_object)
{
  this->allocate = allocate_object;
}

void
InboundObject::setAllocateObject(AllocateObject allocate_object)
{
  this->allocate = allocate_object.toObject();
}

Json::Value
InboundObject::toObject()
{
  Json::Value root;

  if (this->listen.size() != 0) {
    root["listen"] = this->listen;
  }

  root["port"] = this->port;

  root["protocol"] = this->protocol;

  if (this->tag.size() != 0) {
    root["tag"] = this->tag;
  }

  if (!this->settings.isNull() && !this->settings.empty()) {
    root["settings"] = this->settings;
  }

  if (!this->stream_settings.isNull() && !this->stream_settings.empty()) {
    root["streamSettings"] = this->stream_settings;
  }

  if (!this->sniffing.isNull() && !this->sniffing.empty()) {
    root["sniffing"] = this->sniffing;
  }

  if (!this->allocate.isNull() && !this->allocate.empty() &&
      this->allocate["concurrency"].asUInt() != 0) {
    root["allocate"] = this->allocate;
  }

  return root;
}

void
InboundObjects::appendInboundObject(Json::Value inbound_object)
{
  this->inbounds.append(inbound_object);
}

void
InboundObjects::appendInboundObject(InboundObject inbound_object)
{
  this->inbounds.append(inbound_object.toObject());
}

void
InboundObjects::setObject(Json::Value& root)
{
  root["inbounds"] = this->inbounds;
}

Json::Value
ProxySettingsObject::toObject()
{
  Json::Value root;

  root["tag"] = this->tag;

  root["transportLayer"] = this->transport_layer;

  return root;
}

Json::Value
MuxObject::toObject()
{
  Json::Value root;

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
OutboundObject::setProxySettingsObject(Json::Value proxy_settings_object)
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
OutboundObject::setMuxObject(Json::Value mux_object)
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

  servers.append(shadowsocks_object.toObject());
}

void
OutboundObject::appendTrojanObject(
  TrojanObject::OutboundSettingObject& trojan_object)
{
  this->protocol = "trojan";

  servers.append(trojan_object.toObject());
}

void
OutboundObject::appendVMessObject(
  VMessObject::OutboundSettingObject& vmess_object)
{
  this->protocol = "vmess";

  servers.append(vmess_object.toObject());
}

void
OutboundObject::setTransportStreamObject(
  TransportObject::OutboundStreamObject& trojan_stream_object)
{
  this->stream_settings = trojan_stream_object.toObject();
}

Json::Value
OutboundObject::toObject()
{
  Json::Value root;

  root["sendThrough"] = this->send_through;

  root["protocol"] = this->protocol;

  if (!this->servers.isNull() && !this->servers.empty()) {
    if (this->protocol == "vmess") {
      root["settings"]["vnext"] = this->servers;
    } else {
      root["settings"]["servers"] = this->servers;
    }
  }

  if (!this->tag.empty()) {
    root["tag"] = this->tag;
  }

  if (!this->stream_settings.isNull() && !this->stream_settings.empty()) {
    root["streamSettings"] = this->stream_settings;
  }

  if (!this->proxy_settings.isNull() && !this->proxy_settings.empty()) {
    root["proxySettings"] = this->proxy_settings;
  }

  if (!this->mux.isNull() && !this->mux.empty()) {
    root["mux"] = this->mux;
  }

  return root;
}

void
OutboundObjects::appendOutboundObject(Json::Value outbound_object)
{
  this->outbounds.append(outbound_object);
}

void
OutboundObjects::appendOutboundObject(OutboundObject outbound_object)
{
  this->outbounds.append(outbound_object.toObject());
}

void
OutboundObjects::setObject(Json::Value& root)
{
  root["outbounds"] = this->outbounds;
}

Json::Value
TrojanObject::ClientObject::toObject()
{
  Json::Value root;

  root["password"] = this->password;
  root["level"] = this->level;

  if (!this->email.empty()) {
    root["email"] = this->email;
  }

  return root;
}

Json::Value
TrojanObject::FallbackObject::toObject()
{
  Json::Value root;

  root["alpn"] = this->alpn;
  root["path"] = this->path;
  root["dest"] = this->dest;
  root["xver"] = this->xver;

  return root;
}

void
TrojanObject::InboundSettingObject::appendClientObject(Json::Value client)
{
  this->client_objects.append(client);
}

void
TrojanObject::InboundSettingObject::appendClientObject(ClientObject client)
{
  this->client_objects.append(client.toObject());
}

void
TrojanObject::InboundSettingObject::appendFallbackObject(Json::Value fallback)
{
  this->fallback_objects.append(fallback);
}

void
TrojanObject::InboundSettingObject::appendFallbackObject(
  FallbackObject fallback)
{
  this->fallback_objects.append(fallback.toObject());
}

Json::Value
TrojanObject::InboundSettingObject::toObject()
{
  Json::Value root;

  if (!client_objects.isNull() && !client_objects.empty()) {
    root["clients"] = this->client_objects;
  }

  if (!fallback_objects.isNull() && !fallback_objects.empty()) {
    root["fallbacks"] = this->fallback_objects;
  }

  return root;
}

Json::Value
TrojanObject::OutboundSettingObject::toObject()
{
  Json::Value root;

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
    this->alpn_object.append(value);
  }
}

void
TransportObject::TLSObject::appendCertificateObject(Json::Value certificate)
{
  this->certificate_objects.append(certificate);
}

void
TransportObject::TLSObject::appendCertificateObject(
  CertificateObject certificate)
{
  this->certificate_objects.append(certificate.toObject());
}

Json::Value
TransportObject::TLSObject::toObject()
{
  Json::Value root;

  root["serverName"] = this->server_name;
  root["allowInsecure"] = this->allow_insecure;
  root["disableSystemRoot"] = this->disable_system_root;

  if (!alpn_object.isNull() && !alpn_object.empty()) {
    root["alpn"] = this->alpn_object;
  }

  if (!certificate_objects.isNull() && !certificate_objects.empty()) {
    root["certificates"] = this->certificate_objects;
  }

  return root;
}

void
TransportObject::CertificateObject::appendCertificate(const std::string& value)
{
  if (!value.empty()) {
    this->certificate_objects.append(value);
  }
}

void
TransportObject::CertificateObject::appendKey(const std::string& value)
{
  if (!value.empty()) {
    this->key_objects.append(value);
  }
}

Json::Value
TransportObject::CertificateObject::toObject()
{
  Json::Value root;

  root["usage"] = this->usage;
  root["certificateFile"] = this->certificate_file;
  root["keyFile"] = this->key_file;

  if (!this->certificate_objects.isNull() &&
      !this->certificate_objects.empty()) {
    root["certificate"] = this->certificate_objects;
  }

  if (!this->key_objects.isNull() && !this->key_objects.empty()) {
    root["key"] = this->key_objects;
  }

  return root;
}

void
TransportObject::OutboundStreamObject::setTLSSetting(Json::Value tls_object)
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
  Json::Value websocket_object)
{
  this->websocket_setting = websocket_object;
}

void
TransportObject::OutboundStreamObject::setWebsocketSetting(
  WebSocketObject websocket_object)
{
  this->websocket_setting = websocket_object.toObject();
}

Json::Value
TransportObject::OutboundStreamObject::toObject()
{
  Json::Value root;

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

Json::Value
TransportObject::WebSocketObject::toObject()
{
  Json::Value root;

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
    services.append(item);
  }
}

void
APIObject::setObject(Json::Value& root)
{
  root["api"]["tag"] = this->tag.toStdString();
  root["api"]["services"] = this->services;
}

Json::Value
DokodemoDoorObject::InboundSettingObject::toObject()
{
  Json::Value root;

  root["address"] = this->address;
  root["port"] = this->port;
  root["network"] = this->network;
  root["timeout"] = this->timeout;
  root["followRedirect"] = this->follow_redirect;
  root["userLevel"] = this->user_level;

  return root;
}

Json::Value
RuleObject::toObject()
{}

void
RoutingObject::appendRuleObject(RuleObject rule_object)
{}

Json::Value
RoutingObject::toObject()
{}

void
RoutingObjects::appendRoutingObject(Json::Value routing_object)
{}

void
RoutingObjects::appendRoutingObject(RoutingObject routing_object)
{}

Json::Value
StrategyObject::toObject()
{}

void
BalancerObject::setStrategyObject(StrategyObject strategy_object)
{}

void
BalancerObject::setStrategyObject(Json::Value strategy_object)
{}

Json::Value
BalancerObject::toObject()
{}
