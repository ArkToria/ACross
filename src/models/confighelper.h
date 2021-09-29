#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include "across.grpc.pb.h"

#include <fstream>
#include <google/protobuf/util/json_util.h>
#include <sstream>
#include <toml++/toml.h>

namespace across {
namespace setting {
class ConfigHelper
{
public:
  static std::string toJson(
    across::config::Config& config,
    google::protobuf::util::JsonPrintOptions options = defaultPrintOptions());

  static across::config::Config fromJson(const std::string& json_string);

  static google::protobuf::util::JsonPrintOptions defaultPrintOptions();

  static across::config::Config defaultConfig();

  static void saveToFile(const std::string& content,
                         const std::string& file_path = "./across.json");

  static std::string readFromFile(const std::string& file_path);

  static std::string JsonToToml(const std::string& json_str);

  static std::string TomlToJson(const std::string& toml_str);
};
}
}

#endif // CONFIGHELPER_H
