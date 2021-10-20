#include "nodeformmodel.h"

using namespace across;

NodeFormModel::NodeFormModel(QObject* parent)
  : QObject(parent)
  , p_trojan(nullptr)
  , p_shadowsocks(nullptr)
  , p_vmess(nullptr)
  , p_list(nullptr)
{}

const QString&
NodeFormModel::name() const
{
  return m_name;
}

void
NodeFormModel::setName(const QString& newName)
{
  if (m_name == newName)
    return;

  m_name = newName;

  emit nameChanged();
}

const QString&
NodeFormModel::address() const
{
  return m_address;
}

void
NodeFormModel::setAddress(const QString& newAddress)
{
  if (m_address == newAddress)
    return;

  m_address = newAddress;

  emit addressChanged();
}

int
NodeFormModel::port() const
{
  return m_port;
}

void
NodeFormModel::setPort(int newPort)
{
  if (m_port == newPort)
    return;

  m_port = newPort;

  emit portChanged();
}

const QString&
NodeFormModel::password() const
{
  return m_password;
}

void
NodeFormModel::setPassword(const QString& newPassword)
{
  if (m_password == newPassword)
    return;

  m_password = newPassword;

  emit passwordChanged();
}

const QString&
NodeFormModel::protocol() const
{
  return m_protocol;
}

void
NodeFormModel::accept()
{
  NodeInfo node;

  node.name = m_name;
  node.address = m_address;
  node.protocol =
    magic_enum::enum_cast<EntryType>(m_protocol.toStdString()).value();
  node.port = m_port;
  node.password = m_password;

  switch (node.protocol) {
    case EntryType::trojan:
      setTrojanOutbound(node);
      break;
    case EntryType::shadowsocks:
      setShadowsocksOutbound(node);
      break;
    case EntryType::vmess:
      setVMessOutboud(node);
      break;
    case EntryType::raw:
      setRawOutbound(node);
      break;
    case EntryType::scheme:
      decodeOutboundFromURL(node);
      break;
    default:
      break;
  }

  p_list->appendNode(node);

  return;
}

void
NodeFormModel::cancel()
{
  setName("");
  setAddress("");
  setPort(0);
  setPassword("");
  setProtocol("");

  p_trojan = nullptr;
  p_shadowsocks = nullptr;
  p_vmess = nullptr;
}

void
NodeFormModel::setProtocol(const QString& newProtocol)
{
  if (m_protocol == newProtocol)
    return;

  m_protocol = newProtocol;

  emit protocolChanged();
}

TrojanFormModel*
NodeFormModel::trojan() const
{
  return p_trojan;
}

void
NodeFormModel::setTrojan(TrojanFormModel* newTrojan)
{
  if (newTrojan != nullptr) {
    p_trojan = newTrojan;
  }

  emit trojanChanged();
}

bool
NodeFormModel::setTrojanOutbound(NodeInfo& node)
{
  if (p_trojan == nullptr) {
    return false;
  }

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
  stream->set_network(p_trojan->network().toStdString());
  stream->set_security(p_trojan->security().toStdString());

  auto tls = stream->mutable_tlssettings();
  tls->set_allowinsecure(false);
  tls->set_servername(p_trojan->serverName().toStdString());

  if (p_trojan->alpn() == "h2+http/1.1") {
    tls->add_alpn("h2");
    tls->add_alpn("http/1.1");
  } else {
    tls->add_alpn(p_trojan->alpn().toStdString());
  }

  node.raw = SerializeTools::MessageToJson(*outbound).c_str();

  URLMetaObject meta = {
    .name = node.name.toStdString(),
    .outbound = *outbound,
  };

  node.url = SerializeTools::trojanEncode(meta)->toEncoded();

  return true;
}

bool
NodeFormModel::setShadowsocksOutbound(NodeInfo& node)
{
  if (p_shadowsocks == nullptr) {
    return false;
  }

  auto outbound = m_config.add_outbounds();
  outbound->set_protocol("shadowsocks");
  outbound->set_sendthrough("0.0.0.0");
  outbound->set_tag("PROXY");

  auto setting = outbound->mutable_settings()->mutable_shadowsocks();

  auto server = setting->add_servers();
  server->set_address(node.address.toStdString());
  server->set_port(node.port);
  server->set_password(node.password.toStdString());
  server->set_method(p_shadowsocks->security().toStdString());
  server->set_ivcheck(p_shadowsocks->ivCheck());

  node.raw = SerializeTools::MessageToJson(*outbound).c_str();

  URLMetaObject meta = {
    .name = node.name.toStdString(),
    .outbound = *outbound,
  };

  node.url = SerializeTools::sip002Encode(meta)->toEncoded();

  return true;
}

bool
NodeFormModel::setVMessOutboud(NodeInfo& node)
{
  if (p_vmess == nullptr) {
    return false;
  }

  auto outbound = m_config.add_outbounds();
  outbound->set_protocol("vmess");
  outbound->set_sendthrough("0.0.0.0");
  outbound->set_tag("PROXY");

  auto setting = outbound->mutable_settings()->mutable_vmess();

  auto server = setting->add_vnext();

  auto user = server->add_users();
  user->set_id(node.password.toStdString());
  user->set_alterid(p_vmess->alterID().toInt());
  user->set_security(p_vmess->security().toStdString());

  auto stream = outbound->mutable_streamsettings();
  stream->set_network(p_vmess->network().toStdString());
  if (p_vmess->tlsEnable()) {
    stream->set_security("tls");
  } else {
    stream->set_security("none");
  }

  if (stream->network() == "ws") {
    auto websocket = stream->mutable_wssettings();
    websocket->set_path(p_vmess->path().toStdString());

    auto headers = websocket->mutable_headers();
    headers->insert({ "Host", p_vmess->host().toStdString() });
  }

  node.raw = SerializeTools::MessageToJson(*outbound).c_str();

  URLMetaObject meta = {
    .name = node.name.toStdString(),
    .outbound = *outbound,
  };

  node.url = SerializeTools::vmessBase64Encode(meta)->toEncoded();

  return true;
}

bool
NodeFormModel::setRawOutbound(NodeInfo& node)
{
  if (p_raw->rawText().isEmpty())
    return false;

  auto root = Json::parse(p_raw->rawText().toStdString());
  if (!root.contains("protocol"))
    return false;
  else
    node.raw = root.dump().c_str();

  if (auto protocol =
        magic_enum::enum_cast<EntryType>(root["protocol"].get<std::string>());
      protocol.has_value())
    node.protocol = protocol.value();
  else
    node.protocol = EntryType::unknown;

  return true;
}

bool
NodeFormModel::decodeOutboundFromURL(NodeInfo& node)
{
  bool result = false;

  do {
    auto data = p_urlScheme->urlScheme();
    if (data.isEmpty()) {
      break;
    }

    result = SerializeTools::decodeOutboundFromURL(node, data.toStdString());
  } while (false);

  return result;
}

NodeList*
NodeFormModel::list() const
{
  return p_list;
}

void
NodeFormModel::setList(NodeList* newList)
{
  if (newList == nullptr)
    return;

  p_list = newList;

  emit listChanged();
}

ShadowsocksFormModel*
NodeFormModel::shadowsocks() const
{
  return p_shadowsocks;
}

void
NodeFormModel::setShadowsocks(ShadowsocksFormModel* newShadowsocks)
{
  if (p_shadowsocks == newShadowsocks)
    return;
  p_shadowsocks = newShadowsocks;
  emit shadowsocksChanged();
}

VMessFormModel*
NodeFormModel::vmess() const
{
  return p_vmess;
}

void
NodeFormModel::setVmess(VMessFormModel* newVmess)
{
  if (p_vmess == newVmess)
    return;
  p_vmess = newVmess;
  emit vmessChanged();
}

RawOutboundFormModel*
NodeFormModel::raw() const
{
  return p_raw;
}

void
NodeFormModel::setRaw(RawOutboundFormModel* newRaw)
{
  if (p_raw == newRaw)
    return;
  p_raw = newRaw;
  emit rawChanged();
}

URLSchemeFormModel*
NodeFormModel::urlScheme() const
{
  return p_urlScheme;
}

void
NodeFormModel::setURLScheme(URLSchemeFormModel* newUrlScheme)
{
  if (p_urlScheme == newUrlScheme)
    return;
  p_urlScheme = newUrlScheme;
  emit urlSchemeChanged();
}
