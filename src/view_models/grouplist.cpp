#include "grouplist.h"

using namespace across;
using namespace across::utils;
using namespace across::config;
using namespace across::setting;
using namespace across::network;

GroupList::GroupList(QObject *parent) {}

void GroupList::init(QSharedPointer<across::setting::ConfigTools> config,
                     QSharedPointer<across::DBTools> db,
                     QSharedPointer<across::NodeList> nodes,
                     QSharedPointer<across::network::CURLTools> curl
#ifdef __MINGW32__
                     ,
                     QSharedPointer<QSystemTrayIcon> tray) {
#else
) {
#endif
    if (auto app_logger = spdlog::get("app"); app_logger != nullptr) {
        p_logger = app_logger->clone("groups");
    } else {
        qCritical("Failed to start logger");
        return;
    }

    p_config = config;
    p_db = db;
    p_curl = curl;
    p_nodes = nodes;
#ifdef __MINGW32__
    p_tray = tray;
#endif

    connect(p_nodes.get(), &NodeList::groupSizeChanged, this,
            &GroupList::handleItemsChanged);

    reloadItems();
    checkAllUpdate();
}

bool GroupList::insert(const GroupInfo &group_info, const QString &content) {
    bool result = false;
    switch (group_info.type) {
    case base64:
        result = this->insertBase64(group_info, content);
        break;
    case sip008:
        result = this->insertSIP008(group_info, content);
        break;

    default:
        break;
    }
    return result;
}

QList<GroupInfo> GroupList::items() const { return m_groups; }

void GroupList::checkAllUpdate(bool force) {
    for (auto i = 0; i < m_groups.size(); ++i) {
        checkUpdate(i, force);
    }
}

void GroupList::checkUpdate(int index, bool force) {
    do {
        if (index >= m_groups.size())
            break;

        auto group = m_groups.at(index);
        if (!group.is_subscription || group.url.isEmpty())
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

        p_nodes->setDownloadProxy(task);

        connect(p_curl.get(), &across::network::CURLTools::downloadFinished,
                this, &GroupList::handleUpdated);

        p_curl->download(task);
    } while (false);
}

Q_INVOKABLE void GroupList::testTcpPing(int index) {
    if (index >= m_groups.size())
        return;

    if (auto &group = m_groups[index]; group.items != 0) {
        auto nodes = p_db->listAllNodesFromGroupID(group.id);

        for (int i = 0; i < nodes.size(); ++i) {
            auto &node = nodes[i];
            p_nodes->testLatency(node, i);
        }
    }
}

Q_INVOKABLE int GroupList::getIndexByID(int id) {
    for (auto iter = 0; iter < m_groups.size(); ++iter) {
        auto &item = m_groups[iter];
        if (item.id == id) {
            return iter;
        }
    }
    return -1;
}

void GroupList::search(const QString &value) {
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

    for (auto &result : results.toStdMap()) {
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

void GroupList::clearSearch() {
    emit preItemsReset();
    m_groups = m_origin_groups;
    emit postItemsReset();

    p_nodes->clearFilter();
}

QVariantMap GroupList::getGroupInfo(int index) {
    QVariantMap info;
    QString nodes_url;

    if (index < m_groups.size()) {
        nodes_url.clear();

        auto &group = m_groups.at(index);
        info.insert("Name", group.name);
        info.insert("isSubscription", group.is_subscription);
        info.insert("type", group.type);
        info.insert("url", group.url);
        info.insert("cycleTime", group.cycle_time);

        for (auto &node : p_db->listAllNodesFromGroupID(group.id)) {
            nodes_url.append(node.url);
            nodes_url.append("\n");
        }

        info.insert("nodesURL", nodes_url);
    }

    return info;
}

void GroupList::reloadItems(bool reopen_db) {
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

bool GroupList::insertSIP008(const GroupInfo &group_info,
                             const QString &content) {
    if (p_db == nullptr)
        return false;

    auto meta_objects = SerializeTools::sip008Parser(content.toStdString());
    if (!meta_objects.has_value()) {
        p_logger->error("Failed to parse download subscription");
        return false;
    }

    QList<NodeInfo> nodes;
    for (auto &meta : meta_objects.value()) {
        auto url = SerializeTools::sip002Encode(meta).value();
        auto outbound = meta.outbound;
        auto shadowsocks = outbound.settings().shadowsocks();
        auto server = shadowsocks.servers(0);

        std::string json_str;

        NodeInfo node = {
            .id = 0,
            .name = QString::fromStdString(meta.name),
            .group_id = group_info.id,
            .group_name = group_info.name,
            .protocol = across::EntryType::shadowsocks,
            .address = server.address().c_str(),
            .port = server.port(),
            .password = server.password().c_str(),
            .raw =
                QString::fromStdString(SerializeTools::MessageToJson(outbound)),
            .url = QString(url.toEncoded()),
        };

        nodes.append(node);
    }

    if (auto err = p_db->insert(nodes); err.type() != QSqlError::NoError) {
        return false;
    }

    reloadItems();
    return true;
}

bool GroupList::insertBase64(const GroupInfo &group_info,
                             const QString &content) {
    if (p_db == nullptr)
        return false;

    QString temp_data;
    if (!content.contains("://"))
        temp_data = QByteArray::fromBase64(content.toUtf8());
    else
        temp_data = content;

    QList<NodeInfo> nodes;
    for (auto &item : temp_data.split("\n")) {
        item.remove("\r");
        if (item.isEmpty())
            break;

        NodeInfo node = {
            .group_id = group_info.id,
            .group_name = group_info.name,
        };

        if (!SerializeTools::decodeOutboundFromURL(node, item.toStdString()))
            return false;
        else
            nodes.append(node);
    }

    if (auto result = p_db->insert(nodes); result.type() != QSqlError::NoError)
        return false;

    reloadItems();
    return true;
}

void GroupList::appendItem(const QString &group_name, const QString &url,
                           int type, int cycle_time) {
    DownloadTask task = {
        .name = group_name,
        .url = url,
        .user_agent = p_config->networkUserAgent(),
    };

    p_nodes->setDownloadProxy(task);

    GroupInfo group_info = {
        .name = group_name,
        .is_subscription = true,
        .type = magic_enum::enum_value<SubscriptionType>(type),
        .url = url,
        .cycle_time = cycle_time,
    };

    m_pre_groups.append(group_info);

    connect(p_curl.get(), &across::network::CURLTools::downloadFinished, this,
            &GroupList::handleDownloaded);

    p_curl->download(task);
}

void GroupList::appendItem(const QString &group_name,
                           const QString &node_items) {
    GroupInfo group_info = {
        .name = group_name,
        .is_subscription = false,
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

void GroupList::editItem(int index, const QString &group_name,
                         bool is_subscription, const QString &url, int type,
                         int cycle_time, const QString &node_items) {
    // disable edit for default group
    if (index >= m_groups.size())
        return;

    auto group = m_groups.at(index);
    group.is_subscription = is_subscription;

    if (!group_name.isEmpty() && index != 0)
        group.name = group_name;

    bool is_url_changed = false;
    if (group.is_subscription) {
        group.type = magic_enum::enum_value<SubscriptionType>(type);
        group.cycle_time = cycle_time;

        if (group.url != url || node_items.isEmpty()) {
            is_url_changed = true;
            group.url = url;
        }
    }

    m_groups[index] = group;

    if (auto result = p_db->update(group);
        result.type() == QSqlError::NoError) {
        if (auto result = p_db->removeGroupFromID(group.id, true);
            result.type() != QSqlError::NoError)
            return;
    } else {
        return;
    }

    if (is_url_changed) {
        DownloadTask task = {
            .id = group.id,
            .name = group.name,
            .url = group.url,
            .user_agent = p_config->networkUserAgent(),
        };

        p_nodes->setDownloadProxy(task);

        connect(p_curl.get(), &across::network::CURLTools::downloadFinished,
                this, &GroupList::handleUpdated);

        p_curl->download(task);
    } else if (!node_items.isEmpty()) {
        this->insertBase64(group, node_items);
    }
}

void GroupList::removeItem(int index) {
    if (m_groups.size() > index) {
        emit preItemsReset();
        setDisplayGroupID(m_groups.at(index - 1).id);
        p_db->removeGroupFromID(m_groups.at(index).id);
        m_groups.removeAt(index);
        emit postItemsReset();
    }
}

void GroupList::setDisplayGroupID(int id) { p_nodes->setDisplayGroupID(id); }

void GroupList::copyUrlToClipboard(int index) {
    auto item = m_groups.at(index);
    NotifyTools::send(item.url,
                      QString(tr("Copy [%1] URL to clipboard")).arg(item.name)
#ifdef __MINGW32__
                          ,
                      p_tray);
#else
    );
#endif
    ClipboardTools::send(item.url);
}

void GroupList::copyNodesToClipboard(int index) {
    auto item = m_groups.at(index);

    QString nodes_url;
    for (auto &node : p_db->listAllNodesFromGroupID(item.id)) {
        nodes_url.append(node.url);
        nodes_url.append("\n");
    }

    NotifyTools::send(nodes_url,
                      QString(tr("Copy [%1] URL to clipboard")).arg(item.name)
#ifdef __MINGW32__
                          ,
                      p_tray);
#else
    );
#endif
    ClipboardTools::send(nodes_url);
}

void GroupList::handleDownloaded(const QVariant &content) {
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

void GroupList::handleUpdated(const QVariant &content) {
    auto task = content.value<DownloadTask>();
    if (task.content.isEmpty())
        return;

    QList<GroupInfo> temp_groups;
    for (auto &item : m_groups) {
        if (item.id == task.id) {
            auto group = item;
            if (auto result = p_db->removeGroupFromID(group.id, true);
                result.type() != QSqlError::NoError)
                break;
            if (!insert(group, task.content))
                break;

            temp_groups.append(group);
            break;
        }
    }

    if (auto result = p_db->update(temp_groups);
        result.type() != QSqlError::NoError) {
        return;
    } else {
        p_db->updateRuntimeValue(
            RuntimeValue(RunTimeValues::DEFAULT_NODE_ID, 0));
    }

    reloadItems();
}

void GroupList::handleItemsChanged(int64_t group_id, int size) {
    for (auto index = 0; index < m_groups.size(); ++index) {
        if (auto item = m_groups.at(index); item.id == group_id) {
            m_groups[index].items = size;
            emit itemInfoChanged(index);
            break;
        }
    }
}
