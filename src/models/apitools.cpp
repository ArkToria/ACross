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

  Status status = p_stub->GetSysStats(&context, request, &response);

  if (status.ok()) {
    if (response.uptime() > 0) {
      isOk = true;
    }
  } else {
    isOk = false;
    err = std::to_string(status.error_code()) + ": " + status.error_message();
  }

  return std::make_pair(isOk, err);
}
