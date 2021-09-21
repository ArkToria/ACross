#ifndef ACROSS_GROUPLIST_H
#define ACROSS_GROUPLIST_H

#include <memory>

#include "magic_enum.hpp"
#include <QObject>
#include <QPointer>
#include <QVariant>

#include "../models/clipboardtools.h"
#include "../models/cryptotools.h"
#include "../models/dbtools.h"
#include "../models/networktools.h"
#include "../models/notifytools.h"
#include "../models/serializetools.h"

#include "configtools.h"
#include "logtools.h"
#include "nodelist.h"

namespace across {
class GroupList : public QObject
{
  Q_OBJECT
public:
  explicit GroupList(QObject* parent = nullptr);

  void init(QSharedPointer<LogView> log_view,
            QSharedPointer<across::setting::ConfigTools> config,
            QSharedPointer<across::DBTools> db,
            QSharedPointer<across::NodeList> nodes,
            QSharedPointer<across::network::CURLTools> curl);

  bool insert(GroupInfo& group_info, const QString& content);

  bool insertSIP008(const GroupInfo& group_info, const QString& content);

  bool insertBase64(const GroupInfo& group_info, const QString& content);

  QVector<GroupInfo> items() const;

public slots:
  void reloadItems(bool reopen_db = false);

  void appendItem(const QString& group_name,
                  const QString& url,
                  int type,
                  int cycle_time);

  void appendItem(const QString& group_name, const QString& node_items);

  void removeItem(int index);

  void upgradeItem(int index);

  void setDisplayGroupID(int id);

  void copyUrlToClipboard(int index);

signals:
  void preItemAppended();
  void postItemAppended();

  void preItemRemoved(int index);
  void postItemRemoved();

  void preLastItemRemoved();
  void postLastItemRemoved();

private:
  QSharedPointer<across::setting::ConfigTools> p_config;
  QSharedPointer<across::DBTools> p_db;
  QSharedPointer<across::NodeList> p_nodes;
  QSharedPointer<across::network::CURLTools> p_curl;
  std::shared_ptr<across::utils::LogTools> p_logger;

  QVector<GroupInfo> m_items;
  int64_t m_current_group_id;
};
}

#endif // ACROSS_GROUPLIST_H
