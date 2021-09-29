#include "confighelper.h"

using namespace google::protobuf::util;
using namespace across;
using namespace across::setting;

ConfigHelper::ConfigHelper()
{
  this->save();
}

std::string
ConfigHelper::toJson()
{
  std::string json_string;

  MessageToJsonString(m_config, &json_string, m_options);

  return json_string;
}

void
ConfigHelper::save(const std::string& file_path)
{
  ConfigHelper::saveToFile(this->toJson(), file_path);
}

JsonPrintOptions
ConfigHelper::defaultPrintOptions()
{
  google::protobuf::util::JsonPrintOptions options;
  options.add_whitespace = true;
  options.always_print_primitive_fields = true;
  options.preserve_proto_field_names = true;

  return options;
}

config::Config
ConfigHelper::defaultConfig()
{
  across::config::Config config;
  config.set_title("GRPC Config Test");
  config.set_config_version("0.1.0");
  config.set_toml_version("1.0.0");

  if (auto interface = config.mutable_interface(); config.has_interface()) {
    interface->set_language("en_US");

    if (auto theme = interface->mutable_theme(); interface->has_theme()) {
      theme->set_theme("default-light");
      theme->set_include_dir("$HOME/.local/share/across/themes/");
    }

    if (auto tray = interface->mutable_tray(); interface->has_tray()) {
      tray->set_enable(false);
      tray->set_close_to_minimize(false);
    }
  }

  if (auto update = config.mutable_update(); config.has_update()) {
    update->set_auto_update(false);
    update->set_check_update(true);
    update->set_update_channel("stable");
    update->set_update_from_proxy(true);
  }

  return config;
}

void
ConfigHelper::saveToFile(const std::string& content,
                         const std::string& file_path)
{
  std::ofstream file(file_path);
  file << content;
  file.close();
}
