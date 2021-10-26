#include "nodeformmodel.h"

using namespace across;

NodeFormModel::NodeFormModel(QObject* parent)
  : QObject(parent)
  , p_list(nullptr)
{}

void
NodeFormModel::accept(const QVariantMap& values)
{
  NodeInfo node;

  if (!values.contains("type"))
    return;

  switch (values.value("type").toInt()) {
    case 0:
      if (values.contains("url"))
        SerializeTools::decodeOutboundFromURL(
          node, values.value("url").toString().toStdString());
      break;
    case 1:
      if (values.contains("manual"))
        manualSetting(node, values.value("manual").toMap());
      break;
    case 2:
      break;
  }

  p_list->appendNode(node);

  return;
}

// bool
// NodeFormModel::setRawOutbound(NodeInfo& node)
//{
//  if (p_raw->rawText().isEmpty())
//    return false;

//  auto root = Json::parse(p_raw->rawText().toStdString());
//  if (!root.contains("protocol"))
//    return false;
//  else
//    node.raw = root.dump().c_str();

//  if (auto protocol =
//        magic_enum::enum_cast<EntryType>(root["protocol"].get<std::string>());
//      protocol.has_value())
//    node.protocol = protocol.value();
//  else
//    node.protocol = EntryType::unknown;

//  return true;
//}

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

bool
NodeFormModel::manualSetting(NodeInfo& node, const QVariantMap& values)
{
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

bool
NodeFormModel::setTrojanOutbound(NodeInfo& node, const QVariantMap& values)
{
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
    tls->set_servername(values.value("serverName").toString().toStdString());

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

  URLMetaObject meta = {
    .name = node.name.toStdString(),
    .outbound = *outbound,
  };

  node.url = SerializeTools::trojanEncode(meta)->toEncoded();

  return true;
}

bool
NodeFormModel::setShadowsocksOutbound(NodeInfo& node, const QVariantMap& values)
{
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

  URLMetaObject meta = {
    .name = node.name.toStdString(),
    .outbound = *outbound,
  };

  node.url = SerializeTools::sip002Encode(meta)->toEncoded();

  return true;
}

bool
NodeFormModel::setVMessOutboud(NodeInfo& node, const QVariantMap& values)
{
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
  if (values.contains("network"))
    stream->set_network(values.value("network").toString().toStdString());

  if (values.contains("enableTLS") && values.value("enableTLS").toBool()) {
    stream->set_security("tls");
  } else {
    stream->set_security("none");
  }

  if (stream->network() == "ws") {
    auto websocket = stream->mutable_wssettings();

    if (values.contains("host")) {
      auto headers = websocket->mutable_headers();
      headers->insert(
        { "Host", values.value("host").toString().toStdString() });
    }

    if (values.contains("path"))
      websocket->set_path(values.value("path").toString().toStdString());
  }

  node.raw = SerializeTools::MessageToJson(*outbound).c_str();

  URLMetaObject meta = {
    .name = node.name.toStdString(),
    .outbound = *outbound,
  };

  node.url = SerializeTools::vmessBase64Encode(meta)->toEncoded();

  return true;
}
