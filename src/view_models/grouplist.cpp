#include "grouplist.h"

#include <utility>

using namespace across;
using namespace across::utils;
using namespace across::config;
using namespace across::setting;
using namespace across::network;

GroupList::GroupList(QObject *parent) : QObject(parent) {}

void GroupList::init(
    QSharedPointer<across::setting::ConfigTools> config,
    QSharedPointer<across::acolorsapi::AColoRSAPITools> acolors,
    QSharedPointer<across::NodeList> nodes,
    QSharedPointer<across::network::CURLTools> curl,
    QSharedPointer<across::NotificationModel> notifications,
    const QSharedPointer<QSystemTrayIcon> &tray) {
    if (auto app_logger = spdlog::get("app"); app_logger != nullptr) {
        p_logger = app_logger->clone("groups");
    } else {
        qCritical("Failed to start logger");
        return;
    }

    p_config = std::move(config);
    p_acolors = std::move(acolors);
    p_curl = std::move(curl);
    p_nodes = std::move(nodes);
    p_notifications = std::move(notifications);

    if (tray != nullptr) {
        p_tray = tray;
    }

    connect(p_nodes.get(), &NodeList::groupSizeChanged, this,
            &GroupList::handleItemsChanged);

    connect(p_curl.get(), &across::network::CURLTools::downloadFinished, this,
            &GroupList::handleDownloaded);

    connect(&p_acolors->notifications(),
            &acolorsapi::AColoRSNotifications::updateGroup, this,
            &GroupList::handleUpdatedGroup);

    connect(this, &GroupList::nodeLatencyChanged, this,
            &GroupList::handleNodeLatencyChanged);

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

        if (m_is_updating.contains(group.id)) {
            break;
        }
        m_is_updating[group.id] = true;

        p_acolors->profile().updateGroupById(group.id);

        /*
        DownloadTask task = {
            .id = group.id,
            .name = group.name,
            .url = group.url,
            .user_agent = p_config->networkUserAgent(),
            .is_updated = true,
        };

        p_nodes->setDownloadProxy(task);

        p_curl->download(task);
        */
    } while (false);
}

Q_INVOKABLE int GroupList::testTcpPing(int index) {
    if (index >= m_groups.size())
        return 1;

    if (auto &group = m_groups[index]; group.items != 0) {
        if (m_is_tcpPinging.contains(group.id)) {
            return 1;
        }
        m_is_tcpPinging[group.id] = true;

        auto list_all_nodes =
            p_acolors->profile().listAllNodes(int32_t(group.id));
        if (!list_all_nodes.second.ok()) {
            qDebug() << "testTcpPing Error:"
                     << list_all_nodes.second.error_message().c_str();
            return 1;
        }

        auto &nodes = list_all_nodes.first;

        m_tcpPinging_count[group.id] = 0;
        m_group_size[group.id] = nodes.size();
        m_tcpPinging_notifications[group.id] = p_notifications->append(
            tr("[%1] TCP Pinging...").arg(group.name),
            tr("Testing: %1/%2")
                .arg("0")
                .arg(QString::number(m_group_size[group.id])),
            0.0, m_group_size[group.id], 0.0);

        for (int i = 0; i < nodes.size(); ++i) {
            auto &node = nodes[i];
            p_nodes->testLatency(node, i, [this, node, i] {
                emit nodeLatencyChanged(node.group_id, i, node);
            });
        }
    }
    return 0;
}
Q_INVOKABLE int GroupList::testTcpPingLeft(int index) {
    auto &group = m_groups[index];
    if (m_tcpPinging_count.contains(group.id)) {
        return -m_tcpPinging_count[group.id];
    }
    return 0;
}
void GroupList::handleNodeLatencyChanged(qint64 group_id, int index,
                                         const across::NodeInfo &node) {
    m_tcpPinging_count[group_id]++;
    m_tcpPinging_notifications[group_id]->setMessage(
        tr("Testing: %1/%2")
            .arg(QString::number(m_tcpPinging_count[group_id]))
            .arg(QString::number(m_group_size[group_id])));
    m_tcpPinging_notifications[group_id]->setValue(
        m_tcpPinging_count[group_id]);
    if (m_tcpPinging_count[group_id] >= m_group_size[group_id]) {
        p_notifications->remove(
            m_tcpPinging_notifications[group_id]->getIndex());
        m_tcpPinging_count.remove(group_id);
        m_tcpPinging_notifications.remove(group_id);
        m_is_tcpPinging.remove(group_id);
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

        auto list_all_nodes =
            p_acolors->profile().listAllNodes(int32_t(group.id));
        if (!list_all_nodes.second.ok()) {
            qDebug() << "getGroupInfo Error:"
                     << list_all_nodes.second.error_message().c_str();
        }
        for (auto &node : list_all_nodes.first) {
            nodes_url.append(node.url);
            nodes_url.append("\n");
        }

        info.insert("nodesURL", nodes_url);
    }

    return info;
}

void GroupList::reloadItems(bool reopen_db) {
    if (reopen_db) {
        // p_db->reload();
    }

    auto list_all_groups = p_acolors->profile().listAllGroups();
    if (list_all_groups.second.ok()) {
        emit preItemsReset();
        m_groups = list_all_groups.first;
        emit postItemsReset();

        if (m_groups.isEmpty()) {
            p_logger->warn("No group items from database");
            return;
        } else {
            m_origin_groups = m_groups;
        }
    } else {
        qDebug() << "reloadItems Error:"
                 << list_all_groups.second.error_message().c_str();
    }

    p_nodes->reloadItems();
}

bool GroupList::insertSIP008(const GroupInfo &group_info,
                             const QString &content) {
    if (p_acolors == nullptr)
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
        const auto &server = shadowsocks.servers(0);

        std::string json_str;

        NodeInfo node = {
            .id = 0,
            .name = QString::fromStdString(meta.name),
            .group_id = group_info.id,
            .group_name = group_info.name,
            .routing_id = 0,
            .routing_name = "default_routings",
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

    if (auto status =
            p_acolors->profile().appendNodes(int32_t(group_info.id), nodes);
        !status.ok()) {
        return false;
    }

    reloadItems();
    return true;
}

bool GroupList::insertBase64(const GroupInfo &group_info,
                             const QString &content) {
    if (p_acolors == nullptr)
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
            .routing_id = 0,
            .routing_name = "default_routings",
        };

        if (!SerializeTools::decodeOutboundFromURL(node, item.toStdString()))
            return false;
        else
            nodes.append(node);
    }

    if (auto status =
            p_acolors->profile().appendNodes(int32_t(group_info.id), nodes);
        !status.ok()) {
        return false;
    }

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

    p_curl->download(task);
}

void GroupList::appendItem(const QString &group_name,
                           const QString &node_items) {
    GroupInfo group_info = {
        .name = group_name,
        .is_subscription = false,
        .type = base64,
    };

    if (auto result = p_acolors->profile().appendGroup(group_info);
        !result.ok()) {
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

    if (auto result = p_acolors->profile().setGroupById(group.id, group);
        result.ok()) {
        if (auto result = p_acolors->profile().emptyGroupById(group.id);
            !result.ok())
            return;
    } else {
        return;
    }

    if (is_url_changed) {
        m_is_updating[group.id] = true;

        p_acolors->profile().updateGroupById(group.id);

        /*
        DownloadTask task = {
            .id = group.id,
            .name = group.name,
            .url = group.url,
            .user_agent = p_config->networkUserAgent(),
            .is_updated = true,
        };

        p_nodes->setDownloadProxy(task);

        p_curl->download(task);
        */
    } else if (!node_items.isEmpty()) {
        this->insertBase64(group, node_items);
    }
}

void GroupList::removeItem(int index) {
    if (m_groups.size() > index) {
        emit preItemsReset();
        setDisplayGroupID(m_groups.at(index - 1).id);
        p_acolors->profile().removeGroupById(m_groups.at(index).id);
        m_groups.removeAt(index);
        emit postItemsReset();
    }
}

void GroupList::setDisplayGroupID(int id) { p_nodes->setDisplayGroupID(id); }

void GroupList::copyUrlToClipboard(int index) {
    auto item = m_groups.at(index);

#if defined(Q_CC_MINGW) || defined(Q_OS_MACOS)
    NotifyTools::send(item.url,
                      QString(tr("Copy [%1] URL to clipboard")).arg(item.name),
                      p_tray);
#else
    NotifyTools::send(item.url,
                      QString(tr("Copy [%1] URL to clipboard")).arg(item.name));
#endif

    ClipboardTools::send(item.url);
}

void GroupList::copyNodesToClipboard(int index) {
    auto item = m_groups.at(index);

    QString nodes_url;
    for (auto &node : p_acolors->profile().listAllNodes(item.id).first) {
        nodes_url.append(node.url);
        nodes_url.append("\n");
    }

#if defined(Q_CC_MINGW) || defined(Q_OS_MACOS)
    NotifyTools::send(nodes_url,
                      QString(tr("Copy [%1] URL to clipboard")).arg(item.name),
                      p_tray);
#else
    NotifyTools::send(nodes_url,
                      QString(tr("Copy [%1] URL to clipboard")).arg(item.name));
#endif

    ClipboardTools::send(nodes_url);
}

void GroupList::handleDownloaded(const QVariant &content) {
    auto task = content.value<DownloadTask>();
    if (task.content.isEmpty()) {
        if (task.is_updated)
            m_is_updating.remove(task.id);
        return;
    }

    if (task.is_updated) {
        QList<GroupInfo> temp_groups;
        for (auto &item : m_groups) {
            if (item.id == task.id) {
                auto group = item;
                if (auto result = p_acolors->profile().emptyGroupById(group.id);
                    !result.ok())
                    break;
                if (!insert(group, task.content))
                    break;

                temp_groups.append(group);
                break;
            }
        }

        if (auto result = p_acolors->profile().setGroups(temp_groups);
            !result.ok()) {
            m_is_updating.remove(task.id);
            return;
        } else {
            p_acolors->core().setDefaultConfigByNodeId(0);
        }

        m_is_updating.remove(task.id);
    } else {
        for (auto i = 0; i < m_pre_groups.size(); ++i) {
            if (m_pre_groups.at(i).name == task.name) {
                auto group = m_pre_groups.at(i);

                if (auto result = p_acolors->profile().appendGroup(group);
                    !result.ok())
                    break;

                if (!insert(group, task.content))
                    break;

                m_pre_groups.remove(i);
                break;
            }
        }
    }

    reloadItems();
}
void GroupList::handleUpdatedGroup(const int32_t group_id) {
    m_is_updating.remove(group_id);

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
