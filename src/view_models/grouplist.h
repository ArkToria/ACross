#ifndef ACROSS_GROUPLIST_H
#define ACROSS_GROUPLIST_H

#include <memory>

#include "magic_enum.hpp"
#include <QObject>
#include <QPointer>
#include <QVariant>

#ifdef __MINGW32__
#include <QSystemTrayIcon>
#endif

#include "../models/clipboardtools.h"
#include "../models/dbtools.h"
#include "../models/networktools.h"
#include "../models/notifytools.h"
#include "../models/serializetools.h"

#include "configtools.h"
#include "logtools.h"
#include "nodelist.h"

namespace across {

class GroupList : public QObject {
    Q_OBJECT
  public:
    explicit GroupList(QObject *parent = nullptr);

    void init(QSharedPointer<across::setting::ConfigTools> config,
              QSharedPointer<across::DBTools> db,
              QSharedPointer<across::NodeList> nodes,
              QSharedPointer<across::network::CURLTools> curl
#ifdef __MINGW32__
              ,
              QSharedPointer<QSystemTrayIcon> tray);
#else
    );
#endif

    bool insert(const GroupInfo &group_info, const QString &content);
    bool insertSIP008(const GroupInfo &group_info, const QString &content);
    bool insertBase64(const GroupInfo &group_info, const QString &content);

    QList<GroupInfo> items() const;

    Q_INVOKABLE void checkAllUpdate(bool force = false);
    Q_INVOKABLE void checkUpdate(int index, bool force = true);

    Q_INVOKABLE void testTcpPing(int index);

    Q_INVOKABLE int getIndexByID(int id);
    Q_INVOKABLE void search(const QString &value);
    Q_INVOKABLE void clearSearch();
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
    void handleItemsChanged(int64_t group_id, int size);

  signals:
    void preItemsReset();
    void postItemsReset();

    void itemInfoChanged(int index);

  private:
    QSharedPointer<across::setting::ConfigTools> p_config;
    QSharedPointer<across::DBTools> p_db;
    QSharedPointer<across::NodeList> p_nodes;
    QSharedPointer<across::network::CURLTools> p_curl;
#ifdef __MINGW32__
    QSharedPointer<QSystemTrayIcon> p_tray;
#endif

    std::shared_ptr<spdlog::logger> p_logger;

    QList<GroupInfo> m_groups;
    QList<GroupInfo> m_origin_groups;
    QList<GroupInfo> m_pre_groups;
};
} // namespace across

#endif // ACROSS_GROUPLIST_H
