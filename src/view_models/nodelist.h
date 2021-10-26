#ifndef NODELIST_H
#define NODELIST_H

#include "../models/apitools.h"
#include "../models/clipboardtools.h"
#include "../models/coretools.h"
#include "../models/dbtools.h"
#include "../models/notifytools.h"
#include "../models/qrcodetools.h"
#include "../models/serializetools.h"

#include "configtools.h"
#include "jsonhighlighter.h"
#include "logtools.h"

#include "magic_enum.hpp"
#include <QObject>
#include <QPointer>
#include <QQuickTextDocument>
#include <QSharedPointer>
#include <QUrl>
#include <QVariant>
#include <QtConcurrent>

namespace across {
using Json = nlohmann::json;

class NodeList : public QObject
{
  Q_OBJECT
  Q_PROPERTY(
    qint64 currentNodeID READ currentNodeID NOTIFY currentNodeIDChanged)
  Q_PROPERTY(
    qint64 currentGroupID READ currentGroupID NOTIFY currentGroupIDChanged)
  Q_PROPERTY(qint64 displayGroupID READ displayGroupID WRITE setDisplayGroupID
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
  Q_PROPERTY(QString currentNodePassword READ currentNodePassword NOTIFY
               currentNodePasswordChanged)
  Q_PROPERTY(
    QString currentNodeURL READ currentNodeURL NOTIFY currentNodeURLChanged)
  Q_PROPERTY(
    QString uploadTraffic READ uploadTraffic NOTIFY uploadTrafficChanged)
  Q_PROPERTY(
    QString downloadTraffic READ downloadTraffic NOTIFY downloadTrafficChanged)
public:
  explicit NodeList(QObject* parent = nullptr);

  ~NodeList();

  void init(QSharedPointer<LogView> log_view,
            QSharedPointer<across::setting::ConfigTools> config,
            QSharedPointer<across::core::CoreTools> core,
            QSharedPointer<across::DBTools> db);

  bool run();
  void setFilter(const QMap<qint64, QList<qint64>>& search_results);
  void clearFilter();
  void clearItems();
  void reloadItems();
  QString generateConfig();
  void appendNode(NodeInfo node);
  void setUploadTraffic(double newUploadTraffic);
  void setDownloadTraffic(double newDownloadTraffic);
  void testLatency(NodeInfo node, int index);

  Q_INVOKABLE void testLatency(int id);
  Q_INVOKABLE void setCurrentNodeByID(int id);
  Q_INVOKABLE void removeNodeByID(int id);
  Q_INVOKABLE QVariantMap getNodeInfoByIndex(int index);
  Q_INVOKABLE QVariantMap getCurrentNodeInfo();
  Q_INVOKABLE QString getQRCode(int id);
  Q_INVOKABLE void saveQRCodeToFile(int id, const QUrl& url);
  Q_INVOKABLE void setAsDefault(int id);
  Q_INVOKABLE void setDocument(QVariant& v);
  Q_INVOKABLE qint64 getIndexByNode(qint64 node_id, qint64 group_id);
  Q_INVOKABLE static QString jsonFormat(const QString& json_str);
  Q_INVOKABLE static void copyURLToClipboard(const QString& node_name,
                                             const QString& node_url);
  Q_INVOKABLE void copyCurrentNodeURLToClipboard();

public:
  QList<NodeInfo> items();
  qint64 currentNodeID();
  qint64 currentGroupID();
  qint64 displayGroupID();
  QString currentNodeName();
  QString currentNodeGroup();
  QString currentNodeProtocol();
  QString currentNodeAddress();
  int currentNodePort();
  QString currentNodePassword();
  QString currentNodeURL();
  QString uploadTraffic();
  QString downloadTraffic();

public slots:
  void setDisplayGroupID(int group_id);
  void handleLatencyChanged(qint64 group_id, int index, NodeInfo node);

signals:
  void itemReset(int index);
  void itemLatencyChanged(qint64 group_id, int index, NodeInfo node);

  void preItemsReset();
  void postItemsReset();
  void groupSizeChanged(qint64 group_id, int size);

  void preItemAppended();
  void postItemAppended();

  void preItemRemoved(int index);
  void postItemRemoved();

  void currentNodeIDChanged();
  void currentGroupIDChanged();
  void displayGroupIDChanged();
  void currentNodeNameChanged();
  void currentNodeGroupChanged();
  void currentNodeProtocolChanged();
  void currentNodeAddressChanged();
  void currentNodePortChanged();
  void currentNodePasswordChanged();
  void currentNodeURLChanged();

  void nodeLatencyChanged(int id, const QString& group, int latency);

  void updateQRCode(const QString& id, const QString& content);
  void uploadTrafficChanged(const QString& uploadTraffic);
  void downloadTrafficChanged(const QString& downloadTraffic);

private:
  std::shared_ptr<across::utils::LogTools> p_logger;
  QSharedPointer<DBTools> p_db;
  QSharedPointer<across::core::APITools> p_api;
  QSharedPointer<across::setting::ConfigTools> p_config;
  QSharedPointer<across::core::CoreTools> p_core;

  QQueue<QFuture<void>> work_tasks;

  across::jsonHighlighter jsonHighlighter;

  QMap<qint64, QList<qint64>> m_search_results;
  int m_display_group_id = 1;

  NodeInfo m_node;
  QList<NodeInfo> m_nodes;
  QList<NodeInfo> m_origin_nodes;
  across::core::TrafficInfo m_traffic = { 0, 0 };
  across::core::TrafficInfo m_traffic_last = { 0, 0 };
  across::core::TrafficInfo m_traffic_last_rate = { 0, 0 };
  across::core::TrafficInfo m_traffic_sum = { 0, 0 };
};
}

#endif // NODELIST_H
