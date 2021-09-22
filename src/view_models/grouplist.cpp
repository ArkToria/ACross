#include "grouplist.h"

using namespace across;
using namespace across::utils;
using namespace across::config;
using namespace across::setting;
using namespace across::network;

GroupList::GroupList(QObject* parent) {}

void
GroupList::init(QSharedPointer<LogView> log_view,
                QSharedPointer<across::setting::ConfigTools> config,
                QSharedPointer<across::DBTools> db,
                QSharedPointer<across::NodeList> nodes,
                QSharedPointer<across::network::CURLTools> curl)
{
  p_config = config;

  p_logger = std::make_shared<LogTools>(log_view, "group_list");

  p_db = db;
  if (p_db == nullptr) {
    p_logger->error("Failed to get the database");
    return;
  }

  p_curl = curl;
  if (p_curl == nullptr) {
    p_logger->warn("Failed to get the downloader");
  }

  p_nodes = nodes;
  if (p_nodes == nullptr) {
    p_logger->error("Failed to get the nodes");
  }

  reloadItems();
}

bool
GroupList::insert(GroupInfo& group_info, const QString& content)
{
  emit preItemAppended();

  bool err = false;
  do {
    if (auto err = p_db->insert(group_info); err != SQLITE_OK) {
      break;
    }

    if (auto err = p_db->createNodesTable(group_info.name); err != SQLITE_OK) {
      p_logger->error("Failed to create table: {}",
                      group_info.name.toStdString());
      break;
    }

    bool result = false;
    switch (group_info.type) {
      case sip008:
        result = this->insertSIP008(group_info, content);
        break;
      case base64:
        result = this->insertBase64(group_info, content);
        break;
      default:
        break;
    }

    if (!result) {
      emit preLastItemRemoved();
      p_db->removeGroupFromName(group_info.name);
      emit postLastItemRemoved();
      break;
    }

    err = true;
  } while (false);

  reloadItems();
  emit postItemAppended();

  return err;
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
GroupList::insertSIP008(const GroupInfo& group_info, const QString& content)
{
  auto result = SerializeTools::sip008Parser(content.toStdString());
  if (!result.has_value()) {
    p_logger->error("Failed to parse download subscription");
    return false;
  }

  for (auto& server : result.value().servers) {
    auto sip002 = SerializeTools::sip002Encode(server).value().toString();

    ShadowsocksObject::OutboundSettingObject shadows_object;
    shadows_object.fromSIP008Server(server);

    OutboundObject outbound_object;
    outbound_object.appendShadowsocksObject(shadows_object);

    if (p_db != nullptr) {
      NodeInfo node = {
        .id = 0,
        .name = QString::fromStdString(server.remarks),
        .group = group_info.name,
        .group_id = group_info.id,
        .protocol = across::EntryType::shadowsocks,
        .address = QString::fromStdString(server.server),
        .port = server.server_port,
        .password = QString::fromStdString(server.password),
        .raw =
          QString::fromStdString(outbound_object.toObject().toStyledString()),
        .url = QString(QUrl(sip002).toEncoded()),
      };

      p_db->insert(node);
    }
  }

  reloadItems();
  return true;
}

bool
GroupList::insertBase64(const GroupInfo& group_info, const QString& content)
{
  bool result = false;

  if (p_db == nullptr) {
    return result;
  }

  QString decode_data = QByteArray::fromBase64(content.toUtf8());

  for (auto& item : decode_data.split("\n")) {
    item.remove("\r");
    if (item.isEmpty()) {
      break;
    }

    NodeInfo node = { .group = group_info.name, .group_id = group_info.id };

    result = SerializeTools::decodeOutboundFromURL(node, item);

    if (!result) {
      break;
    } else {
      p_db->insert(node);
    }
  }

  reloadItems();
  return result;
}

void
GroupList::appendItem(const QString& group_name,
                      const QString& url,
                      int type,
                      int cycle_time)
{
  DownloadTask task = { .filename = group_name,
                        .url = url,
                        .user_agent = p_config->networkUserAgent() };

  GroupInfo group_info = { .name = group_name,
                           .isSubscription = true,
                           .type =
                             magic_enum::enum_value<SubscriptionType>(type),
                           .url = url,
                           .cycle_time = cycle_time };

  m_pre_items.append(group_info);

  connect(p_curl.get(),
          &across::network::CURLTools::downloadFinished,
          this,
          &GroupList::handleDownloaded);

  p_curl->download(task);
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

    if (p_db->createNodesTable(group.name) != SQLITE_OK) {
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
        p_db->removeGroupFromName(group.name);
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

    p_db->removeGroupFromName(m_items.at(index).name);

    m_items.removeAt(index);

    emit postItemRemoved();
  }
}

void
GroupList::upgradeItem(int index)
{
  auto item = m_items.at(index);

  if (item.isSubscription) {
    //    auto data_stream = std::make_shared<std::stringstream>();

    //    connect(p_curl.get(), &across::network::CURLTools::downloadFinished,
    //    [&]() {
    //      auto result = p_db->createNodesTable(item.name.toStdString());
    //    });
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

void
GroupList::handleDownloaded(const QVariant& content)
{
  auto task = content.value<DownloadTask>();

  for (auto i = m_pre_items.size() - 1; i >= 0; --i) {
    auto item = m_pre_items.at(i);
    if (task.filename == item.name) {
      if (task.content.isEmpty()) {
        break;
      } else {
        insert(item, task.content);
      }
      m_pre_items.removeAt(i);
    }
  }
}
