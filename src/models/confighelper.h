#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include "across.grpc.pb.h"
#include "serializetools.h"

#include <QFile>

namespace across {
namespace setting {
class ConfigHelper
{
public:
  static across::config::Config defaultConfig();

  static void saveToFile(const std::string& content,
                         const QString& file_path = "./across.json");

  static std::string readFromFile(const QString& file_path);
};
}
}

#endif // CONFIGHELPER_H
