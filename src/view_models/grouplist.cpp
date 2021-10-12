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
  p_curl = curl;
  p_nodes = nodes;

  connect(p_nodes.get(),
          &NodeList::itemsSizeChanged,
          this,
          &GroupList::handleItemsChanged);

  reloadItems();
  checkAllUpdate();
}

bool
GroupList::insert(const GroupInfo& group_info, const QString& content)
{
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
  return result;
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

    auto group = m_groups.at(index);
    if (!group.isSubscription || group.url.isEmpty())
      break;

    if (group.cycle_time >
          group.modified_time.daysTo(QDateTime::currentDateTime()) &&
        !force)
      break;

    DownloadTask task = {
      .id = group.id,
      .name = group.name,
      .url = group.url,
      .user_agent = p_config->networkUserAgent(),
    };

    connect(p_curl.get(),
            &across::network::CURLTools::downloadFinished,
            this,
            &GroupList::handleUpdated);

    p_curl->download(task);
  } while (false);
}

Q_INVOKABLE int
GroupList::getIndexByID(int id)
{
  for (auto iter = 0; iter < m_groups.size(); ++iter) {
    auto& item = m_groups[iter];
    if (item.id == id) {
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

  if (results.isEmpty()) {
    emit preItemsReset();
    m_groups.clear();
    emit postItemsReset();
    p_nodes->clearItems();
    return;
  }

  for (auto& result : results.toStdMap()) {
    for (; iter != m_origin_groups.end(); ++iter) {
      if (iter->id == result.first) {
        auto group = *iter;
        group.items = result.second.size();
        temp_groups.append(group);
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
  setDisplayGroupID(results.firstKey());
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
GroupList::reloadItems(bool reopen_db)
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

  p_nodes->reloadItems();
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
    .name = group_name,
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

  if (auto result = p_db->insert(group_info);
      result.type() != QSqlError::NoError) {
    return;
  }

  if (!insert(group_info, node_items)) {
    p_logger->error("Failed to parse url");
  }
}

void
GroupList::removeItem(int index)
{
  if (m_groups.size() > index) {
    emit preItemsReset();
    setDisplayGroupID(m_groups.at(index - 1).id);
    p_db->removeGroupFromID(m_groups.at(index).id);
    m_groups.removeAt(index);
    emit postItemsReset();
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

  for (auto i = 0; i < m_pre_groups.size(); ++i) {
    if (m_pre_groups.at(i).name == task.name) {
      auto group = m_pre_groups.at(i);

      if (auto result = p_db->insert(group);
          result.type() != QSqlError::NoError)
        break;

      if (!insert(group, task.content))
        break;

      m_pre_groups.remove(i);
      break;
    }
  }

  reloadItems();
}

void
GroupList::handleUpdated(const QVariant& content)
{
  auto task = content.value<DownloadTask>();
  if (task.content.isEmpty())
    return;

  for (auto group : m_groups) {
    if (group.id == task.id) {
      if (auto result = p_db->removeGroupFromID(group.id, true);
          result.type() != QSqlError::NoError)
        break;
      if (!insert(group, task.content))
        break;
      if (auto result = p_db->update(group);
          result.type() != QSqlError::NoError)
        break;

      break;
    }
  }

  reloadItems();
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
