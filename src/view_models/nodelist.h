#ifndef NODELIST_H
#define NODELIST_H

#include "../models/clipboardtools.h"
#include "../models/apitools.h"
#include "../models/coretools.h"
#include "../models/dbtools.h"
#include "../models/jsontools.h"
#include "../models/notifytools.h"
#include "../models/qrcodetools.h"

#include "configtools.h"
#include "logtools.h"

#include "magic_enum.hpp"
#include <QObject>
#include <QPointer>
#include <QSharedPointer>
#include <QUrl>
#include <QVariant>
#include <QtConcurrent>

namespace across {
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

  void init(QSharedPointer<LogView> log_view,
            QSharedPointer<across::setting::ConfigTools> config,
            QSharedPointer<across::core::CoreTools> core,
            QSharedPointer<across::DBTools> db);

  bool run();
  void reloadItems();
  Json::Value generateConfig();
  void appendNode(NodeInfo node);
  void setUploadTraffic(double newUploadTraffic);
  void setDownloadTraffic(double newDownloadTraffic);
  void setLatency(int id);

  Q_INVOKABLE void setCurrentNodeByID(int id);
  Q_INVOKABLE void removeNodeByID(int id);
  Q_INVOKABLE QString getQRCode(int id);
  Q_INVOKABLE void saveQRCodeToFile(int id, const QUrl& url);
  Q_INVOKABLE void setAsDefault(int id);
  Q_INVOKABLE void setAvgLatency(int id);

public:
  QVector<NodeInfo> items();
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
  void copyUrlToClipboard(int id);

signals:
  void itemsSizeChanged(qint64 group_id, int size);

  void preItemsReset();
  void postItemsReset();

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

  void nodeLatencyChanged(int id,const QString &group,int latency);

  void updateQRCode(const QString& id, const QString& content);
  void uploadTrafficChanged(const QString& uploadTraffic);
  void downloadTrafficChanged(const QString& downloadTraffic);

private:
  std::shared_ptr<across::utils::LogTools> p_logger;
  std::shared_ptr<across::config::JsonTools> p_json;
  QSharedPointer<DBTools> p_db;
  QSharedPointer<across::core::APITools> p_api;
  QSharedPointer<across::setting::ConfigTools> p_config;
  QSharedPointer<across::core::CoreTools> p_core;

  QVector<NodeInfo> m_nodes;
  NodeInfo m_node;

  int m_display_group_id = 1;
  across::core::TrafficInfo m_traffic = { 0, 0 };
  across::core::TrafficInfo m_traffic_last = { 0, 0 };
  across::core::TrafficInfo m_traffic_last_rate = { 0, 0 };
  across::core::TrafficInfo m_traffic_sum = { 0, 0 };
};
}

#endif // NODELIST_H
