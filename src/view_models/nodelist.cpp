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
               QSharedPointer<DBTools> db,
               QSharedPointer<ImageProvider> qrcode)
{
  p_logger = std::make_shared<LogTools>(log_view, "node_list");

  p_json = std::make_shared<JsonTools>();

  p_db = db;

  p_config = config;

  p_core = core;

  p_qrcode = qrcode;

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

  for (auto& item : p_db->listAllNodes()) {
    m_all_items.insert(item.first, item.second);
  }

  if (!m_all_items.isEmpty())
    for (auto& item : m_all_items.first().nodes) {
      m_items.append(item);
    }

  emit postItemsReset();
}

void
NodeList::appendNode(NodeInfo node)
{
  node.group_id = currentGroupID();

  auto groups = p_db->listAllGroupsInfo();

  for (auto& item : groups) {
    if (item.id == node.group_id) {
      node.group = item.name;
      break;
    }
  }

  p_db->insert(node);

  reloadItems();
}

void
NodeList::removeCurrentNode(int id)
{
    for (auto& item : m_items) {
      if (item.id == id) {
        auto result = p_db->removeItemFromID(item.group, item.id);
        if (result == SQLITE_OK) {
          reloadItems();
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
      p_qrcode->setContent(item.name, item.url);
      return item.name;
    }
  }

  return "";
}

int
NodeList::currentNodeID()
{
  auto temp_items = m_all_items.find(m_group_id).value();

  return temp_items.current_id;
}

int
NodeList::currentNodeIndex()
{
  auto temp_items = m_all_items.find(m_group_id).value();

  return temp_items.current_index;
}

int
NodeList::currentGroupID()
{
  return m_group_id;
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
NodeList::currentNodeURL() const
{
  return m_current_node.url;
}

void
NodeList::setCurrentGroupID(int group_id)
{
  if (m_group_id == group_id) {
    return;
  }

  m_group_id = group_id;

  emit currentGroupIDChanged();
}

void
NodeList::setDisplayGroupID(int group_id)
{
  do {
    if (group_id <= 0 || group_id == m_display_group_id) {
      break;
    }

    m_display_group_id = group_id;

    emit preItemsReset();

    m_items.clear();
    for (auto& item : m_all_items.find(m_display_group_id).value().nodes) {
      m_items.append(item);
    }

    emit postItemsReset();

    emit displayGroupIDChanged();

  } while (false);
}

void
NodeList::setCurrentNode(int id, int index)
{
  do {
    auto items = m_all_items.find(m_display_group_id);
    if (items != m_all_items.end()) {
      items->current_id = id;
      items->current_index = index;

      emit currentNodeIDChanged();
      emit currentNodeIndexChanged();
    } else {
      p_logger->error("Failed to get group: {}", m_group_id);
      break;
    }

    auto iter = std::find_if(
      items->nodes.begin(), items->nodes.end(), [&](NodeInfo item) {
        if (item.id == id) {
          return true;
        } else {
          return false;
        }
      });

    if (iter != items->nodes.end()) {
      m_current_node = *iter;

      setCurrentGroupID(m_current_node.group_id);

      emit currentNodeNameChanged();
      emit currentNodeGroupChanged();
      emit currentNodeProtocolChanged();
      emit currentNodeAddressChanged();
      emit currentNodePortChanged();
    } else {
      break;
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
      InboundSettings inbound_settings = p_config->getInboundConfig();
      inbound_settings.setObject(root);
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
