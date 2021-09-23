#include "configtools.h"

using namespace across::setting;
using namespace across::core;
using namespace across::config;
using namespace across::utils;

template<typename T>
void
Table::fromNodeView(toml::v2::node_view<toml::node> node,
                    const toml::v2::node_view<toml::node>& default_config,
                    T& config,
                    const std::string& key)
{
  using U = std::conditional_t<std::is_same_v<T, QString>, std::string, T>;
  if (!node[key].value<U>().has_value())
    node.as_table()->insert(key, default_config[key]);
  if (auto temp = node[key].value<U>(); temp.has_value()) {
    if constexpr (std::is_same_v<T, QString>)
      config = QString::fromStdString(temp.value());
    else
      config = temp.value();
  }
}
template<typename T>
void
Table::toNodeView(toml::v2::node_view<toml::node> node,
                  const toml::v2::node_view<toml::node>& default_config,
                  const T& config,
                  const std::string& key)
{
  using U = std::conditional_t<std::is_same_v<T, QString>, std::string, T>;
  if (!node[key].value<U>().has_value())
    node.as_table()->insert(key, default_config[key]);

  if constexpr (std::is_same_v<T, QString>)
    *node[key].as_string() = config.toStdString();
  else if constexpr (std::is_same_v<T, bool>)
    *node[key].as_boolean() = config;
  else if constexpr (std::is_same_v<T, uint>)
    *node[key].as_integer() = config;
  else if constexpr (std::is_same_v<T, int>)
    *node[key].as_integer() = config;
}

void
Interface::Language::fromNodeView(
  toml::v2::node_view<toml::node> language,
  const toml::v2::node_view<toml::node>& default_config)
{
  if (auto temp = *language.value<std::string>(); !temp.empty())
    this->language = QString::fromStdString(temp);
}

void
Interface::Language::toNodeView(
  const toml::v2::node_view<toml::node>& language,
  const toml::v2::node_view<toml::node>& default_config)
{
  *language.as_string() = this->language.toStdString();
}

void
Interface::Theme::fromNodeView(
  toml::v2::node_view<toml::node> theme,
  const toml::v2::node_view<toml::node>& default_config,
  QString& config,
  const std::string& key)
{
  if (!theme[key].value<std::string>().has_value())
    theme.as_table()->insert(key, default_config[key]);
  if (auto temp = theme[key].value<std::string>(); temp.has_value())
    config = QString::fromStdString(temp.value());
}
void
Interface::Theme::fromNodeView(
  toml::v2::node_view<toml::node> theme,
  const toml::v2::node_view<toml::node>& default_config)
{
  fromNodeView(theme, default_config, this->theme, "theme");

  fromNodeView(theme, default_config, this->include_dir, "include_dir");
}

void
Interface::Theme::toNodeView(
  toml::v2::node_view<toml::node> theme,
  const toml::v2::node_view<toml::node>& default_config,
  const QString& config,
  const std::string& key)
{
  auto& table = *theme.as_table();
  if (!theme[key].value<std::string>().has_value())
    table.insert(key, default_config[key]);
  *theme[key].as_string() = config.toStdString();
}

void
Interface::Theme::toNodeView(
  const toml::v2::node_view<toml::node>& theme,
  const toml::v2::node_view<toml::node>& default_config)
{
  toNodeView(theme, default_config, this->theme, "theme");
  toNodeView(theme, default_config, this->include_dir, "include_dir");
}

void
Interface::Tray::fromNodeView(
  toml::v2::node_view<toml::node> theme,
  const toml::v2::node_view<toml::node>& default_config,
  bool& config,
  const std::string& key)
{
  if (!theme[key].value<bool>().has_value())
    theme.as_table()->insert(key, default_config[key]);
  if (auto temp = theme[key].value<bool>(); temp.has_value())
    config = temp.value();
}

void
Interface::Tray::fromNodeView(
  toml::v2::node_view<toml::node> tray,
  const toml::v2::node_view<toml::node>& default_config)
{
  fromNodeView(tray, default_config, this->enable, "enable");
}

void
Interface::Tray::toNodeView(
  toml::v2::node_view<toml::node> tray,
  const toml::v2::node_view<toml::node>& default_config,
  const bool& config,
  const std::string& key)
{
  if (!tray[key].value<bool>().has_value())
    tray.as_table()->insert(key, default_config[key]);
  *tray[key].as_boolean() = config;
}

void
Interface::Tray::toNodeView(
  const toml::v2::node_view<toml::node>& tray,
  const toml::v2::node_view<toml::node>& default_config)
{
  toNodeView(tray, default_config, this->enable, "enable");
}

template<>
void
Interface::fromNodeView(toml::v2::node_view<toml::node> interface,
                        const toml::v2::node_view<toml::node>& default_config,
                        Language& config,
                        const std::string& key)
{
  if (!interface[key].value<std::string>().has_value())
    interface.as_table()->insert(key, default_config[key]);
  if (auto temp = interface[key].value<std::string>(); temp.has_value())
    config.fromNodeView(interface[key], default_config[key]);
}

void
Interface::fromNodeView(toml::v2::node_view<toml::node> interface,
                        const toml::v2::node_view<toml::node>& default_config)
{
  fromNodeView(interface, default_config, this->language, "language");
  fromNodeView(interface, default_config, this->theme, "theme");
  fromNodeView(interface, default_config, this->tray, "tray");
}

template<typename T>
void
Interface::fromNodeView(toml::v2::node_view<toml::node> interface,
                        const toml::v2::node_view<toml::node>& default_config,
                        T& config,
                        const std::string& key)
{
  if (interface[key].as_table() == nullptr)
    interface.as_table()->insert(key, default_config[key]);
  config.fromNodeView(interface[key], default_config[key]);
}

template<>
void
Interface::toNodeView(toml::v2::node_view<toml::node> interface,
                      const toml::v2::node_view<toml::node>& default_config,
                      Language& config,
                      const std::string& key)
{
  if (interface[key].as_string() == nullptr)
    interface.as_table()->insert(key, default_config[key]);
  config.toNodeView(interface[key], default_config[key]);
}

void
Interface::toNodeView(const toml::v2::node_view<toml::node>& interface,
                      const toml::v2::node_view<toml::node>& default_config)
{
  toNodeView(interface, default_config, this->language, "language");
  toNodeView(interface, default_config, this->theme, "theme");
  toNodeView(interface, default_config, this->tray, "tray");
}

template<typename T>
void
Interface::toNodeView(toml::v2::node_view<toml::node> interface,
                      const toml::v2::node_view<toml::node>& default_config,
                      T& config,
                      const std::string& key)
{
  if (interface[key].as_table() == nullptr)
    interface.as_table()->insert(key, default_config[key]);
  config.toNodeView(interface[key], default_config[key]);
}

void
Network::fromNodeView(toml::v2::node_view<toml::node> network,
                      const toml::v2::node_view<toml::node>& default_config)
{
  Table::fromNodeView(
    network, default_config, this->test_method, "test_method");
  Table::fromNodeView(network, default_config, this->test_url, "test_url");
  Table::fromNodeView(network, default_config, this->user_agent, "user_agent");
}

void
Network::toNodeView(const toml::v2::node_view<toml::node>& network,
                    const toml::v2::node_view<toml::node>& default_config)
{
  Table::toNodeView(network, default_config, this->test_method, "test_method");
  Table::toNodeView(network, default_config, this->test_url, "test_url");
  Table::toNodeView(network, default_config, this->user_agent, "user_agent");
}

void
Update::fromNodeView(toml::v2::node_view<toml::node> update,
                     const toml::v2::node_view<toml::node>& default_config)
{
  Table::fromNodeView(update, default_config, this->auto_update, "auto_update");
  Table::fromNodeView(
    update, default_config, this->check_update, "check_update");
  Table::fromNodeView(
    update, default_config, this->update_from_proxy, "update_from_proxy");
  Table::fromNodeView(
    update, default_config, this->update_channel, "update_channel");
}

void
Update::toNodeView(const toml::v2::node_view<toml::node>& update,
                   const toml::v2::node_view<toml::node>& default_config)
{
  Table::toNodeView(update, default_config, this->auto_update, "auto_update");
  Table::toNodeView(update, default_config, this->check_update, "check_update");
  Table::toNodeView(
    update, default_config, this->update_channel, "update_channel");
  Table::toNodeView(
    update, default_config, this->update_from_proxy, "update_from_proxy");
}

void
DataBase::fromNodeView(toml::v2::node_view<toml::node> database,
                       const toml::v2::node_view<toml::node>& default_config)
{
  Table::fromNodeView(database, default_config, this->backend, "db_backend");
  Table::fromNodeView(database, default_config, this->path, "db_path");

  if (database["auth"].as_table() == nullptr)
    database.as_table()->insert("auth", default_config["auth"]);

  Table::fromNodeView(
    database["auth"], default_config["auth"], this->auth.enable, "enable");

  if (this->auth.enable) {
    Table::fromNodeView(database["auth"],
                        default_config["auth"],
                        this->auth.username,
                        "username");
    Table::fromNodeView(database["auth"],
                        default_config["auth"],
                        this->auth.password,
                        "password");
    Table::fromNodeView(
      database["auth"], default_config["auth"], this->auth.address, "address");
    Table::fromNodeView(
      database["auth"], default_config["auth"], this->auth.port, "port");
  }
}

void
DataBase::toNodeView(const toml::v2::node_view<toml::node>& database,
                     const toml::v2::node_view<toml::node>& default_config)
{
  Table::toNodeView(database, default_config, this->path, "db_path");
  Table::toNodeView(database, default_config, this->backend, "db_backend");

  if (database["auth"].as_table() == nullptr)
    database.as_table()->insert("auth", default_config["auth"]);

  if (this->auth.enable) {
    Table::toNodeView(
      database["auth"], default_config["auth"], this->auth.enable, "enable");
    Table::toNodeView(database["auth"],
                      default_config["auth"],
                      this->auth.username,
                      "username");
    Table::toNodeView(database["auth"],
                      default_config["auth"],
                      this->auth.password,
                      "password");
    Table::toNodeView(
      database["auth"], default_config["auth"], this->auth.address, "address");
    Table::toNodeView(
      database["auth"], default_config["auth"], this->auth.port, "port");
  }
}

void
Core::fromNodeView(toml::v2::node_view<toml::node> core,
                   const toml::v2::node_view<toml::node>& default_config)
{
  Table::fromNodeView(core, default_config, this->core_path, "core_path");
  Table::fromNodeView(core, default_config, this->assets_path, "assets_path");
  Table::fromNodeView(core, default_config, this->log_level, "log_level");
  Table::fromNodeView(core, default_config, this->log_lines, "log_lines");

  if (core["api"].as_table() == nullptr)
    core.as_table()->insert("api", default_config["api"]);

  Table::fromNodeView(
    core["api"], default_config["api"], this->api.enable, "enable");
  if (this->api.enable) {
    Table::fromNodeView(
      core["api"], default_config["api"], this->api.port, "port");
  }
}

void
Core::toNodeView(const toml::v2::node_view<toml::node>& core,
                 const toml::v2::node_view<toml::node>& default_config)
{
  Table::toNodeView(core, default_config, this->core_path, "core_path");
  Table::toNodeView(core, default_config, this->assets_path, "assets_path");
  Table::toNodeView(core, default_config, this->log_level, "log_level");
  Table::toNodeView(core, default_config, this->log_lines, "log_lines");

  if (core["api"].as_table() == nullptr)
    core.as_table()->insert("api", default_config["api"]);

  Table::toNodeView(
    core["api"], default_config["api"], this->api.enable, "enable");
  Table::toNodeView(core["api"], default_config["api"], this->api.port, "port");
}

void
Theme::Colors::fromNodeView(toml::v2::node_view<toml::node> colors_node)
{
  if (auto temp = colors_node["text_color"].value<std::string>();
      temp.has_value())
    this->text_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["background_color"].value<std::string>();
      temp.has_value())
    this->background_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["highlight_color"].value<std::string>();
      temp.has_value())
    this->highlight_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["highlight_text_color"].value<std::string>();
      temp.has_value())
    this->highlight_text_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["warn_color"].value<std::string>();
      temp.has_value())
    this->warn_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["warn_text_color"].value<std::string>();
      temp.has_value())
    this->warn_text_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["shadow_color"].value<std::string>();
      temp.has_value())
    this->shadow_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["border_color"].value<std::string>();
      temp.has_value())
    this->border_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["deep_color"].value<std::string>();
      temp.has_value())
    this->deep_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["deep_text_color"].value<std::string>();
      temp.has_value())
    this->deep_text_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["style_color"].value<std::string>();
      temp.has_value())
    this->style_color = QString::fromStdString(temp.value());

  if (auto temp = colors_node["style_text_color"].value<std::string>();
      temp.has_value())
    this->style_text_color = QString::fromStdString(temp.value());
}

void
InboundSettings::SOCKS::fromNodeView(
  toml::v2::node_view<toml::node> socks,
  const toml::v2::node_view<toml::node>& default_config)
{
  Table::fromNodeView(socks, default_config, this->enable, "enable");
  if (this->enable) {
    Table::fromNodeView(socks, default_config, this->listen, "listen");
    Table::fromNodeView(socks, default_config, this->port, "port");
    Table::fromNodeView(socks, default_config, this->udp, "udp");
    Table::fromNodeView(socks, default_config, this->ip, "ip");
    Table::fromNodeView(socks, default_config, this->user_level, "user_level");

    if (socks["auth"].as_table() == nullptr)
      socks.as_table()->insert("auth", default_config["auth"]);

    Table::fromNodeView(
      socks["auth"], default_config["auth"], this->username, "username");
    Table::fromNodeView(
      socks["auth"], default_config["auth"], this->password, "password");
  }
}

void
InboundSettings::SOCKS::toNodeView(
  const toml::v2::node_view<toml::node>& socks,
  const toml::v2::node_view<toml::node>& default_config)
{
  Table::toNodeView(socks, default_config, this->enable, "enable");
  Table::toNodeView(socks, default_config, this->listen, "listen");
  Table::toNodeView(socks, default_config, this->port, "port");
  Table::toNodeView(socks, default_config, this->udp, "udp");
  Table::toNodeView(socks, default_config, this->ip, "ip");
  Table::toNodeView(socks, default_config, this->user_level, "user_level");

  if (socks["auth"].as_table() == nullptr)
    socks.as_table()->insert("auth", default_config["auth"]);

  Table::toNodeView(
    socks["auth"], default_config["auth"], this->username, "username");
  Table::toNodeView(
    socks["auth"], default_config["auth"], this->password, "password");
}

InboundObject
InboundSettings::SOCKS::toInboundObject()
{
  InboundObject inbound_object;

  inbound_object.listen = this->listen.toStdString();
  inbound_object.port = std::to_string(this->port);
  inbound_object.protocol = this->protocol.toStdString();

  SocksObject::InboundSettingObject inbound_setting_object;

  if (!this->username.isEmpty()) {
    SocksObject::AccountObject account_object;

    account_object.username = this->username.toStdString();
    account_object.password = this->password.toStdString();

    inbound_setting_object.appendAccountObject(account_object);
  }

  inbound_setting_object.ip = this->ip.toStdString();
  inbound_setting_object.udp = this->udp;
  inbound_setting_object.user_level = this->user_level;

  inbound_object.settings = inbound_setting_object.toObject();

  return inbound_object;
}

void
InboundSettings::HTTP::fromNodeView(
  toml::v2::node_view<toml::node> http,
  const toml::v2::node_view<toml::node>& default_config)
{
  Table::fromNodeView(http, default_config, this->enable, "enable");
  if (this->enable) {
    Table::fromNodeView(http, default_config, this->listen, "listen");
    Table::fromNodeView(http, default_config, this->port, "port");
    Table::fromNodeView(
      http, default_config, this->allow_transparent, "allow_transparent");
    Table::fromNodeView(http, default_config, this->timeout, "timeout");
    Table::fromNodeView(http, default_config, this->user_level, "user_level");

    if (http["auth"].as_table() == nullptr)
      http.as_table()->insert("auth", default_config["auth"]);

    Table::fromNodeView(
      http["auth"], default_config["auth"], this->username, "username");
    Table::fromNodeView(
      http["auth"], default_config["auth"], this->password, "password");
  }
}

void
InboundSettings::HTTP::toNodeView(
  const toml::v2::node_view<toml::node>& http,
  const toml::v2::node_view<toml::node>& default_config)
{
  Table::toNodeView(http, default_config, this->enable, "enable");
  Table::toNodeView(http, default_config, this->listen, "listen");
  Table::toNodeView(http, default_config, this->port, "port");
  Table::toNodeView(
    http, default_config, this->allow_transparent, "allow_transparent");
  Table::toNodeView(http, default_config, this->timeout, "timeout");
  Table::toNodeView(http, default_config, this->user_level, "user_level");

  if (http["auth"].as_table() == nullptr)
    http.as_table()->insert("auth", default_config["auth"]);

  Table::toNodeView(
    http["auth"], default_config["auth"], this->username, "username");
  Table::toNodeView(
    http["auth"], default_config["auth"], this->password, "password");
}

InboundObject
InboundSettings::HTTP::toInboundObject()
{
  InboundObject inbound_object;

  inbound_object.listen = this->listen.toStdString();
  inbound_object.port = std::to_string(this->port);
  inbound_object.protocol = this->protocol.toStdString();

  HttpObject::InboundSettingObject inbound_setting_object;

  if (!this->username.isEmpty()) {
    HttpObject::AccountObject account_object;

    account_object.username = this->username.toStdString();
    account_object.password = this->password.toStdString();

    inbound_setting_object.appendAccountObject(account_object);
  }

  inbound_setting_object.timeout = this->timeout;
  inbound_setting_object.allow_transparent = this->allow_transparent;
  inbound_setting_object.user_level = this->user_level;

  inbound_object.settings = inbound_setting_object.toObject();

  return inbound_object;
}

void
InboundSettings::fromNodeView(
  toml::v2::node_view<toml::node> inbound,
  const toml::v2::node_view<toml::node>& default_config)
{
  socks.fromNodeView(inbound["socks"], default_config["socks"]);
  http.fromNodeView(inbound["http"], default_config["http"]);
}

void
InboundSettings::toNodeView(
  const toml::v2::node_view<toml::node>& inbound,
  const toml::v2::node_view<toml::node>& default_config)
{
  socks.toNodeView(inbound["socks"], default_config["socks"]);
  http.toNodeView(inbound["http"], default_config["http"]);
}

void
InboundSettings::setObject(Json::Value& root)
{
  InboundObjects inbound_objects;

  if (socks.enable) {
    inbound_objects.appendInboundObject(socks.toInboundObject());
  }

  if (http.enable) {
    inbound_objects.appendInboundObject(http.toInboundObject());
  }

  inbound_objects.setObject(root);
}

ConfigTools::ConfigTools(QObject* parent) {}

bool
ConfigTools::init(QSharedPointer<LogView> log_view, const QString& file_path)
{
  // initial config logger
  p_logger = std::make_shared<LogTools>(log_view, "setting");

  QString config_path = loadConfigPath(file_path);

  connect(
    this, &ConfigTools::configChanged, this, [&]() { this->saveConfig(); });

  QFile example_file(":/misc/across_example.toml");
  if (!example_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    p_logger->error("Failed to read build-in config file");
    return false;
  }

  if (auto default_config_content = example_file.readAll();
      !default_config_content.isEmpty()) {
    this->m_default_config = toml::parse(default_config_content.toStdString());
  } else {
    p_logger->warn("Build-in config is empty");
    return false;
  }

  if (config_path.isEmpty()) {
    p_logger->warn("Failed to load config path, use build-in config instead");
  } else {
    this->m_config_path = config_path;

    this->m_config = toml::parse_file(config_path.toStdString());
    if (m_config.empty()) {
      p_logger->error("Failed to parse config, use build-in config instead");

      for (auto i : m_default_config) {
        m_config.insert(i.first, i.second);
        printf("%s\n", i.first.c_str());
      }
    }
  }

  if (!loadCoreConfig()) {
    p_logger->error("Failed to load core config");
  }

  if (!loadDBConfig()) {
    p_logger->error("Failed to load database config");
  }

  if (!loadInterfaceConfig()) {
    p_logger->error("Failed to load interface config");
  }

  if (!loadThemeConfig()) {
    p_logger->error("Failed to load colors config");
  }

  if (!loadInboundConfig()) {
    p_logger->error("Failed to load inbounds config");
  }

  emit configChanged();

  return true;
}

QString
ConfigTools::loadConfigPath(const QString& file_path)
{
  QString config_path = "./across.toml";

  // Setting Path > Env Path > Current Path > Default Path
  do {
    if (isFileExist(file_path)) {
      config_path = file_path;
      break;
    }

    auto env_path = m_envs.getInfo().ACROSS_CONFIG_PATH;
    if (!env_path.isEmpty()) {
      config_path = env_path;

      if (isFileExist(config_path)) {
        break;
      }
    }

    auto env_config_home = m_envs.get("XDG_CONFIG_HOME");
    if (env_config_home.isEmpty()) {
      env_config_home = m_envs.get("HOME").append("/.config");
    }

    QDir xdg_path(env_config_home + "/across/");
    auto temp_config = xdg_path.filePath(m_config_name);
    if (isFileExist(temp_config)) {
      config_path = temp_config;
      break;
    } else {
      // create directory and example config file
      if (!xdg_path.exists()) {
        xdg_path.mkdir(xdg_path.path());
      }

      QFile config_file(temp_config);
      if (!config_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        break;
      }

      QFile example_config(":/misc/across_example.toml");
      if (!example_config.open(QIODevice::ReadOnly | QIODevice::Text)) {
        break;
      }

      config_file.write(example_config.readAll());
      config_file.close();
      config_path = temp_config;

      p_logger->info("Generate new config on: {}", temp_config.toStdString());
    }
  } while (false);

  return config_path;
}

bool
ConfigTools::loadInterfaceConfig()
{
  bool result = false;

  do {
    result = true;

    if (m_config.empty() || !m_config["interface"].is_table()) {
      m_config.insert_or_assign("interface",
                                *m_default_config["interface"].as_table());
      result = false;
    }

    m_interface.fromNodeView(m_config["interface"],
                             m_default_config["interface"]);

  } while (false);

  return result;
}

bool
ConfigTools::loadInterfaceTheme()
{
  bool result = false;

  do {
    if (m_config.empty() || !m_config["interface"]["theme"].is_table()) {
      break;
    }

    if (m_config["interface"]["theme"].as_table() == nullptr)
      m_config["interface"].as_table()->insert(
        "theme", m_default_config["interface"]["theme"]);
    m_interface.theme.fromNodeView(m_config["interface"]["theme"],
                                   m_default_config["interface"]["theme"]);

    result = true;
  } while (false);

  return result;
}

bool
ConfigTools::loadInterfaceLanguage()
{
  if (m_config.empty())
    return false;

  m_interface.fromNodeView(m_config["interface"],
                           m_default_config["interface"]);
  return true;
}

bool
ConfigTools::loadUpdateConfig()
{
  bool result = false;

  do {
    if (m_config.empty() || !m_config["update"].is_table()) {
      break;
    }

    m_update.fromNodeView(m_config["update"], m_default_config["update"]);

    result = true;
  } while (false);

  return result;
}

bool
ConfigTools::loadDBConfig()
{
  if (!m_config["database"].is_table()) {
    m_config.insert("database", *m_default_config["database"].as_table());
  }

  if (auto temp = m_config["database"]["db_path"].value<std::string>();
      temp.has_value() && temp != m_db.path.toStdString()) {
    setDBPath(QString::fromStdString(temp.value()), true);

    return true;
  } else {
    p_logger->warn("Failed to get the db_path, use default path `{}`",
                   m_db.path.toStdString());
  }

  return isFileExist(m_db.path);
}

bool
ConfigTools::loadThemeConfig()
{
  auto themes = m_config["themes"];
  if (!themes.as_array() || themes.as_array()->size() == 0) {
    m_config.as_table()->insert("themes", m_default_config["themes"]);
    themes = m_config["themes"];
  }

  if (themes.as_array()->empty()) {
    p_logger->error("No themes configuration");
    return false;
  }

  for (int i = 0; i < themes.as_array()->size(); ++i) {
    if (auto temp = themes[i]["name"].value<std::string>(); temp.has_value()) {
      if (auto temp_name = QString::fromStdString(temp.value());
          m_interface.theme.theme == temp_name) {
        m_theme.name = temp_name;

        if (auto theme_temp = themes[i]["tray"]["stylish"].value<std::string>();
            theme_temp.has_value())
          m_theme.tray.stylish = QString::fromStdString(theme_temp.value());

        if (auto theme_temp = themes[i]["tray"]["color"].value<std::string>();
            theme_temp.has_value())
          m_theme.tray.color = QString::fromStdString(theme_temp.value());

        if (auto theme_temp = themes[i]["border"]["radius"].value<int>();
            theme_temp.has_value())
          m_theme.border.radius = theme_temp.value();

        if (auto theme_temp = themes[i]["border"]["width"].value<int>();
            theme_temp.has_value())
          m_theme.border.width = theme_temp.value();

        if (auto theme_temp = themes[i]["item"]["spacing"].value<int>();
            theme_temp.has_value())
          m_theme.item.spacing = theme_temp.value();

        if (auto theme_temp = themes[i]["icon"]["style"].value<std::string>();
            theme_temp.has_value())
          m_theme.icon.style = QString::fromStdString(theme_temp.value());

        m_theme.colors.fromNodeView(themes[i]["colors"]);
        return true;
      }
    }
  }

  m_config["interface"].as_table()->insert(
    "theme", *m_default_config["interface"]["theme"].as_table());

  return false;
}

bool
ConfigTools::loadCoreConfig()
{
  bool result = false;
  auto envs_info = m_envs.getInfo();

  do {
    result = true;

    if (m_config.empty() || !m_config["core"].is_table()) {
      m_config.insert_or_assign("core", *m_default_config["core"].as_table());
      result = false;
    }

    m_core.fromNodeView(m_config["core"], m_default_config["core"]);

    if (m_core.core_path.isEmpty()) {
      if (!envs_info.V2RAY_CORE_PATH.isEmpty()) {
        m_core.core_path = envs_info.V2RAY_CORE_PATH;
      } else {
        m_core.core_path = "/usr/bin/v2ray";
      }
    }

    if (!isFileExist(m_core.core_path)) {
      p_logger->error("Core file doesn't exist");
      m_core.core_path = ""; // reset the core path
      break;
    }

    if (m_core.assets_path.isEmpty()) {
      if (!envs_info.V2RAY_ASSETS_PATH.isEmpty()) {
        m_core.assets_path = envs_info.V2RAY_ASSETS_PATH;
      } else {
        m_core.assets_path = "/usr/share/v2ray";
      }
    }

    if (!isFileExist(m_core.assets_path + "/geosite.dat") ||
        !isFileExist(m_core.assets_path + "/geoip.dat")) {
      p_logger->error("Geographic files doesn't exist");
      break;
    }

  } while (false);

  return result;
}

bool
ConfigTools::loadInboundConfig()
{
  bool result = true;
  if (m_config.empty()) {
    m_config.insert("inbound", *m_default_config["inbound"].as_table());
    result = false;
  }

  if (m_config["inbound"]["socks"].as_table() == nullptr) {
    m_config.insert_or_assign("inbound",
                              *m_default_config["inbound"].as_table());
    result = false;
  }
  if (m_config["inbound"]["http"].as_table() == nullptr) {
    m_config.insert_or_assign("inbound",
                              *m_default_config["inbound"].as_table());
    result = false;
  }

  m_inbound.fromNodeView(m_config["inbound"], m_default_config["inbound"]);
  return result;
}

bool
ConfigTools::loadNetworkConfig()
{
  if (m_config.empty())
    return false;

  m_network.fromNodeView(m_config["network"], m_default_config["network"]);
  return true;
}

InboundSettings
ConfigTools::getInboundConfig()
{
  return m_inbound;
}

DataBase
ConfigTools::getDBConfig()
{
  return m_db;
}

QString
ConfigTools::getConfigVersion()
{
  if (auto temp = m_config["config_version"].value<std::string>();
      temp.has_value()) {
    return QString::fromStdString(temp.value());
  }

  return {};
}

QString
ConfigTools::getConfigTomlVersion()
{
  auto toml_version =
    QString::fromStdString(*m_config["toml_version"].value<std::string>());

  return toml_version;
}

QString
ConfigTools::getLanguage()
{
  auto language = QString::fromStdString(
    *m_config["interface"]["language"].value<std::string>());

  return language;
}

void
ConfigTools::freshColors()
{
  if (loadThemeConfig()) {
    emit textColorChanged();
    emit backgroundColorChanged();
    emit highlightColorChanged();
    emit highlightTextColorChanged();
    emit warnColorChanged();
    emit warnTextColorChanged();
    emit shadowColorChanged();
    emit borderColorChanged();
    emit deepColorChanged();
    emit deepTextColorChanged();
    emit styleColorChanged();
    emit styleTextColorChanged();
    emit borderRadiusChanged();
    emit borderWidthChanged();
    emit itemSpacingChanged();
    emit iconStyleChanged();
  }
}

bool
ConfigTools::testApi()
{
  bool result = false;

  if (m_core.api.enable) {
    APITools client(m_core.api.port);
    auto [stats, err] = client.isOk();

    if (stats) {
      m_api_result_text = "";
    } else {
      m_api_result_text = QString::fromStdString(err);
    }

    result = stats;
  }

  emit apiResultTextChanged();

  return result;
}

bool
ConfigTools::testAndSetAddr(const QString& addr)
{
  bool result = false;
  QHostAddress temp_addr(addr);

  switch (temp_addr.protocol()) {
    case QAbstractSocket::IPv4Protocol:
    case QAbstractSocket::IPv6Protocol:
      result = true;
      setInboundAddress(addr);
      break;
    default:
      break;
  }

  return result;
}

void
ConfigTools::freshInbound()
{
  if (loadInboundConfig()) {
    emit socksEnableChanged();
    emit socksPortChanged();
    emit socksUsernameChanged();
    emit socksPasswordChanged();
    emit httpEnableChanged();
    emit httpPortChanged();
    emit httpUsernameChanged();
    emit httpPasswordChanged();
  }
}

bool
ConfigTools::saveConfig(QString config_path)
{
  bool result = false;
  QString path;
  std::ofstream config;
  QDateTime now = QDateTime::currentDateTimeUtc();

  do {
    if (!config_path.isEmpty()) {
      path = config_path;
      break;
    }

    if (!m_config_path.isEmpty()) {
      path = m_config_path;
      break;
    }
  } while (false);

  if (!path.isEmpty()) {
    m_db.toNodeView(m_config["database"], m_default_config["database"]);
    m_core.toNodeView(m_config["core"], m_default_config["core"]);
    m_inbound.toNodeView(m_config["inbound"], m_default_config["inbound"]);
    m_network.toNodeView(m_config["network"], m_default_config["network"]);
    m_interface.toNodeView(m_config["interface"],
                           m_default_config["interface"]);

    config.open(path.toStdString(), std::ios::out | std::ios::trunc);

    config << "# Generate Time: " << now.toString().toStdString() << "\n"
           << toml::default_formatter(m_config);

    config.close();
    result = true;
  }

  return result;
}

void
ConfigTools::setDBPath(const QString& db_path, bool init)
{
  auto temp_path = db_path;

#ifdef Q_OS_LINUX
  if ((temp_path == "~") || (temp_path.startsWith("~/"))) {
    temp_path.replace(0, 1, QDir::homePath());
  }

  if (temp_path.startsWith("$")) {
    wordexp_t p;
    wordexp(db_path.toStdString().c_str(), &p, 0);
    temp_path = QString::fromStdString(*p.we_wordv);
    wordfree(&p);
  }
#endif

  if (temp_path.startsWith("file:///")) {
    temp_path = QUrl(temp_path).toLocalFile();
  }

  if (!temp_path.split("/").last().contains(".db")) {
    QDir db_dir(temp_path);
    m_db.path = db_dir.relativeFilePath("across.db");
  } else {
    QFileInfo db_file(temp_path);
    m_db.path = db_file.filePath();
  }

  auto dir = QFileInfo(m_db.path).dir();
  if (!dir.exists()) {
    QDir().mkdir(dir.path());
  }

  // auto save config
  if (!init) {
    emit configChanged();
  }

  emit dbPathChanged();
}

toml::table
ConfigTools::getConfig()
{
  return m_config;
}

QString
ConfigTools::textColor()
{
  return m_theme.colors.text_color;
}

QString
ConfigTools::backgroundColor()
{
  return m_theme.colors.background_color;
}

QString
ConfigTools::highlightColor()
{
  return m_theme.colors.highlight_color;
}

QString
ConfigTools::highlightTextColor()
{
  return m_theme.colors.highlight_text_color;
}

QString
ConfigTools::warnColor()
{
  return m_theme.colors.warn_color;
}

QString
ConfigTools::warnTextColor()
{
  return m_theme.colors.warn_text_color;
}

QString
ConfigTools::shadowColor()
{
  return m_theme.colors.shadow_color;
}

QString
ConfigTools::borderColor()
{
  return m_theme.colors.border_color;
}

QString
ConfigTools::deepColor()
{
  return m_theme.colors.deep_color;
}

QString
ConfigTools::deepTextColor()
{
  return m_theme.colors.deep_text_color;
}

QString
ConfigTools::styleColor()
{
  return m_theme.colors.style_color;
}

QString
ConfigTools::styleTextColor()
{
  return m_theme.colors.style_text_color;
}

QString
ConfigTools::trayStylish()
{
  return m_theme.tray.stylish;
}

QString
ConfigTools::trayColor()
{
  return m_theme.tray.color;
}

int
ConfigTools::borderRadius()
{
  return m_theme.border.radius;
}

int
ConfigTools::borderWidth()
{
  return m_theme.border.width;
}

int
ConfigTools::itemSpacing()
{
  return m_theme.item.spacing;
}

const QString&
ConfigTools::iconStyle() const
{
  return m_theme.icon.style;
}

const QString&
ConfigTools::currentTheme() const
{
  return m_interface.theme.theme;
}

bool
ConfigTools::socksEnable()
{
  return m_inbound.socks.enable;
}

QString
ConfigTools::socksPort()
{
  return QString::fromStdString(std::to_string(m_inbound.socks.port));
}

QString
ConfigTools::socksUsername()
{
  return m_inbound.socks.username;
}

QString
ConfigTools::socksPassword()
{
  return m_inbound.socks.password;
}

bool
ConfigTools::httpEnable()
{
  return m_inbound.http.enable;
}

QString
ConfigTools::httpPort()
{
  return QString::fromStdString(std::to_string(m_inbound.http.port));
}

QString
ConfigTools::httpUsername()
{
  return m_inbound.http.username;
}

QString
ConfigTools::httpPassword()
{
  return m_inbound.http.password;
}

void
ConfigTools::setCorePath(const QUrl& val)
{
  QString path = val.toLocalFile();

  if (path == m_core.core_path) {
    return;
  }

  if (isFileExist(path)) {
    m_core.core_path = path;

    emit configChanged();
    emit corePathChanged();
  }
}

void
ConfigTools::setAssetsPath(const QUrl& val)
{
  QString path = val.toLocalFile();

  if (path == m_core.assets_path || path.isEmpty()) {
    return;
  }

  m_core.assets_path = path;

  emit configChanged();
  emit assetsPathChanged();
}

void
ConfigTools::setLogLevel(const QString& log_level)
{
  if (log_level == m_core.log_level) {
    return;
  }

  m_core.log_level = log_level;

  emit configChanged();
  emit logLevelChanged();
}

void
ConfigTools::setLogLines(int log_lines)
{
  if (log_lines == m_core.log_lines) {
    return;
  }

  m_core.log_lines = log_lines;

  emit configChanged();
  emit logLinesChanged(m_core.log_lines);
}

void
ConfigTools::setApiEnable(bool val)
{
  if (val == m_core.api.enable) {
    return;
  }

  m_core.api.enable = val;

  emit configChanged();
  emit apiEnableChanged();
}

void
ConfigTools::setApiPort(QString& portStr)
{
  uint port = portStr.toUInt();
  if (port == m_core.api.port || portStr.isEmpty()) {
    return;
  }

  if (port >= 0 && port <= 65535) {
    m_core.api.port = port;

    emit configChanged();
    emit apiPortChanged();
  }
}

void
ConfigTools::setInboundAddress(const QString& addr)
{
  if (addr.isEmpty() || addr == m_inbound.socks.listen) {
    return;
  }

  // Keep the listening address the same
  m_inbound.socks.listen = addr;
  m_inbound.http.listen = addr;

  emit configChanged();
  emit inboundAddressChanged();
}

void
ConfigTools::setTextColor(const QString& val)
{
  if (val == m_theme.colors.text_color) {
    return;
  }

  m_theme.colors.text_color = val;
  emit textColorChanged();
}

void
ConfigTools::setBackgroundColor(const QString& val)
{
  if (val == m_theme.colors.background_color) {
    return;
  }

  m_theme.colors.background_color = val;
  emit backgroundColorChanged();
}

void
ConfigTools::setHighlightColor(const QString& val)
{
  if (val == m_theme.colors.highlight_color) {
    return;
  }

  m_theme.colors.highlight_color = val;
  emit highlightColorChanged();
}

void
ConfigTools::setHighlightTextColor(const QString& val)
{
  if (val == m_theme.colors.highlight_text_color) {
    return;
  }

  m_theme.colors.highlight_text_color = val;
  emit highlightTextColorChanged();
}

void
ConfigTools::setWarnColor(const QString& val)
{
  if (val == m_theme.colors.warn_color) {
    return;
  }

  m_theme.colors.warn_color = val;
  emit warnColorChanged();
}

void
ConfigTools::setWarnTextColor(const QString& val)
{
  if (val == m_theme.colors.warn_text_color) {
    return;
  }

  m_theme.colors.warn_text_color = val;
  emit warnTextColorChanged();
}

void
ConfigTools::setShadowColor(const QString& val)
{
  if (val == m_theme.colors.shadow_color) {
    return;
  }

  m_theme.colors.shadow_color = val;
  emit shadowColorChanged();
}

void
ConfigTools::setBorderColor(const QString& val)
{
  if (val == m_theme.colors.border_color) {
    return;
  }

  m_theme.colors.border_color = val;
  emit borderColorChanged();
}

void
ConfigTools::setDeepColor(const QString& val)
{
  if (val == m_theme.colors.deep_color) {
    return;
  }

  m_theme.colors.deep_color = val;
  emit deepColorChanged();
}

void
ConfigTools::setDeepTextColor(const QString& val)
{
  if (val == m_theme.colors.deep_text_color) {
    return;
  }

  m_theme.colors.deep_text_color = val;
  emit deepTextColorChanged();
}

void
ConfigTools::setStyleColor(const QString& val)
{
  if (val == m_theme.colors.style_color) {
    return;
  }

  m_theme.colors.style_color = val;
  emit styleColorChanged();
}

void
ConfigTools::setStyleTextColor(const QString& val)
{
  if (val == m_theme.colors.style_text_color) {
    return;
  }

  m_theme.colors.style_text_color = val;
  emit styleTextColorChanged();
}

void
ConfigTools::setTrayStylish(const QString& val)
{
  if (val == m_theme.tray.stylish) {
    return;
  }

  m_theme.tray.stylish = val;
  emit trayStylishChanged();
}

void
ConfigTools::setTrayColor(const QString& val)
{
  if (val == m_theme.tray.color) {
    return;
  }

  m_theme.tray.color = val;
  emit trayColorChanged();
}

void
ConfigTools::setBorderRadius(int radius)
{
  if (radius == m_theme.border.radius) {
    return;
  }

  m_theme.border.radius = radius;
  emit borderRadiusChanged();
}

void
ConfigTools::setBorderWidth(int width)
{
  if (width == m_theme.border.width) {
    return;
  }

  m_theme.border.width = width;
  emit borderWidthChanged();
}

void
ConfigTools::setItemSpacing(int spacing)
{
  if (spacing == m_theme.item.spacing) {
    return;
  }

  m_theme.item.spacing = spacing;
  emit itemSpacingChanged();
}

void
ConfigTools::setIconStyle(const QString& newIconStyle)
{
  if (m_theme.icon.style == newIconStyle)
    return;

  m_theme.icon.style = newIconStyle;

  emit iconStyleChanged();
}

void
ConfigTools::setCurrentTheme(const QString& newCurrentTheme)
{
  if (m_interface.theme.theme == newCurrentTheme ||
      newCurrentTheme == "current" || newCurrentTheme.isEmpty())
    return;

  *m_config["interface"]["theme"]["theme"].as_string() =
    newCurrentTheme.toStdString();

  if (loadInterfaceTheme()) {
    freshColors();

    emit configChanged();
    emit currentThemeChanged();
    emit trayColorChanged();
    emit trayStylishChanged();
    emit borderColorChanged();
    emit borderRadiusChanged();
  }
}

void
ConfigTools::setSocksEnable(bool val)
{
  if (val == m_inbound.socks.enable) {
    return;
  }

  m_inbound.socks.enable = val;

  emit configChanged();
  emit socksEnableChanged();
}

void
ConfigTools::setSocksPort(const QString& portStr)
{
  uint port = portStr.toUInt();
  if (port == m_inbound.socks.port || portStr.isEmpty()) {
    return;
  }

  m_inbound.socks.port = port;

  emit configChanged();
  emit socksPortChanged();
}

void
ConfigTools::setSocksUsername(const QString& name)
{
  if (name == m_inbound.socks.username) {
    return;
  }

  m_inbound.socks.username = name;

  emit configChanged();
  emit socksUsernameChanged();
}

void
ConfigTools::setSocksPassword(const QString& pass)
{
  if (pass == m_inbound.socks.password) {
    return;
  }

  m_inbound.socks.password = pass;

  emit configChanged();
  emit socksPasswordChanged();
}

void
ConfigTools::setHttpEnable(bool val)
{
  if (val == m_inbound.http.enable) {
    return;
  }

  m_inbound.http.enable = val;

  emit configChanged();
  emit httpEnableChanged();
}

void
ConfigTools::setHttpPort(QString& portStr)
{
  uint port = portStr.toUInt();
  if (port == m_inbound.http.port || portStr.isEmpty()) {
    return;
  }

  m_inbound.http.port = port;

  emit configChanged();
  emit httpPortChanged();
}

void
ConfigTools::setHttpUsername(const QString& name)
{
  if (name == m_inbound.http.username) {
    return;
  }

  m_inbound.http.username = name;

  emit configChanged();
  emit httpUsernameChanged();
}

void
ConfigTools::setHttpPassword(const QString& pass)
{
  if (pass == m_inbound.http.password) {
    return;
  }

  m_inbound.http.password = pass;

  emit configChanged();
  emit httpPasswordChanged();
}

bool
ConfigTools::isFileExist(QString file_path)
{
  return QFile(file_path).exists();
}

QString
ConfigTools::guiVersion()
{
  return getVersion();
}

QString
ConfigTools::coreInfo()
{
  QProcess core_process;
  QString info;

  if (m_core.core_path.isEmpty()) {
    return info;
  }

  core_process.start(m_core.core_path, { "-version" });
  core_process.waitForFinished();

  if (core_process.exitStatus() == QProcess::NormalExit) {
    QString raw_info = core_process.readAllStandardOutput();

    if (!raw_info.isEmpty()) {
      info = raw_info;
    }
  }

  return info;
}

QString
ConfigTools::coreVersion()
{
  auto temp_info = coreInfo();
  if (temp_info.isEmpty()) {
    return "not found";
  }

  QString version =
    temp_info.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts).at(1);
  if (!version.isEmpty()) {
    m_core.core_version = version;
  }

  return m_core.core_version;
}

QString
ConfigTools::corePath()
{
  return m_core.core_path;
}

QString
ConfigTools::assetsPath()
{
  return m_core.assets_path;
}

QString
ConfigTools::logLevel()
{
  return m_core.log_level;
}

int
ConfigTools::logLines()
{
  return m_core.log_lines;
}

QString
ConfigTools::dbPath()
{
  return m_db.path;
}

Core
ConfigTools::getCore()
{
  return m_core;
}

bool
ConfigTools::apiEnable()
{
  return m_core.api.enable;
}

QString
ConfigTools::apiPort()
{
  return QString::fromStdString(std::to_string(m_core.api.port));
}

QString
ConfigTools::apiResultText()
{
  return m_api_result_text;
}

QString
ConfigTools::inboundAddress()
{
  return m_inbound.socks.listen;
}

const QString&
ConfigTools::currentLanguage() const
{
  return m_interface.language.language;
}

bool
ConfigTools::enableTray()
{
  return m_interface.tray.enable;
}

const QString&
ConfigTools::networkTestMethod() const
{
  return m_network.test_method;
}

const QString&
ConfigTools::networkTestURL() const
{
  return m_network.test_url;
}

const QString&
ConfigTools::networkUserAgent() const
{
  return m_network.user_agent;
}

void
ConfigTools::setCurrentLanguage(const QString& newCurrentLanguage)
{
  if (m_interface.language.language == newCurrentLanguage ||
      newCurrentLanguage == "current" || newCurrentLanguage.isEmpty())
    return;

  m_interface.language.language = newCurrentLanguage;

  emit configChanged();
  emit currentLanguageChanged(m_interface.language.language);
}

void
ConfigTools::setEnableTray(bool val)
{
  if (m_interface.tray.enable == val)
    return;

  m_interface.tray.enable = val;

  emit configChanged();
  emit enableTrayChanged();
}

void
ConfigTools::setNetworkTestMethod(const QString& val)
{
  if (m_network.test_method == val)
    return;

  m_network.test_method = val;

  emit configChanged();
  emit networkTestMethodChanged();
}

void
ConfigTools::setNetworkTestURL(const QString& val)
{
  if (m_network.test_url == val)
    return;

  m_network.test_url = val;

  emit configChanged();
  emit networkTestURLChanged();
}

void
ConfigTools::setNetworkUserAgent(const QString& val)
{
  if (m_network.user_agent == val)
    return;

  m_network.user_agent = val;

  emit configChanged();
  emit networkUserAgentChanged();
}

QString
ConfigTools::buildInfo()
{
  return getBuildInfo();
}

QString
ConfigTools::extraInfo()
{
  return getExtraInfo();
}

QString
ConfigTools::buildTime()
{
  return getBuildTime();
}

QString
ConfigTools::sourceCodeURL()
{
  return getSourceCodeURL();
}

QString
ConfigTools::reportURL()
{
  return getReportURL();
}

QString
ConfigTools::licenseURL()
{
  return getLicenseURL();
}
