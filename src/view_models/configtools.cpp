#include "configtools.h"

using namespace across::setting;
using namespace across::core;
using namespace across::config;

void
Interface::Language::fromNodeView(toml::v2::node_view<toml::node> language)
{
  this->language = QString::fromStdString(*language.value<std::string>());
}

void
Interface::Language::toNodeView(const toml::v2::node_view<toml::node>& language)
{
  *language.as_string() = this->language.toStdString();
}

void
Interface::Theme::fromNodeView(toml::v2::node_view<toml::node> theme)
{
  this->theme = QString::fromStdString(*theme["theme"].value<std::string>());
  this->include_dir =
    QString::fromStdString(*theme["include_dir"].value<std::string>());
}

void
Interface::Theme::toNodeView(const toml::v2::node_view<toml::node>& theme)
{
  *theme["theme"].as_string() = this->theme.toStdString();
  *theme["include_dir"].as_string() = this->include_dir.toStdString();
}

void
Interface::fromNodeView(toml::v2::node_view<toml::node> interface)
{
  this->language.fromNodeView(interface["language"]);
  this->theme.fromNodeView(interface["theme"]);
}

void
Interface::toNodeView(const toml::v2::node_view<toml::node>& interface)
{
  this->language.toNodeView(interface["language"]);
  this->theme.toNodeView(interface["theme"]);
}

void
Update::fromNodeView(toml::v2::node_view<toml::node> update)
{
  this->auto_update = *update["auto_update"].value<bool>();
  this->check_update = *update["check_update"].value<bool>();
  this->update_from_proxy = *update["update_from_proxy"].value<bool>();
  this->update_channel =
    QString::fromStdString(*update["update_channel"].value<std::string>());
}

void
Update::toNodeView(const toml::v2::node_view<toml::node>& update)
{
  *update["auto_update"].as_boolean() = this->auto_update;
  *update["check_update"].as_boolean() = this->check_update;
  *update["update_channel"].as_string() = this->update_channel.toStdString();
  *update["update_from_proxy"].as_boolean() = this->update_from_proxy;
}

void
DataBase::fromNodeView(toml::v2::node_view<toml::node> database)
{
  this->path =
    QString::fromStdString(*database["db_path"].value<std::string>());
  this->backend =
    QString::fromStdString(*database["db_backend"].value<std::string>());

  auto _enable_auth = database["auth"]["enable"].value<bool>();
  if (_enable_auth.has_value() && _enable_auth.value() == true) {
    this->auth.enable = _enable_auth.value();

    auto _auth = database["auth"];
    this->auth.username =
      QString::fromStdString(*_auth["username"].value<std::string>());
    this->auth.password =
      QString::fromStdString(*_auth["password"].value<std::string>());
    this->auth.address =
      QString::fromStdString(*_auth["address"].value<std::string>());

    auto _port = database["auth"]["port"].value<uint>();
    if (_port.has_value()) {
      this->auth.port = _port.value();
    }
  }
}

void
DataBase::toNodeView(const toml::v2::node_view<toml::node>& database)
{
  *database["db_path"].as_string() = this->path.toStdString();
  *database["db_backend"].as_string() = this->backend.toStdString();

  if (this->auth.enable) {
    *database["auth"]["enable"].as_boolean() = this->auth.enable;
    *database["auth"]["username"].as_string() =
      this->auth.username.toStdString();
    *database["auth"]["password"].as_string() =
      this->auth.password.toStdString();
    *database["auth"]["address"].as_string() = this->auth.address.toStdString();
    *database["auth"]["port"].as_integer() = this->auth.port;
  }
}

void
Core::fromNodeView(toml::v2::node_view<toml::node> core)
{
  this->core_path =
    QString::fromStdString(*core["core_path"].value<std::string>());
  this->assets_path =
    QString::fromStdString(*core["assets_path"].value<std::string>());
  this->log_level =
    QString::fromStdString(*core["log_level"].value<std::string>());

  auto _log_lines = core["log_lines"].value<int>();
  if (_log_lines.has_value()) {
    this->log_lines = _log_lines.value();
  }

  auto _api_enable = core["api"]["enable"].value<bool>();
  if (_api_enable.has_value() && _api_enable.value() == true) {
    this->api.enable = _log_lines.value();

    auto _api_port = core["api"]["port"].value<uint>();
    if (_api_port.has_value()) {
      this->api.port = _api_port.value();
    }
  }
}

void
Core::toNodeView(const toml::v2::node_view<toml::node>& core)
{
  *core["core_path"].as_string() = core_path.toStdString();
  *core["assets_path"].as_string() = assets_path.toStdString();
  *core["log_level"].as_string() = log_level.toStdString();
  *core["log_lines"].as_integer() = log_lines;
  *core["api"]["enable"].as_boolean() = api.enable;
  *core["api"]["port"].as_integer() = api.port;
}

void
Theme::Colors::fromNodeView(toml::v2::node_view<toml::node> colors_node)
{
  this->text_color =
    QString::fromStdString(*colors_node["text_color"].value<std::string>());
  this->background_color = QString::fromStdString(
    *colors_node["background_color"].value<std::string>());
  this->highlight_color = QString::fromStdString(
    *colors_node["highlight_color"].value<std::string>());
  this->highlight_text_color = QString::fromStdString(
    *colors_node["highlight_text_color"].value<std::string>());
  this->warn_color =
    QString::fromStdString(*colors_node["warn_color"].value<std::string>());
  this->warn_text_color = QString::fromStdString(
    *colors_node["warn_text_color"].value<std::string>());
  this->shadow_color =
    QString::fromStdString(*colors_node["shadow_color"].value<std::string>());
  this->border_color =
    QString::fromStdString(*colors_node["border_color"].value<std::string>());
  this->deep_color =
    QString::fromStdString(*colors_node["deep_color"].value<std::string>());
  this->deep_text_color = QString::fromStdString(
    *colors_node["deep_text_color"].value<std::string>());
  this->style_color =
    QString::fromStdString(*colors_node["style_color"].value<std::string>());
  this->style_text_color = QString::fromStdString(
    *colors_node["style_text_color"].value<std::string>());
}

void
InboundSettings::SOCKS::fromNodeView(toml::v2::node_view<toml::node> socks)
{
  enable = *socks["enable"].value<bool>();

  if (enable) {
    this->listen =
      QString::fromStdString(*socks["listen"].value<std::string>());
    this->port = *socks["port"].value<uint>();

    this->udp = *socks["udp"].value<bool>();
    this->ip = QString::fromStdString(*socks["ip"].value<std::string>());
    this->user_level = *socks["user_level"].value<int>();

    this->username =
      QString::fromStdString(*socks["auth"]["username"].value<std::string>());
    this->password =
      QString::fromStdString(*socks["auth"]["password"].value<std::string>());
  }
}

void
InboundSettings::SOCKS::toNodeView(const toml::v2::node_view<toml::node>& socks)
{
  *socks["enable"].as_boolean() = enable;
  *socks["listen"].as_string() = listen.toStdString();
  *socks["port"].as_integer() = port;

  *socks["udp"].as_boolean() = udp;
  *socks["ip"].as_string() = ip.toStdString();
  *socks["user_level"].as_integer() = user_level;

  *socks["auth"]["username"].as_string() = username.toStdString();
  *socks["auth"]["password"].as_string() = password.toStdString();
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
InboundSettings::HTTP::fromNodeView(toml::v2::node_view<toml::node> http)
{
  enable = *http["enable"].value<bool>();

  if (enable) {
    this->listen = QString::fromStdString(*http["listen"].value<std::string>());
    this->port = *http["port"].value<uint>();

    this->allow_transparent = *http["allow_transparent"].value<bool>();
    this->timeout = *http["timeout"].value<int>();
    this->user_level = *http["user_level"].value<int>();

    this->username =
      QString::fromStdString(*http["auth"]["username"].value<std::string>());
    this->password =
      QString::fromStdString(*http["auth"]["password"].value<std::string>());
  }
}

void
InboundSettings::HTTP::toNodeView(const toml::v2::node_view<toml::node>& http)
{
  *http["enable"].as_boolean() = enable;
  *http["listen"].as_string() = listen.toStdString();
  *http["port"].as_integer() = port;

  *http["allow_transparent"].as_boolean() = allow_transparent;
  *http["timeout"].as_integer() = timeout;
  *http["user_level"].as_integer() = user_level;

  *http["auth"]["username"].as_string() = username.toStdString();
  *http["auth"]["password"].as_string() = password.toStdString();
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
InboundSettings::fromNodeView(toml::v2::node_view<toml::node> inbound)
{
  socks.fromNodeView(inbound["socks"]);
  http.fromNodeView(inbound["http"]);
}

void
InboundSettings::toNodeView(const toml::v2::node_view<toml::node>& inbound)
{
  if (socks.enable) {
    socks.toNodeView(inbound["socks"]);
  }

  if (http.enable) {
    http.toNodeView(inbound["http"]);
  }
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
ConfigTools::init(std::shared_ptr<spdlog::details::thread_pool> thread_pool,
                  QString file_path)
{
  bool result = false;

  // initial config logger
  p_logger = std::make_shared<LogTools>(thread_pool, "setting");

  QString config_path = loadConfigPath(file_path);

  connect(
    this, &ConfigTools::configChanged, this, [&]() { this->saveConfig(); });

  do {
    if (config_path.isEmpty()) {
      p_logger->warn("Failed to load config path");
      break;
    } else {
      this->m_config_path = config_path;

      this->m_config = toml::parse_file(config_path.toStdString());
      if (m_config.empty()) {
        p_logger->error("Failed to parse config");
        break;
      }
    }

    if (!(result = loadCoreConfig())) {
      p_logger->error("Failed to load core config");
      break;
    }

    if (!(result = loadDBConfig())) {
      p_logger->error("Failed to load database config");
      break;
    }

    if (!(result = loadInterfaceConfig())) {
      p_logger->error("Failed to load interface config");
      break;
    }

    if (!(result = loadThemeConfig())) {
      p_logger->error("Failed to load colors config");
      break;
    }

    if (!(result = loadInboundConfig())) {
      p_logger->error("Failed to load inbounds config");
      break;
    }

    emit configChanged();
    result = true;
  } while (false);

  return result;
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
    if (m_config.empty() || !m_config["interface"].is_table()) {
      break;
    }

    m_interface.fromNodeView(m_config["interface"]);

    result = true;
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

    m_interface.theme.fromNodeView(m_config["interface"]["theme"]);

    result = true;
  } while (false);

  return result;
}

bool
ConfigTools::loadInterfaceLanguage()
{
  bool result = false;

  do {
    if (m_config.empty()) {
      break;
    }

    m_interface.fromNodeView(m_config["interface"]["language"]);

    result = true;
  } while (false);

  return result;
}

bool
ConfigTools::loadUpdateConfig()
{
  bool result = false;

  do {
    if (m_config.empty() || !m_config["update"].is_table()) {
      break;
    }

    m_update.fromNodeView(m_config["update"]);

    result = true;
  } while (false);

  return result;
}

bool
ConfigTools::loadDBConfig()
{
  bool result = false;

  do {
    auto temp = m_config["database"]["db_path"].value<std::string>();
    if (!temp.has_value()) {
      p_logger->warn("Failed to get the db_path, use default path `{}`",
                     m_db.path.toStdString());
      result = isFileExist(m_db.path);
      break;
    }

    if (temp != m_db.path.toStdString()) {
      setDBPath(QString::fromStdString(temp.value()), true);
    }

    result = true;
  } while (false);

  return result;
}

bool
ConfigTools::loadThemeConfig()
{
  bool result = false;

  do {
    auto themes = m_config["themes"];
    if (!themes.is_array() || themes.as_array()->size() == 0) {
      p_logger->error("No theme configuration");
      break;
    }

    for (int i = 0; i < themes.as_array()->size(); ++i) {
      auto temp = themes[i]["name"].value<std::string>();

      if (temp.has_value()) {
        auto temp_name = QString::fromStdString(temp.value());

        if (m_interface.theme.theme == temp_name) {
          m_theme.name = temp_name;

          m_theme.tray.stylish = QString::fromStdString(
            *themes[i]["tray"]["stylish"].value<std::string>());
          m_theme.tray.color = QString::fromStdString(
            *themes[i]["tray"]["color"].value<std::string>());

          m_theme.border.radius = *themes[i]["border"]["radius"].value<int>();
          m_theme.border.width = *themes[i]["border"]["width"].value<int>();

          m_theme.item.spacing = *themes[i]["item"]["spacing"].value<int>();

          m_theme.icon.style = QString::fromStdString(
            *themes[i]["icon"]["style"].value<std::string>());

          m_theme.colors.fromNodeView(themes[i]["colors"]);
          result = true;
          break;
        }
      } else {
        p_logger->warn("Failed to load the theme: {}", std::to_string(i));
        break;
      }
    }
  } while (false);

  return result;
}

bool
ConfigTools::loadCoreConfig()
{
  bool result = false;
  auto envs_info = m_envs.getInfo();

  do {
    if (m_config.empty() || !m_config["core"].is_table()) {
      break;
    }

    m_core.fromNodeView(m_config["core"]);

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

    result = true;
  } while (false);

  return result;
}

bool
ConfigTools::loadInboundConfig()
{
  bool result = false;

  do {
    if (m_config.empty()) {
      break;
    }

    m_inbound.fromNodeView(m_config["inbound"]);
    result = true;
  } while (false);

  return result;
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
  QString config_version;
  auto temp = m_config["config_version"].value<std::string>();
  if (temp.has_value()) {
    config_version = QString::fromStdString(temp.value());
  }

  return config_version;
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
    m_db.toNodeView(m_config["database"]);
    m_core.toNodeView(m_config["core"]);
    m_inbound.toNodeView(m_config["inbound"]);
    m_interface.toNodeView(m_config["interface"]);

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

  QString version = temp_info.split(QRegExp("\\s+"), Qt::SkipEmptyParts).at(1);
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
