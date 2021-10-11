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

QVector<GroupInfo>
GroupList::items() const
{
  return m_items;
}

void
GroupList::checkAllUpdate(bool force)
{
  for (auto i = 0; i < m_items.size(); ++i) {
    checkUpdate(i, force);
  }
}

void
GroupList::checkUpdate(int index, bool force)
{
  do {
    if (index >= m_items.size())
      break;

    auto item = m_items.at(index);
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

Q_INVOKABLE void GroupList::testTcpPing(int index) 
{
  /*
  QFuture<void> waitFuture = QtConcurrent::run([&,index]{
    do{
      if (index >= m_items.size())
        break;
      auto item = m_items.at(index);    


      p_nodes->setDisplayGroupID(item.id);
      qint64 len = p_nodes->items().size();
      QVector<QFuture<void>> setFuture(len);
      QVector<NodeInfo> nodes(len);
      for (int i=0; i<len;i++){
        nodes[i]=p_nodes->items().at(i);
        setFuture[i] = QtConcurrent::run(&NodeList::setLatency,
                                                    p_nodes.get(),
                                                    nodes[i].id);
      }
      for (int i=0; i<len;i++){
        setFuture[i].waitForFinished();
      }
    } while(false);
  });
  */
}

Q_INVOKABLE int GroupList::getIndexByID(int id) 
{
  for(auto iter = 0; iter < m_items.size(); ++iter){
    auto& item = m_items[iter];
    if(item.id == id) {
      return iter;
    }
  }
  return -1;
}

void
GroupList::reloadItems(bool reopen_db)
{
  if (reopen_db) {
    p_db->reload();
  }

  if (auto result = p_db->reloadAllGroupsInfo();
      result.type() == QSqlError::NoError) {
    m_items = p_db->getAllGroupsInfo();
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

  QString temp_data;
  if (!content.contains("://")) {
    temp_data = QByteArray::fromBase64(content.toUtf8());
  } else {
    temp_data = content;
  }

  for (auto& item : temp_data.split("\n")) {
    item.remove("\r");
    if (item.isEmpty()) {
      break;
    }

    NodeInfo node = {
      .group_id = group_info.id,
      .group_name = group_info.name,
    };

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
  if (m_items.size() > index) {
    setDisplayGroupID(m_items.at(index - 1).id);

    emit preItemRemoved(index);

    p_db->removeGroupFromID(m_items.at(index).id);

    m_items.removeAt(index);

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
  auto item = m_items.at(index);

  NotifyTools().send(item.url,
                     QString(tr("Copy [%1] URL to clipboard")).arg(item.name));

  ClipboardTools().send(item.url);
}

void
GroupList::editItem(int index)
{
  if (index < m_items.size()) {
    m_items[index].modified_time = QDateTime::currentDateTime();
    auto item = m_items.at(index);
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

  for (auto i = m_pre_items.size() - 1; i >= 0; --i) {
    if (auto item = m_pre_items.at(i); task.filename == item.name)
      if (insert(item, task.content))
        m_pre_items.removeAt(i);
  }
}

void
GroupList::handleUpdated(const QVariant& content)
{
  auto task = content.value<DownloadTask>();
  if (task.content.isEmpty())
    return;

  for (auto iter = 0; iter < m_items.size(); ++iter) {
    auto& item = m_items[iter];
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

        //        item.id = p_db->getLastID();
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
  for (auto index = 0; index < m_items.size(); ++index) {
    if (auto item = m_items.at(index); item.id == group_id) {
      m_items[index].items = size;
      emit itemInfoChanged(index);
      break;
    }
  }
}
