#include "apitools.h"
#include <iostream>

using namespace across;
using namespace across::core;

APITools::APITools(uint port)
{
  p_channel = grpc::CreateChannel(LOCAL_HOST + ":" + std::to_string(port),
                                  grpc::InsecureChannelCredentials());

  // create thread
  p_thread = new QThread(this);
  p_worker = new APIWorker(p_channel);
  p_worker->moveToThread(p_thread);

  // control signals
  connect(this, &APITools::operate, p_worker, &APIWorker::start);
  connect(
    p_worker, &APIWorker::trafficChanged, this, &APITools::handleTrafficResult);
}

APITools::~APITools()
{
  if (p_worker) {
    p_worker->stop();
    p_worker->deleteLater();
  }

  // destroy pointer
  if (p_thread) {
    p_thread->quit();
    p_thread->wait();
  }
}

void
APITools::startMonitoring(const QString& tag)
{
  m_tag = tag;

  // start thread process
  p_thread->start();
  emit operate(m_tag);
}

void
APITools::stopMonitoring()
{
  p_worker->stop();
}

void
APITools::restartMonitoring()
{
  startMonitoring(m_tag);
}

std::pair<bool, std::string>
APITools::isOk() const
{
  auto p_stub = StatsService::NewStub(p_channel);
  bool isOk = false;

  std::string err;
  ClientContext context;
  SysStatsRequest request;
  SysStatsResponse response;

  // check response stats
  if (auto stats = p_stub->GetSysStats(&context, request, &response);
      stats.ok() && response.uptime() > 0) {
    isOk = true;
  } else {
    err = std::to_string(stats.error_code()) + ": " + stats.error_message();
  }

  return { isOk, err };
}

void
APITools::handleTrafficResult(const QVariant& data)
{
  emit trafficChanged(data);
}

APIWorker::APIWorker(std::shared_ptr<grpc::Channel> channel)
{
  p_stub = StatsService::NewStub(channel);
}

void
APIWorker::start(const QString& tag)
{
  // use old tag on restart
  if (!tag.isEmpty())
    m_tag = tag;

  // initialize the stop handle
  m_stop = false;

  // lambda
  auto call_api = [&](const QString& type) -> int64_t {
    ClientContext context;
    GetStatsResponse response;
    GetStatsRequest request;

    request.set_name(
      QString("outbound>>>%1>>>traffic>>>%2").arg(m_tag, type).toStdString());
    
    request.set_reset(false);

    if (auto status = p_stub->GetStats(&context, request, &response);
        status.ok()) {
      return response.stat().value();
    }

    return 0;
  };

  while (!m_stop) {
    auto traffic_info = TrafficInfo{
      .upload = call_api("uplink"),
      .download = call_api("downlink"),
    };

    emit trafficChanged(QVariant::fromValue<TrafficInfo>(traffic_info));

    if (traffic_info.upload < 0 || traffic_info.download < 0)
      break;
    else
      QThread::msleep(1000); // once per second
  }
}

void
APIWorker::stop()
{
  this->m_stop = true;
}

void
TrafficInfo::clear()
{
  download = upload = 0;
}
