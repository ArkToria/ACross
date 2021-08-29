#ifndef NODELIST_H
#define NODELIST_H

#include "../models/coretools.h"
#include "../models/dbtools.h"
#include "../models/jsontools.h"
#include "../view_models/configtools.h"

#include "magic_enum.hpp"
#include <QObject>
#include <QPointer>
#include <QSharedPointer>
#include <QVariant>

namespace across {

class NodeList : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int currentNodeID READ currentNodeID NOTIFY currentNodeIDChanged)
  Q_PROPERTY(
    int currentNodeIndex READ currentNodeIndex NOTIFY currentNodeIndexChanged)
  Q_PROPERTY(int currentGroupID READ currentGroupID WRITE setCurrentGroupID
               NOTIFY currentGroupIDChanged)
  Q_PROPERTY(int displayGroupID READ displayGroupID WRITE setDisplayGroupID
               NOTIFY displayGroupIDChanged)
  Q_PROPERTY(
    QString currentNodeName READ currentNodeName NOTIFY currentNodeNameChanged)
  Q_PROPERTY(QString currentNodeGroup READ currentNodeGroup NOTIFY
               currentNodeGroupChanged)
  Q_PROPERTY(QString currentNodeProtocol READ currentNodeProtocol NOTIFY
               currentNodeProtocolChanged)
  Q_PROPERTY(QString currentNodeAddress READ currentNodeAddress NOTIFY
               currentNodeAddressChanged)
  Q_PROPERTY(
    int currentNodePort READ currentNodePort NOTIFY currentNodePortChanged)

public:
  explicit NodeList(QObject* parent = nullptr);

  void init(std::shared_ptr<spdlog::details::thread_pool> thread_pool,
            DBTools& db,
            across::setting::ConfigTools& config_tools,
            across::core::CoreTools& core_tools);

  QVector<NodeInfo> items();

  void reloadItems();

  void appendNode(NodeInfo node);

  Q_INVOKABLE void removeCurrentNode(int id);

public:
  int currentNodeID();

  int currentNodeIndex();

  int currentGroupID();

  int displayGroupID();

  QString currentNodeName();

  QString currentNodeGroup();

  QString currentNodeProtocol();

  QString currentNodeAddress();

  int currentNodePort();

public slots:
  void setCurrentGroupID(int group_id);

  void setDisplayGroupID(int group_id);

  void setCurrentNode(int id, int index);

signals:
  void preItemsReset();
  void postItemsReset();

  void preItemAppended();
  void postItemAppended();

  void preItemRemoved(int index);
  void postItemRemoved();

  void currentNodeIDChanged();
  void currentNodeIndexChanged();
  void currentGroupIDChanged();
  void displayGroupIDChanged();
  void currentNodeNameChanged();
  void currentNodeGroupChanged();
  void currentNodeProtocolChanged();
  void currentNodeAddressChanged();
  void currentNodePortChanged();

private:
  DBTools* p_db;
  std::shared_ptr<LogTools> p_logger;
  across::setting::ConfigTools* p_config;
  across::core::CoreTools* p_core;
  std::shared_ptr<across::config::JsonTools> p_json;

  QVector<NodeInfo> m_items;
  QMap<int, NodesInfo> m_all_items;

  int m_group_id = 1;
  int m_display_group_id = 1;
  NodeInfo m_current_node;
};
}

#endif // NODELIST_H
