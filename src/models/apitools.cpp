#include "apitools.h"
#include <iostream>

using namespace across::core;

APITools::APITools(std::shared_ptr<grpc::Channel> channel)
{
  p_stub = StatsService::NewStub(channel);
}

APITools::APITools(uint port)
{
  auto channel = grpc::CreateChannel("127.0.0.1:" + std::to_string(port),
                                     grpc::InsecureChannelCredentials());

  p_stub = StatsService::NewStub(channel);
}

std::pair<bool, std::string>
APITools::isOk() const
{
  bool isOk = false;
  std::string err;
  ClientContext context;
  SysStatsRequest request;
  SysStatsResponse response;

  if (auto stats = p_stub->GetSysStats(&context, request, &response);
      stats.ok()) {
    if (response.uptime() > 0) {
      isOk = true;
    }
  } else {
    isOk = false;
    err = std::to_string(stats.error_code()) + ": " + stats.error_message();
  }

  return std::make_pair(isOk, err);
}

std::pair<int64_t, int64_t>
APITools::getTraffic(const QString& tag)
{
  auto call_api = [&tag, this](const QString& type) -> int64_t {
    ClientContext context;
    GetStatsResponse response;
    GetStatsRequest request;
    request.set_name(
      QString("inbound>>>%1>>>traffic>>>%2").arg(tag, type).toStdString());
    request.set_reset(false);

    if (auto status = p_stub->GetStats(&context, request, &response);
        status.ok()) {
      return response.stat().value();
    }

    return 0;
  };

  return { call_api("uplink"), call_api("downlink") };
}
