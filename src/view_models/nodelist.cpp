#include "nodelist.h"

using namespace across;
using namespace across::core;
using namespace across::config;
using namespace across::setting;
using namespace across::utils;
using namespace across::network;

NodeList::NodeList(QObject* parent)
  : QObject(parent)
{}

NodeList::~NodeList()
{
  while (!work_tasks.isEmpty())
    work_tasks.dequeue().waitForFinished();
}

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

  connect(
    this, &NodeList::itemLatencyChanged, this, &NodeList::handleLatencyChanged);

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

  if (p_config->enableAutoConnect()) {
    NodeInfo node;
    if (auto id = p_db->getCurrentNodeID(); id) {
      auto nodes = p_db->listAllNodesFromGroupID(p_db->getCurrentGroupID());
      for (auto& nodeItem : nodes) {
        if (id == nodeItem.id) {
          node = nodeItem;
        }
      }
    }
    if (auto id = p_db->getDefaultNodeID(); id) {
      auto nodes = p_db->listAllNodesFromGroupID(p_db->getDefaultGroupID());
      for (auto& nodeItem : nodes) {
        if (id == nodeItem.id) {
          node = nodeItem;
        }
      }
    }
    m_node = node;
    if (!run()) {
      p_logger->error("Failed to start current node: {} {}",
                      node.id,
                      node.name.toStdString());
    }
  }

  reloadItems();
}

bool
NodeList::run()
{
  bool res = false;
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

    p_core->setConfig(QString::fromStdString(config.dump()));
    p_core->run();
    res = true;
  } while (false);

  return res;
}

void
NodeList::setFilter(const QMap<qint64, QList<qint64>>& search_results)
{
  if (search_results.isEmpty())
    return;

  m_search_results = search_results;
  setDisplayGroupID(m_search_results.firstKey());
}

void
NodeList::clearFilter()
{
  m_search_results.clear();

  emit preItemsReset();
  m_nodes = m_origin_nodes;
  emit postItemsReset();
}

void
NodeList::clearItems()
{
  emit preItemsReset();
  m_nodes.clear();
  emit postItemsReset();
}

QList<NodeInfo>
NodeList::items()
{
  return m_nodes;
}

void
NodeList::reloadItems()
{
  emit preItemsReset();
  m_nodes = p_db->listAllNodesFromGroupID(displayGroupID());
  m_origin_nodes = m_nodes;

  if (!m_search_results.isEmpty() &&
      m_search_results.contains(m_display_group_id)) {
    QList<NodeInfo> temp_nodes;
    auto nodes_id = m_search_results.value(m_display_group_id);
    auto iter = m_origin_nodes.begin();
    for (auto& node_id : nodes_id) {
      for (; iter != m_origin_nodes.end(); ++iter) {
        if (iter->id == node_id) {
          temp_nodes.append(*iter);
          ++iter;
          break;
        }
      }
    }

    m_nodes = temp_nodes;
  }
  emit postItemsReset();
}

Json
NodeList::generateConfig()
{
  Json root;
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
    api_rule_object.inbound_tag.emplace_back("ACROSS_API_INBOUND");

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
  file << root;
  file.close();
#endif

  return root;
}

void
NodeList::appendNode(NodeInfo node)
{
  node.group_id = displayGroupID();
  node.group_name = p_db->getGroupFromID(node.group_id).value().name;

  if (auto err = p_db->insert(node); err.type() != QSqlError::NoError) {
    p_logger->error("Failed to add node: {}", node.name.toStdString());
  } else {
    reloadItems();
    emit groupSizeChanged(node.group_id, m_nodes.size());
  }
}

void
NodeList::removeNodeByID(int id)
{
  for (const auto& node : m_nodes) {
    if (id == node.id) {
      auto group_id = node.group_id;
      if (auto result = p_db->removeNodeFromID(id);
          result.type() != QSqlError::NoError) {
        p_logger->error("Failed to remove node: {}", id);
      } else {
        reloadItems();
      }
      emit groupSizeChanged(group_id, m_nodes.size());
      break;
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

Q_INVOKABLE qint64
NodeList::getIndexByNode(qint64 node_id, qint64 group_id)
{
  auto nodes = p_db->listAllNodesFromGroupID(group_id);
  for (qint64 index = 0; index < nodes.size(); index++) {
    if (node_id == nodes.at(index).id) {
      return index;
    }
  }
  return -1;
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
  auto nodes = p_db->listAllNodesFromGroupID(currentGroupID());
  auto iter = std::find_if(nodes.begin(), nodes.end(), [&](NodeInfo& item) {
    return item.id == id;
  });
  if (iter == nodes.end()) {
    p_logger->error("Failed to copy node url: {}", id);
    return;
  }

  auto item = *iter;

  NotifyTools().send(item.url,
                     QString(tr("Copy [%1] URL to clipboard")).arg(item.name));

  ClipboardTools().send(item.url);
}

void
NodeList::handleLatencyChanged(qint64 group_id, int index, NodeInfo node)
{
  p_db->update(node);
  if (group_id == displayGroupID()) {
    if (index < m_nodes.size()) {
      m_nodes[index] = node;
      emit itemReset(index);
    }
  }

  while (!work_tasks.isEmpty() && work_tasks.head().isFinished())
    work_tasks.dequeue();
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
    RuntimeValue(RunTimeValues::DEFAULT_GROUP_ID, displayGroupID()));
}

Q_INVOKABLE void
NodeList::testLatency(int id)
{
  auto iter = std::find_if(m_nodes.begin(), m_nodes.end(), [&](NodeInfo& item) {
    return item.id == id;
  });
  if (iter == m_nodes.end()) {
    p_logger->error("Failed to load node info: {}", id);
    return;
  }
  int index = iter - m_nodes.begin();

  if (auto& node = *iter; !node.address.isEmpty()) {
    testLatency(node, index);
  }
}

void
NodeList::testLatency(NodeInfo node, int index)
{
  work_tasks.enqueue(QtConcurrent::run([this, index, node] {
    auto current_node = node;
    TCPPing ping;
    ping.setAddr(node.address);
    ping.setPort(node.port);
    current_node.latency = ping.getAvgLatency();
    emit itemLatencyChanged(node.group_id, index, current_node);
  }));
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
