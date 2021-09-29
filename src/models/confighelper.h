#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include "across.grpc.pb.h"

#include <fstream>
#include <sstream>
#include <google/protobuf/util/json_util.h>

namespace across {
namespace setting {
class ConfigHelper
{
public:
  ConfigHelper();

  std::string toJson();

  across::config::Config fromJson(const std::string& json_string);

  void save(const std::string& file_path = "./across.json");

  static google::protobuf::util::JsonPrintOptions defaultPrintOptions();

  static across::config::Config defaultConfig();

  static void saveToFile(const std::string& content,
                         const std::string& file_path = "./across.json");

private:
  google::protobuf::util::JsonPrintOptions m_options = defaultPrintOptions();
  across::config::Config m_config = defaultConfig();
};
}
}

#endif // CONFIGHELPER_H
