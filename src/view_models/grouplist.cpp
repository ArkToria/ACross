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

  connect(p_nodes.get(),
          &NodeList::itemsSizeChanged,
          this,
          &GroupList::handleItemsChanged);

  reloadItems();

  // TEST: Auto checking for updates
  checkAllUpdate();
}

bool
GroupList::insert(GroupInfo& group_info, const QString& content)
{
  emit preItemAppended();

  bool insert_result = false;
  do {
    if (auto err = p_db->insert(group_info); err.type() != QSqlError::NoError) {
      p_logger->error("Failed to insert group: {}",
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
      p_db->removeGroupFromID(group_info.id);
      emit postLastItemRemoved();
      break;
    }

    insert_result = true;
  } while (false);

  reloadItems();
  emit postItemAppended();

  return insert_result;
}

QList<GroupInfo>
GroupList::items() const
{
  return m_groups;
}

void
GroupList::checkAllUpdate(bool force)
{
  for (auto i = 0; i < m_groups.size(); ++i) {
    checkUpdate(i, force);
  }
}

void
GroupList::checkUpdate(int index, bool force)
{
  do {
    if (index >= m_groups.size())
      break;

    auto item = m_groups.at(index);
    if (!item.isSubscription || item.url.isEmpty())
      break;

    if (item.cycle_time >
          item.modified_time.daysTo(QDateTime::currentDateTime()) &&
        !force)
      break;

    DownloadTask task = {
      .filename = item.name,
      .url = item.url,
      .user_agent = p_config->networkUserAgent(),
    };

    connect(p_curl.get(),
            &across::network::CURLTools::downloadFinished,
            this,
            &GroupList::handleUpdated);

    p_curl->download(task);
  } while (false);
}

Q_INVOKABLE int GroupList::getIndexByID(int id) 
{
  for (auto iter = 0; iter < m_groups.size(); ++iter) {
    auto& item = m_groups[iter];
    if(item.id == id) {
      return iter;
    }
  }
  return -1;
}

void
GroupList::search(const QString& value)
{
  QList<GroupInfo> temp_groups;
  auto iter = m_origin_groups.begin();
  auto results = p_db->search(value);
  auto keys = results.keys();

  for (auto& key : keys) {
    for (; iter != m_origin_groups.end(); ++iter) {
      if (iter->id == key) {
        temp_groups.append(*iter);
        ++iter;
        break;
      }
    }
  }

  emit preItemsReset();
  m_groups = temp_groups;
  emit postItemsReset();

  p_nodes->setFilter(results);
  p_nodes->reloadItems();
}

void
GroupList::clearSearch()
{
  emit preItemsReset();
  m_groups = m_origin_groups;
  emit postItemsReset();

  p_nodes->clearFilter();
}

void
GroupList::reloadItems(bool reopen_db, bool reload_nodes)
{
  if (reopen_db) {
    p_db->reload();
  }

  if (auto result = p_db->reloadAllGroupsInfo();
      result.type() == QSqlError::NoError) {
    emit preItemsReset();
    m_groups = p_db->getAllGroupsInfo();
    emit postItemsReset();

    if (m_groups.isEmpty()) {
      p_logger->warn("No group items from database");
      return;
    } else {
      m_origin_groups = m_groups;
    }
  }

  if (reload_nodes) {
    p_nodes->reloadItems();
  }
}

bool
GroupList::insertSIP008(const GroupInfo& group_info, const QString& content)
{
  if (p_db == nullptr)
    return false;

  auto sip008 = SerializeTools::sip008Parser(content.toStdString());
  if (!sip008.has_value()) {
    p_logger->error("Failed to parse download subscription");
    return false;
  }

  QList<NodeInfo> nodes;
  for (auto& server : sip008.value().servers) {
    auto sip002 = SerializeTools::sip002Encode(server).value().toString();

    ShadowsocksObject::OutboundSettingObject shadows_object;
    shadows_object.fromSIP008Server(server);

    OutboundObject outbound_object;
    outbound_object.appendShadowsocksObject(shadows_object);

    NodeInfo node = {
      .id = 0,
      .name = QString::fromStdString(server.remarks),
      .group_id = group_info.id,
      .group_name = group_info.name,
      .protocol = across::EntryType::shadowsocks,
      .address = QString::fromStdString(server.server),
      .port = server.server_port,
      .password = QString::fromStdString(server.password),
      .raw =
        QString::fromStdString(outbound_object.toObject().toStyledString()),
      .url = QString(QUrl(sip002).toEncoded()),
    };

    nodes.append(node);
  }

  if (auto err = p_db->insert(nodes); err.type() != QSqlError::NoError) {
    return false;
  }

  reloadItems();
  return true;
}

bool
GroupList::insertBase64(const GroupInfo& group_info, const QString& content)
{
  if (p_db == nullptr)
    return false;

  QString temp_data;
  if (!content.contains("://"))
    temp_data = QByteArray::fromBase64(content.toUtf8());
  else
    temp_data = content;

  QList<NodeInfo> nodes;
  for (auto& item : temp_data.split("\n")) {
    item.remove("\r");
    if (item.isEmpty())
      break;

    NodeInfo node = {
      .group_id = group_info.id,
      .group_name = group_info.name,
    };

    if (!SerializeTools::decodeOutboundFromURL(node, item))
      return false;
    else
      nodes.append(node);
  }

  if (auto result = p_db->insert(nodes); result.type() != QSqlError::NoError)
    return false;

  reloadItems();
  return true;
}

void
GroupList::appendItem(const QString& group_name,
                      const QString& url,
                      int type,
                      int cycle_time)
{
  DownloadTask task = {
    .filename = group_name,
    .url = url,
    .user_agent = p_config->networkUserAgent(),
  };

  GroupInfo group_info = {
    .name = group_name,
    .isSubscription = true,
    .type = magic_enum::enum_value<SubscriptionType>(type),
    .url = url,
    .cycle_time = cycle_time,
  };

  m_pre_groups.append(group_info);

  connect(p_curl.get(),
          &across::network::CURLTools::downloadFinished,
          this,
          &GroupList::handleDownloaded);

  p_curl->download(task);
}

void
GroupList::appendItem(const QString& group_name, const QString& node_items)
{
  GroupInfo group_info = {
    .name = group_name,
    .isSubscription = false,
    .type = base64,
  };

  if (!insert(group_info, node_items)) {
    p_logger->error("Failed to parse url");
  }
}

void
GroupList::removeItem(int index)
{
  if (m_groups.size() > index) {
    setDisplayGroupID(m_groups.at(index - 1).id);

    emit preItemRemoved(index);

    p_db->removeGroupFromID(m_groups.at(index).id);

    m_groups.removeAt(index);

    emit postItemRemoved();
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
  auto item = m_groups.at(index);

  NotifyTools().send(item.url,
                     QString(tr("Copy [%1] URL to clipboard")).arg(item.name));

  ClipboardTools().send(item.url);
}

void
GroupList::editItem(int index)
{
  if (index < m_groups.size()) {
    m_groups[index].modified_time = QDateTime::currentDateTime();
    auto item = m_groups.at(index);
    p_db->update(item);
    emit itemInfoChanged(index);
  } else {
    p_logger->error("GroupList out of index: {}", index);
  }
}

void
GroupList::handleDownloaded(const QVariant& content)
{
  auto task = content.value<DownloadTask>();
  if (task.content.isEmpty())
    return;

  for (auto i = m_pre_groups.size() - 1; i >= 0; --i) {
    if (auto item = m_pre_groups.at(i); task.filename == item.name)
      if (insert(item, task.content))
        m_pre_groups.removeAt(i);
  }
}

void
GroupList::handleUpdated(const QVariant& content)
{
  auto task = content.value<DownloadTask>();
  if (task.content.isEmpty())
    return;

  for (auto iter = 0; iter < m_groups.size(); ++iter) {
    auto& item = m_groups[iter];
    if (task.filename == item.name) {
      item.modified_time = QDateTime::currentDateTime();
      do {
        if (auto err = p_db->update(item); err.type() != QSqlError::NoError) {
          p_logger->error("Failed to update group: {}",
                          item.name.toStdString());
          break;
        }

        if (auto err = p_db->removeGroupFromID(item.id, true);
            err.type() != QSqlError::NoError) {
          p_logger->error("Failed to remove old table: {}",
                          item.name.toStdString());
          break;
        }

        bool result = false;
        switch (item.type) {
          case sip008:
            result = this->insertSIP008(item, task.content);
            break;
          case base64:
            result = this->insertBase64(item, task.content);
            break;
          default:
            break;
        }

        if (!result) {
          p_logger->error("Failed to insert nodes");
          break;
        }

        p_db->update(item);
      } while (false);

      reloadItems();
      emit itemInfoChanged(iter);
    }
  }
}

void
GroupList::handleItemsChanged(int64_t group_id, int size)
{
  for (auto index = 0; index < m_groups.size(); ++index) {
    if (auto item = m_groups.at(index); item.id == group_id) {
      m_groups[index].items = size;
      emit itemInfoChanged(index);
      break;
    }
  }
}
