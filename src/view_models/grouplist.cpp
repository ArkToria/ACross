#include "grouplist.h"

using namespace across;
using namespace across::utils;
using namespace across::config;
using namespace across::setting;

GroupList::GroupList(QObject* parent) {}

void
GroupList::init(std::shared_ptr<spdlog::details::thread_pool> thread_pool,
                DBTools& db,
                network::CURLTools& curl_tools,
                NodeList& node_list,
                ConfigTools& config)
{
  p_logger = std::make_shared<LogTools>(thread_pool, "group_list");

  p_thread_pool = thread_pool;

  p_db = &db;
  if (p_db == nullptr) {
    p_logger->error("Failed to get the database");
    return;
  }

  p_curl = &curl_tools;
  if (p_curl == nullptr) {
    p_logger->warn("Failed to get the downloader");
  }

  p_nodes = &node_list;
  if (p_nodes == nullptr) {
    p_logger->error("Failed to get the nodes");
  }

  p_config = &config;

  reloadItems();
}

QVector<GroupInfo>
GroupList::items() const
{
  return m_items;
}

void
GroupList::reloadItems(bool reopen_db)
{
  m_items.clear();

  if (reopen_db) {
    p_db->reload();
  }

  auto temp_items = p_db->listAllGroupsInfo();

  for (auto& item : temp_items) {
    m_items.append(item);
  }

  if (m_items.isEmpty()) {
    p_logger->warn("No group items from database");
  } else {
    p_logger->info("Load group items from database");
  }

  p_nodes->reloadItems();
}

bool
GroupList::insertSIP008(const std::stringstream& data_stream,
                        const network::CURLTools::DownloadTask& task)
{
  auto result = SerializeTools::sip008Parser(data_stream);
  if (!result.has_value()) {
    p_logger->error("Failed to parse download subscription");
    return false;
  }

  for (auto& server : result.value().servers) {
    ShadowsocksObject::OutboundSettingObject shadows_object;
    shadows_object.fromSIP008Server(server);

    OutboundObject outbound_object;
    outbound_object.appendShadowsocksObject(shadows_object);

    if (p_db != nullptr) {
      NodeInfo node{ 0,
                     QString::fromStdString(server.remarks),
                     QString::fromStdString(task.name),
                     task.group_id,
                     across::EntryType::shadowsocks,
                     QString::fromStdString(server.server),
                     server.server_port,
                     QString::fromStdString(server.password),
                     QString::fromStdString(
                       outbound_object.toObject().toStyledString()) };

      p_db->insert(node);
    }
  }

  reloadItems();
  return true;
}

bool
GroupList::insertBase64(const std::stringstream& data_stream,
                        const network::CURLTools::DownloadTask& task)
{
  bool result = false;

  if (p_db == nullptr) {
    return result;
  }

  QString decode_data = QByteArray::fromBase64(data_stream.str().c_str());

  for (auto& item : decode_data.split("\n")) {
    if (item.isEmpty()) {
      break;
    }

    NodeInfo node;
    node.group = QString::fromStdString(task.name);
    node.group_id = task.group_id;

    result = SerializeTools::decodeOutboundFromURL(node, item);

    if (!result) {
      break;
    } else {
      p_db->insert(node);
    }
  }

  return result;
}

void
GroupList::appendItem(const QString& group_name,
                      const QString& url,
                      int type,
                      int cycle_time)
{
  emit preItemAppended();

  GroupInfo item;
  item.name = group_name.toLatin1();
  item.isSubscription = true;
  item.url = url;
  item.type = magic_enum::enum_value<SubscriptionType>(type);
  item.cycle_time = cycle_time;

  auto result = p_db->insert(item);
  if (result == SQLITE_OK) {
    item.id = p_db->getLastID();

    std::stringstream data_stream;
    across::network::CURLTools::DownloadTask task(
      item.name.toStdString(),
      item.url.toStdString(),
      item.id,
      p_config->networkUserAgent().toStdString());

    connect(p_curl, &across::network::CURLTools::downloadFinished, [&]() {
      auto result = p_db->createNodesTable(item.name.toStdString());

      // TODO: rewrite parse checker
      if (result == SQLITE_OK) {
        bool rc = false;
        switch (item.type) {
          case sip008:
            rc = this->insertSIP008(data_stream, task);
            break;
          case base64:
            rc = this->insertBase64(data_stream, task);
          default:
            break;
        }

        reloadItems();

        // failed to parse
        if (!rc) {
          emit preLastItemRemoved();
          p_db->removeGroupFromName(task.name);
          emit postLastItemRemoved();
        }

        emit postItemAppended();
      }
    });

    p_curl->download(task, &data_stream);
  }
}

void
GroupList::appendItem(const QString& group_name, const QString& node_items)
{
  emit preItemAppended();

  GroupInfo group;
  group.id = m_items.size() + 1;
  group.name = group_name;
  group.isSubscription = false;

  int result = 0;

  do {
    result = p_db->insert(group);
    if (result != SQLITE_OK) {
      break;
    }

    group.id = p_db->getLastID();

    if (p_db->createNodesTable(group.name.toStdString()) != SQLITE_OK) {
      break;
    }

    for (auto& item : node_items.split("\n")) {
      if (item.isEmpty()) {
        break;
      }

      NodeInfo node;
      node.group = group.name;
      node.group_id = group.id;

      result = SerializeTools::decodeOutboundFromURL(node, item);

      if (!result) {
        emit preLastItemRemoved();
        p_db->removeGroupFromName(group.name.toStdString());
        emit postLastItemRemoved();
        break;
      } else {
        p_db->insert(node);
        reloadItems();
      }
    }
  } while (false);

  emit postItemAppended();
}

void
GroupList::removeItem(int index)
{
  if (m_items.size() > index) {
    setDisplayGroupID(m_items.at(index - 1).id);

    emit preItemRemoved(index);

    p_db->removeGroupFromName(m_items.at(index).name.toStdString());

    m_items.removeAt(index);

    emit postItemRemoved();
  }
}

void
GroupList::upgradeItem(int index)
{
  auto item = m_items.at(index);
  if (item.isSubscription) {
    std::stringstream data_stream;

    across::network::CURLTools::DownloadTask task(
      item.name.toStdString(), item.url.toStdString(), item.id);

    connect(p_curl, &across::network::CURLTools::downloadFinished, [&]() {

    });
  }
}

void
GroupList::setDisplayGroupID(int id)
{
  p_nodes->setDisplayGroupID(id);
}

void
GroupList::copyUrlToClipboard(int index)
{
  auto item = m_items.at(index);

  NotifyTools().send(item.url,
                     QString(tr("Copy [%1] URL to clipboard")).arg(item.name));

  ClipboardTools().send(item.url);
}
