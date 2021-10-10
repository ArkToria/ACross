#include "nodelist.h"

using namespace across;
using namespace across::core;
using namespace across::config;
using namespace across::setting;
using namespace across::utils;

NodeList::NodeList(QObject* parent)
  : QObject(parent)
{}

void
NodeList::init(QSharedPointer<LogView> log_view,
               QSharedPointer<across::setting::ConfigTools> config,
               QSharedPointer<CoreTools> core,
               QSharedPointer<DBTools> db)
{
  p_logger = std::make_shared<LogTools>(log_view, "node_list");

  p_json = std::make_shared<JsonTools>();

  p_db = db;

  p_config = config;

  p_core = core;

  connect(p_config.get(), &ConfigTools::apiEnableChanged, this, [&]() {
    if (!p_config->apiEnable() && p_api != nullptr)
      p_api->stopMonitoring();
    else {
      if (p_core->isRunning() && p_api != nullptr) {
        p_api->restartMonitoring();
        m_traffic_last.clear();
        m_traffic_sum.clear();
      }
    }
  });

  connect(p_config.get(), &ConfigTools::apiPortChanged, this, [&]() {
    if (p_api != nullptr) {
      p_api->stopMonitoring();
      p_api.reset(new APITools(p_config->apiPort().toUInt()));
      m_traffic_last.clear();
      m_traffic_sum.clear();
    }
  });

  connect(p_core.get(), &CoreTools::isRunningChanged, this, [&]() {
    if (p_api != nullptr) {
      if (p_core->isRunning()) {
        p_api->startMonitoring("PROXY");
      } else {
        p_api->stopMonitoring();
        m_traffic_last.clear();
        m_traffic_sum.clear();
      }
    }
  });

  if (p_config->apiEnable()) {
    p_api =
      QSharedPointer<APITools>(new APITools(p_config->apiPort().toUInt()));

    connect(p_api.get(),
            &APITools::trafficChanged,
            this,
            [this](const QVariant& data) {
              auto traffic = data.value<TrafficInfo>();

              m_traffic_sum.download += traffic.download -
                                        m_traffic_last.download -
                                        m_traffic_last_rate.download;

              m_traffic_sum.upload += traffic.upload - m_traffic_last.upload -
                                      m_traffic_last_rate.upload;

              setDownloadTraffic(std::max(m_traffic_sum.download, 0LL));
              setUploadTraffic(std::max(m_traffic_sum.upload, 0LL));

              m_traffic_last_rate = {
                .upload = traffic.upload - m_traffic_last.upload,
                .download = traffic.download - m_traffic_last.download,
              };

              m_traffic_last = traffic;
            });
  }

  reloadItems();
}

bool
NodeList::run()
{
  do {
    if (m_node.raw.isEmpty()) {
      p_logger->error("Failed to load current node");
      break;
    }

    if (p_config == nullptr) {
      p_logger->error("Failed to load config");
      break;
    }

    if (p_json == nullptr) {
      p_logger->error("Failed to load json tools");
    };

    if (p_core == nullptr) {
      p_logger->error("Failed to load core tools");
    };

    auto config = generateConfig();

    p_core->setConfig(QString::fromStdString(config.toStyledString()));
    p_core->run();
  } while (false);

  return false;
}

QVector<NodeInfo>
NodeList::items()
{
  return m_nodes;
}

void
NodeList::reloadItems()
{
  emit preItemsReset();

  m_nodes = p_db->listAllNodesFromGroupID(displayGroupID());

  emit postItemsReset();
}

Json::Value
NodeList::generateConfig()
{
  Json::Value root;
  {
    LogObject log_object;
    if (auto level = magic_enum::enum_cast<LogObject::LogLevel>(
          p_config->logLevel().toStdString())) {
      log_object.setLogLevel(level.value());
      log_object.setObject(root);
    }
  }

  {
    APIObject api_object;
    api_object.setObject(root);
  }

  {
    RuleObject api_rule_object = {
      .type = "field",
      .outbound_tag = "ACROSS_API",
    };
    api_rule_object.inbound_tag.append("ACROSS_API_INBOUND");

    RoutingObject routing_object;
    routing_object.appendRuleObject(api_rule_object);
    routing_object.setObject(root);

    Stats().setObject(root);

    SystemPolicyObject system_policy_object;
    PolicyObject policy_object;
    policy_object.setSystemPolicyObject(system_policy_object);
    policy_object.setObject(root);
  }

  {
    p_config->setInboundObject(root);
  }

  {
    p_json->setData(m_node.raw.toStdString());

    OutboundObjects outbound_objects;
    outbound_objects.appendOutboundObject(p_json->getRoot());
    outbound_objects.setObject(root);
  }

#ifdef QT_DEBUG
  std::ofstream file("generation_test.json");
  Json::StreamWriterBuilder builder;
  std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
  writer->write(root, &file);
  file.close();
#endif

  return root;
}

void
NodeList::appendNode(NodeInfo node)
{
  node.group_id = displayGroupID();
  node.group_name = p_db->getGroupNameFromGroupID(node.group_id);

  if (auto err = p_db->insert(node); err.type() != QSqlError::NoError) {
    p_logger->error("Failed to add node: {}", node.name.toStdString());
  } else {
    reloadItems();
    emit itemsSizeChanged(node.group_id, m_nodes.size());
  }
}

void
NodeList::removeNodeByID(int id)
{
  if (auto result = p_db->removeNodeFromID(id);
      result.type() != QSqlError::NoError) {
    p_logger->error("Failed to remove node: {}", id);
  } else {
    reloadItems();

    for (auto& node : m_nodes) {
      if (id == node.id) {
        emit itemsSizeChanged(node.group_id, m_nodes.size());
        break;
      }
    }
  }
}

QString
NodeList::getQRCode(int id)
{
  for (auto& item : m_nodes) {
    if (item.id == id) {
      emit updateQRCode(item.name, item.url);
      return item.name;
    }
  }

  return "";
}

qint64
NodeList::currentNodeID()
{
  return m_node.id;
}

qint64
NodeList::currentGroupID()
{
  return m_node.group_id;
}

qint64
NodeList::displayGroupID()
{
  return m_display_group_id;
}

QString
NodeList::currentNodeName()
{
  return m_node.name;
}

QString
NodeList::currentNodeGroup()
{
  return m_node.group_name;
}

QString
NodeList::currentNodeProtocol()
{
  return magic_enum::enum_name(m_node.protocol).data();
}

QString
NodeList::currentNodeAddress()
{
  return m_node.address;
}

int
NodeList::currentNodePort()
{
  return m_node.port;
}

QString
NodeList::currentNodePassword()
{
  return m_node.password;
}

QString
NodeList::currentNodeURL()
{
  return m_node.url;
}

void
NodeList::setDisplayGroupID(int group_id)
{
  if (group_id <= 0 || group_id == m_display_group_id) {
    return;
  }
  m_display_group_id = group_id;

  reloadItems();
  emit displayGroupIDChanged();
}

void
NodeList::setCurrentNodeByID(int id)
{
  for (auto& node : m_nodes) {
    if (id == node.id) {
      m_node = node;

      p_db->updateRuntimeValue(
        RuntimeValue(RunTimeValues::CURRENT_NODE_ID, node.id));
      p_db->updateRuntimeValue(
        RuntimeValue(RunTimeValues::CURRENT_GROUP_ID, node.group_id));

      emit currentNodeIDChanged();
      emit currentNodeNameChanged();
      emit currentNodeGroupChanged();
      emit currentNodeProtocolChanged();
      emit currentNodeAddressChanged();
      emit currentNodePortChanged();
      emit currentNodePasswordChanged();
      emit currentNodeURLChanged();
      emit currentGroupIDChanged();

      if (!run()) {
        p_logger->error("Failed to start current node: {} {}",
                        node.id,
                        node.name.toStdString());
      }
    }
  }
}

void
NodeList::copyUrlToClipboard(int id)
{
  auto iter = std::find_if(m_nodes.begin(), m_nodes.end(), [&](NodeInfo& item) {
    return item.id == id;
  });
  if (iter == m_nodes.end()) {
    p_logger->error("Failed to copy node url: {}", id);
    return;
  }

  auto item = *iter;

  NotifyTools().send(item.url,
                     QString(tr("Copy [%1] URL to clipboard")).arg(item.name));

  ClipboardTools().send(item.url);
}

void
NodeList::saveQRCodeToFile(int id, const QUrl& url)
{
  auto iter = std::find_if(m_nodes.begin(), m_nodes.end(), [&](NodeInfo& item) {
    return item.id == id;
  });
  if (iter == m_nodes.end()) {
    p_logger->error("Failed to load node info: {}", id);
    return;
  }

  if (auto filename = url.fileName(); !filename.isEmpty()) {
    if (filename.contains("*")) {
      filename = iter->name;
    }

    if (!filename.contains(".png")) {
      filename.append(".png");
    }

    auto path = QUrl(url.toString(QUrl::RemoveFilename)).toLocalFile();
    auto file_path = path.append(filename);
    auto img = QRCodeTools().write(iter->url);

    if (!img.save(file_path) || img.isNull()) {
      p_logger->error("Failed to save image: {}", file_path.toStdString());
    }
  }
}

void
NodeList::setAsDefault(int id)
{
  p_db->updateRuntimeValue(RuntimeValue(RunTimeValues::DEFAULT_NODE_ID, id));
  p_db->updateRuntimeValue(
    RuntimeValue(RunTimeValues::DEFAULT_NODE_ID, displayGroupID()));
}
void
NodeList::setAvgLatency(int id)
{
  auto iter = std::find_if(m_nodes.begin(), m_nodes.end(), [&](NodeInfo& item) {
    return item.id == id;
  });
  if (iter == m_nodes.end()) {
    p_logger->error("Failed to load node info: {}", id);
    return;
  }
  QFuture<void> setFuture = QtConcurrent::run(&NodeList::setLatency,
                                              this,
                                              iter->id,
                                              iter->group_name,
                                              iter->address,
                                              iter->port);
}

void
NodeList::setLatency(qint64 id,
                     const QString& group_name,
                     const QString& addr,
                     unsigned int port)
{
  across::network::TCPPing tcpPingTool(addr, port);

  int latency = tcpPingTool.getAvgLatency();

  emit nodeLatencyChanged(id, group_name, latency);
}

QString
NodeList::uploadTraffic()
{
  return APITools::unitConvert(m_traffic.upload);
}

void
NodeList::setUploadTraffic(double newUploadTraffic)
{
  if (m_traffic.upload == newUploadTraffic)
    return;
  m_traffic.upload = newUploadTraffic;
  emit uploadTrafficChanged(APITools::unitConvert(m_traffic.upload));
}

QString
NodeList::downloadTraffic()
{
  return APITools::unitConvert(m_traffic.download);
}

void
NodeList::setDownloadTraffic(double newDownloadTraffic)
{
  if (m_traffic.download == newDownloadTraffic)
    return;
  m_traffic.download = newDownloadTraffic;
  emit downloadTrafficChanged(APITools::unitConvert(m_traffic.download));
}
