#ifndef APITOOLS_H
#define APITOOLS_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QVariant>

#include <atomic>
#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include <vector>

#include "v2ray_api.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using v2ray::core::app::stats::command::GetStatsRequest;
using v2ray::core::app::stats::command::GetStatsResponse;
using v2ray::core::app::stats::command::QueryStatsRequest;
using v2ray::core::app::stats::command::QueryStatsResponse;
using v2ray::core::app::stats::command::Stat;
using v2ray::core::app::stats::command::StatsService;
using v2ray::core::app::stats::command::SysStatsRequest;
using v2ray::core::app::stats::command::SysStatsResponse;

namespace across {
namespace core {
struct TrafficInfo
{
  int64_t upload = -1;
  int64_t download = -1;
};

class APIWorker : public QObject
{
  Q_OBJECT
public:
  APIWorker(std::shared_ptr<grpc::Channel> channel);

public slots:
  void start(const QString& tag);

  void stop();

signals:
  void trafficChanged(const QVariant& data);

private:
  bool m_stop = false;
  QString m_tag = "ACROSS_INBOUND_API";
  std::unique_ptr<StatsService::Stub> p_stub;
};

class APITools : public QObject
{
  Q_OBJECT
public:
  explicit APITools(uint port);

  ~APITools();

  void startMonitoring(const QString& tag);

  void stopMonitoring();

  void restartMonitoring();

  std::pair<bool, std::string> isOk() const;

public slots:
  void handleTrafficResult(const QVariant& data);

signals:
  void operate(const QString& tag);

  void trafficChanged(const QVariant& data);

private:
  std::shared_ptr<Channel> p_channel;
  QThread* p_thread = nullptr;
  APIWorker* p_worker = nullptr;
  QString m_tag;
};
}
}

#endif // APITOOLS_H
