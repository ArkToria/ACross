#ifndef NODELIST_H
#define NODELIST_H

#include "../models/clipboardtools.h"
#include "../models/apitools.h"
#include "../models/coretools.h"
#include "../models/dbtools.h"
#include "../models/jsontools.h"
#include "../models/notifytools.h"

#include "configtools.h"
#include "logtools.h"

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
  Q_PROPERTY(
    QString currentNodeURL READ currentNodeURL NOTIFY currentNodeURLChanged)
  Q_PROPERTY(double uploadTraffic READ uploadTraffic WRITE setUploadTraffic
               NOTIFY uploadTrafficChanged)
  Q_PROPERTY(double downloadTraffic READ downloadTraffic WRITE
               setDownloadTraffic NOTIFY downloadTrafficChanged)
public:
  explicit NodeList(QObject* parent = nullptr);

  void init(QSharedPointer<LogView> log_view,
            QSharedPointer<across::setting::ConfigTools> config,
            QSharedPointer<across::core::CoreTools> core,
            QSharedPointer<across::DBTools> db);

  QVector<NodeInfo> items();

  void reloadItems();

  void appendNode(NodeInfo node);

  Q_INVOKABLE void removeCurrentNode(int id);

  Q_INVOKABLE QString getQRCode(int id);

public:
  int currentNodeID();

  int currentNodeIndex();

  int currentGroupID();

  int displayGroupID();

  const QString& currentNodeName() const;

  const QString& currentNodeGroup() const;

  QString currentNodeProtocol() const;

  const QString& currentNodeAddress() const;

  int currentNodePort();

  const QString& currentNodeURL() const;

  double uploadTraffic();

  double downloadTraffic();

public slots:
  void setCurrentGroupID(int group_id);

  void setDisplayGroupID(int group_id);

  void setCurrentNode(int id, int index);

  void copyUrlToClipboard(int id);

  void setUploadTraffic(double newUploadTraffic);

  void setDownloadTraffic(double newDownloadTraffic);

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
  void currentNodeURLChanged();

  void updateQRCode(const QString& id, const QString& content);
  void uploadTrafficChanged(double uploadTraffic);
  void downloadTrafficChanged(double downloadTraffic);

private:
  std::shared_ptr<across::utils::LogTools> p_logger;
  std::shared_ptr<across::config::JsonTools> p_json;
  QSharedPointer<DBTools> p_db;
  QSharedPointer<across::core::APITools> p_api;
  QSharedPointer<across::setting::ConfigTools> p_config;
  QSharedPointer<across::core::CoreTools> p_core;

  QVector<NodeInfo> m_items;
  QMap<int, NodesInfo> m_all_items;

  int m_group_id = 1;
  int m_display_group_id = 1;
  NodeInfo m_current_node;
  across::core::TrafficInfo m_traffic_delta;
  across::core::TrafficInfo m_traffic_last = { 0, 0 };
};
}

#endif // NODELIST_H
