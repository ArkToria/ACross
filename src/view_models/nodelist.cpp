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

              setDownloadTraffic(std::max(m_traffic_sum.download, 0L));
              setUploadTraffic(std::max(m_traffic_sum.upload, 0L));

              m_traffic_last_rate = {
                .upload = traffic.upload - m_traffic_last.upload,
                .download = traffic.download - m_traffic_last.download,
              };

              m_traffic_last = traffic;
            });
  }

  reloadItems();
}

QVector<NodeInfo>
NodeList::items()
{
  return m_items;
}

void
NodeList::reloadItems()
{
  emit preItemsReset();

  m_items.clear();
  m_all_items.clear();

  m_all_items = p_db->listAllNodes();

  if (auto iter = m_all_items.find(this->displayGroupID());
      iter != m_all_items.end()) {
    m_items = *iter;
  }

  auto current_node_id = p_db->getCurrentNodeID();
  if (auto iter = m_all_items.find(current_node_id);
      iter != m_all_items.end()) {
    for (auto& item : *iter) {
      if (item.id == current_node_id) {
        m_current_node = item;
      }
    }
  }

  emit postItemsReset();
}

void
NodeList::appendNode(NodeInfo node)
{
  node.group_id = displayGroupID();

  auto groups = p_db->getAllGroupsInfo();

  // get display group name
  for (auto& item : groups) {
    if (item.id == node.group_id) {
      node.group = item.name;
      break;
    }
  }

  if (auto err = p_db->insert(node); err.type() != QSqlError::NoError) {
    p_logger->error("Failed to add node: {}", node.name.toStdString());
  } else {
    reloadItems();

    emit itemsSizeChanged(node.group_id, m_items.size());
  }
}

void
NodeList::removeNodeByID(int id)
{
  for (auto& item : m_items) {
    if (item.id == id) {
      auto result = p_db->removeItemFromID(item.group, item.id);
      if (result.type() == QSqlError::NoError) {

        reloadItems();
        emit itemsSizeChanged(item.group_id, m_items.size());
      }
      break;
    }
  }
}

QString
NodeList::getQRCode(int id)
{
  for (auto& item : m_items) {
    if (item.id == id) {
      emit updateQRCode(item.name, item.url);
      return item.name;
    }
  }

  return "";
}

int
NodeList::currentNodeID()
{
  return m_current_node.id;
}

int
NodeList::currentGroupID()
{
  return m_current_node.group_id;
}

int
NodeList::displayGroupID()
{
  return m_display_group_id;
}

const QString&
NodeList::currentNodeName() const
{
  return m_current_node.name;
}

const QString&
NodeList::currentNodeGroup() const
{
  return m_current_node.group;
}

QString
NodeList::currentNodeProtocol() const
{
  return magic_enum::enum_name(m_current_node.protocol).data();
}

const QString&
NodeList::currentNodeAddress() const
{
  return m_current_node.address;
}

int
NodeList::currentNodePort()
{
  return m_current_node.port;
}

const QString&
NodeList::currentNodePassword() const
{
  return m_current_node.password;
}

const QString&
NodeList::currentNodeURL() const
{
  return m_current_node.url;
}

void
NodeList::setDisplayGroupID(int group_id)
{
    if (group_id <= 0 || group_id == m_display_group_id) {
      return;
    }

    m_display_group_id = group_id;

    if (auto iter = m_all_items.find(m_display_group_id);
        iter != m_all_items.end()) {
      emit preItemsReset();

      m_items.clear();
      m_items = *iter;

      emit postItemsReset();
      emit displayGroupIDChanged();
    }
}

void
NodeList::setCurrentNodeByID(int id)
{
  do {
    if (auto iter = m_all_items.find(m_display_group_id);
        iter != m_all_items.end()) {
      for (auto& item : *iter) {
        if (item.id == id) {
          m_current_node = item;

          p_db->updateRuntimeValue("CURRENT_NODE_ID",
                                   QString::number(m_current_node.id));
          p_db->updateRuntimeValue("CURRENT_GROUP_ID",
                                   QString::number(m_current_node.group_id));

          emit currentNodeIDChanged();
          emit currentNodeNameChanged();
          emit currentNodeGroupChanged();
          emit currentNodeProtocolChanged();
          emit currentNodeAddressChanged();
          emit currentNodePortChanged();
          emit currentNodePasswordChanged();
          emit currentNodeURLChanged();
          emit currentGroupIDChanged();
          break;
        }
      }
    } else {
      p_logger->error("Failed to load display group: {}", m_display_group_id);
    }

    if (p_config == nullptr) {
      p_logger->error("Failed to load config");
      break;
    }

    if (p_json == nullptr) {
      p_logger->error("Failed to load json tools");
    };

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
      p_json->setData(m_current_node.raw.toStdString());

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
    p_core->setConfig(QString::fromStdString(root.toStyledString()));
    p_core->run();

  } while (false);
}

void
NodeList::copyUrlToClipboard(int id)
{
  auto iter = std::find_if(m_items.begin(), m_items.end(), [&](NodeInfo& item) {
    return item.id == id;
  });
  if (iter == m_items.end()) {
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
  auto iter = std::find_if(m_items.begin(), m_items.end(), [&](NodeInfo& item) {
    return item.id == id;
  });
  if (iter == m_items.end()) {
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
  p_db->updateRuntimeValue("DEFAULT_NODE_ID", QString::number(id));
  p_db->updateRuntimeValue("DEFAULT_GROUP_ID",
                           QString::number(displayGroupID()));
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
