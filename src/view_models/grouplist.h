#ifndef ACROSS_GROUPLIST_H
#define ACROSS_GROUPLIST_H

#include <memory>

#include "magic_enum.hpp"
#include <QObject>
#include <QPointer>
#include <QVariant>

#if defined(Q_CC_MINGW) || defined(Q_OS_MACOS)
#include <QSystemTrayIcon>
#endif

#include "../models/acolorstools.h"
#include "../models/clipboardtools.h"
#include "../models/networktools.h"
#include "../models/notifytools.h"
#include "../models/serializetools.h"

#include "configtools.h"
#include "logtools.h"
#include "nodelist.h"
#include "notificationmodel.h"

namespace across {

class GroupList : public QObject {
    Q_OBJECT
  public:
    explicit GroupList(QObject *parent = nullptr);

    void init(QSharedPointer<across::setting::ConfigTools> config,
              QSharedPointer<across::acolorsapi::AColoRSAPITools> acolors,
              QSharedPointer<across::NodeList> nodes,
              QSharedPointer<across::network::CURLTools> curl,
              QSharedPointer<across::NotificationModel> notifications,
              const QSharedPointer<QSystemTrayIcon> &tray = nullptr);

    bool insert(const GroupInfo &group_info, const QString &content);
    bool insertSIP008(const GroupInfo &group_info, const QString &content);
    bool insertBase64(const GroupInfo &group_info, const QString &content);

    QList<GroupInfo> items() const;

    Q_INVOKABLE void checkAllUpdate(bool force = false);
    Q_INVOKABLE void checkUpdate(int index, bool force = true);

    Q_INVOKABLE int testTcpPing(int index);
    Q_INVOKABLE int testTcpPingLeft(int index);

    Q_INVOKABLE int getIndexByID(int id);
    // Q_INVOKABLE void search(const QString &value);
    // Q_INVOKABLE void clearSearch();
    Q_INVOKABLE QVariantMap getGroupInfo(int index);

  public slots:
    void reloadItems(bool reopen_db = false);
    void appendItem(const QString &group_name, const QString &url, int type,
                    int cycle_time);
    void appendItem(const QString &group_name, const QString &node_items);
    void editItem(int index, const QString &group_name, bool is_subscription,
                  const QString &url, int type, int cycle_time,
                  const QString &node_items);
    void removeItem(int index);
    void setDisplayGroupID(int id);
    void copyUrlToClipboard(int index);
    void copyNodesToClipboard(int index);
    void handleDownloaded(const QVariant &content);
    void handleUpdatedGroup(const int32_t group_id);
    void handleItemsChanged(int64_t group_id, int size);
    void handleNodeLatencyChanged(qint64 group_id, int index,
                                  const across::NodeInfo &node);

  signals:
    void preItemsReset();
    void postItemsReset();

    void itemInfoChanged(int index);
    void nodeLatencyChanged(qint64 group_id, int index,
                            const across::NodeInfo &node);

  private:
    QSharedPointer<across::setting::ConfigTools> p_config;
    QSharedPointer<across::acolorsapi::AColoRSAPITools> p_acolors;
    QSharedPointer<across::NodeList> p_nodes;
    QSharedPointer<across::network::CURLTools> p_curl;
    QSharedPointer<across::NotificationModel> p_notifications;
    QSharedPointer<QSystemTrayIcon> p_tray;

    std::shared_ptr<spdlog::logger> p_logger;

    QList<GroupInfo> m_groups;
    QList<GroupInfo> m_origin_groups;
    QList<GroupInfo> m_pre_groups;
    QMap<int64_t, bool> m_is_updating;
    QMap<int64_t, bool> m_is_tcpPinging;
    QMap<int64_t, int> m_tcpPinging_count;
    QMap<int64_t, int> m_group_size;
    QMap<int64_t, Notification *> m_tcpPinging_notifications;
};
} // namespace across

#endif // ACROSS_GROUPLIST_H
