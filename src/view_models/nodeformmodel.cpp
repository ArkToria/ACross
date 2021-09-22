#include "nodeformmodel.h"

using namespace across;
using namespace across::config;
using namespace across::utils;

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

  TrojanObject::OutboundSettingObject outbound_setting;
  outbound_setting.address = node.address.toStdString();
  outbound_setting.port = node.port;
  outbound_setting.password = node.password.toStdString();

  TransportObject::TLSObject tls;
  tls.server_name = p_trojan->serverName().toStdString();
  if (p_trojan->alpn() == "h2+http/1.1") {
    tls.appendAlpn("h2");
    tls.appendAlpn("http/1.1");
  } else {
    tls.appendAlpn(p_trojan->alpn().toStdString());
  }

  TransportObject::OutboundStreamObject outbound_stream;
  outbound_stream.network = magic_enum::enum_cast<TransportObject::Network>(
                              p_trojan->network().toStdString())
                              .value();
  outbound_stream.security = p_trojan->security().toStdString();
  outbound_stream.setTLSSetting(tls);

  OutboundObject outbound_object;
  outbound_object.appendTrojanObject(outbound_setting);
  outbound_object.setTransportStreamObject(outbound_stream);

  node.raw =
    QString::fromStdString(outbound_object.toObject().toStyledString());

  URLMetaObject meta = {
    .name = node.name.toStdString(),
    .address = node.address.toStdString(),
    .port = node.port,
    .password = node.password.toStdString(),
    .outbound_object = outbound_object,
  };

  node.url = SerializeTools::trojanEncode(meta)->toString(QUrl::FullyEncoded);

  return true;
}

bool
NodeFormModel::setShadowsocksOutbound(NodeInfo& node)
{
  if (p_shadowsocks == nullptr) {
    return false;
  }

  ShadowsocksObject::OutboundSettingObject outbound_setting;
  outbound_setting.address = node.address.toStdString();
  outbound_setting.port = node.port;
  outbound_setting.password = node.password.toStdString();
  outbound_setting.setMethod(p_shadowsocks->security().toStdString());
  outbound_setting.iv_check = p_shadowsocks->ivCheck();

  OutboundObject outbound_object;
  outbound_object.appendShadowsocksObject(outbound_setting);

  node.raw =
    QString::fromStdString(outbound_object.toObject().toStyledString());

  SIP008::Server server = {
    .remarks = node.name.toStdString(),
    .server = node.address.toStdString(),
    .server_port = node.port,
    .password = node.password.toStdString(),
    .method = p_shadowsocks->security().toStdString(),
  };

  node.url = SerializeTools::sip002Encode(server)->toString(QUrl::FullyEncoded);

  return true;
}

bool
NodeFormModel::setVMessOutboud(NodeInfo& node)
{
  if (p_vmess == nullptr) {
    return false;
  }

  VMessObject::UserObject user_object;
  user_object.id = node.password.toStdString();
  user_object.alter_id = p_vmess->alterID().toInt();
  user_object.setMethod(p_vmess->security().toStdString());

  VMessObject::OutboundSettingObject outbound_setting;
  outbound_setting.address = node.address.toStdString();
  outbound_setting.port = node.port;
  outbound_setting.appendUserObject(user_object);

  TransportObject::OutboundStreamObject outbound_stream;
  outbound_stream.network = magic_enum::enum_cast<TransportObject::Network>(
                              p_vmess->network().toStdString())
                              .value();
  if (p_vmess->tlsEnable()) {
    outbound_stream.security = "tls"; // default value
  } else {
    outbound_stream.security = "none"; // default value
  }

  TransportObject::WebSocketObject websocket_object;

  switch (outbound_stream.network) {
    case TransportObject::Network::ws:
      websocket_object.setHost(p_vmess->host().toStdString());
      websocket_object.path = p_vmess->path().toStdString();
      outbound_stream.setWebsocketSetting(websocket_object);
      break;
    default:
      break;
  }

  OutboundObject outbound_object;
  outbound_object.appendVMessObject(outbound_setting);
  outbound_object.setTransportStreamObject(outbound_stream);

  node.raw =
    QString::fromStdString(outbound_object.toObject().toStyledString());

  return true;
}

bool
NodeFormModel::setRawOutbound(NodeInfo& node)
{
  if (p_raw->rawText().isEmpty()) {
    return false;
  }

  JsonTools json_tools;
  json_tools.setData(p_raw->rawText().toStdString());
  auto root = json_tools.getRoot();

  auto protocol = magic_enum::enum_cast<EntryType>(root["protocol"].asString());
  if (protocol.has_value()) {
    node.protocol = protocol.value();
  } else {
    node.protocol = EntryType::unknown;
  }

  node.raw = p_raw->rawText();

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

    result = SerializeTools::decodeOutboundFromURL(node, data);
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
