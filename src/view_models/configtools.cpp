#include "configtools.h"

using namespace across::setting;
using namespace across::core;
using namespace across::config;
using namespace across::utils;

ConfigTools::ConfigTools(QObject* parent) {}

bool
ConfigTools::init(QSharedPointer<LogView> log_view, const QString& file_path)
{
  // initial config logger
  p_logger = std::make_shared<LogTools>(log_view, "setting");

  connect(
    this, &ConfigTools::configChanged, this, [&]() { this->saveConfig(); });

  if (auto config_path = loadConfigPath(file_path); config_path.isEmpty()) {
    p_logger->warn("Failed to load config path, use build-in config instead");
  } else {
    this->m_config_path = config_path;

    if (auto json_str = ConfigHelper::readFromFile(m_config_path.toStdString());
        !json_str.empty()) {
      this->m_conf.MergeFrom(ConfigHelper::fromJson(json_str));
    }
  }

  p_core = m_conf.mutable_core();
  p_db = m_conf.mutable_database();
  p_interface = m_conf.mutable_interface();
  p_inbound = m_conf.mutable_inbound();
  p_network = m_conf.mutable_network();
  setDBPath("", true);
  loadThemeConfig();
  emit configChanged();
  return true;
}

QString
ConfigTools::loadConfigPath(const QString& file_path)
{
  QString config_path = "./across.json";

  // Setting Path > Env Path > Current Path > Default Path
  do {
    if (isFileExist(file_path)) {
      config_path = file_path;
      break;
    }

    if (auto env_path = m_envs.getInfo().ACROSS_CONFIG_PATH;
        !env_path.isEmpty()) {
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
    config_path = xdg_path.filePath(m_config_name);
    if (isFileExist(config_path)) {
      break;
    } else {
      // create directory and example config file
      if (!xdg_path.exists()) {
        xdg_path.mkdir(xdg_path.path());
      }

      ConfigHelper::saveToFile(ConfigHelper::toJson(m_conf),
                               config_path.toStdString());

      p_logger->info("Generate new config on: {}", config_path.toStdString());
    }
  } while (false);

  return config_path;
}

void
ConfigTools::loadThemeConfig()
{
  auto interface_theme = p_interface->mutable_theme();
  for (auto& theme : *m_conf.mutable_themes()) {
    if (interface_theme->theme() == theme.name()) {
      p_theme = &theme;
      break;
    }
  }

  auto debug = p_theme->name();

  freshColors();
  emit currentThemeChanged();
  emit trayColorChanged();
  emit trayStylishChanged();
  emit borderColorChanged();
  emit borderRadiusChanged();
}

Config*
ConfigTools::configPtr()
{
  return &m_conf;
}

void
ConfigTools::setInboundObject(Json::Value& root)
{}

QString
ConfigTools::getConfigVersion()
{
  return QString::fromStdString(m_conf.config_version());
}

QString
ConfigTools::getConfigTomlVersion()
{
  return QString::fromStdString(m_conf.toml_version());
}

QString
ConfigTools::getLanguage()
{
  return QString::fromStdString(m_conf.interface().language());
}

void
ConfigTools::freshColors()
{
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

bool
ConfigTools::testAPI()
{
  bool result = false;

  if (m_conf.core().api().enable()) {
    APITools client(m_conf.core().api().port());
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
    emit socksEnableChanged();
    emit socksPortChanged();
    emit socksUsernameChanged();
    emit socksPasswordChanged();
    emit httpEnableChanged();
    emit httpPortChanged();
    emit httpUsernameChanged();
    emit httpPasswordChanged();
}

void
ConfigTools::saveConfig(QString config_path)
{
  if (!config_path.isEmpty()) {
    ConfigHelper::saveToFile(ConfigHelper::toJson(m_conf),
                             config_path.toStdString());
  } else {
    ConfigHelper::saveToFile(ConfigHelper::toJson(m_conf),
                             m_config_path.toStdString());
  }
}

void
ConfigTools::setDBPath(const QString& db_path, bool init)
{
  QString temp_path = db_path;
  if (db_path.isEmpty())
    temp_path = p_db->db_path().c_str();

#ifdef Q_OS_LINUX
  if ((temp_path == "~") || (temp_path.startsWith("~/"))) {
    temp_path.replace(0, 1, QDir::homePath());
  }

  if (temp_path.startsWith("$")) {
    wordexp_t p;
    wordexp(temp_path.toStdString().c_str(), &p, 0);
    temp_path = QString::fromStdString(*p.we_wordv);
    wordfree(&p);
  }
#endif

  if (temp_path.startsWith("file:///")) {
    temp_path = QUrl(temp_path).toLocalFile();
  }

  if (!temp_path.split("/").last().contains(".db")) {
    auto path = QDir(temp_path).relativeFilePath("across.db").toStdString();
    p_db->set_db_path(path);
  } else {
    p_db->set_db_path(QFileInfo(temp_path).filePath().toStdString());
  }

  if (auto dir = QFileInfo(temp_path).dir(); !dir.exists()) {
    QDir().mkdir(dir.path());
  }

  // auto save config
  if (!init) {
    emit configChanged();
  }

  emit dbPathChanged();
}

QString
ConfigTools::textColor()
{
  return p_theme->colors().text_color().c_str();
}

QString
ConfigTools::backgroundColor()
{
  return p_theme->colors().background_color().c_str();
}

QString
ConfigTools::highlightColor()
{
  return p_theme->colors().highlight_color().c_str();
}

QString
ConfigTools::highlightTextColor()
{
  return p_theme->colors().highlight_text_color().c_str();
}

QString
ConfigTools::warnColor()
{
  return p_theme->colors().warn_color().c_str();
}

QString
ConfigTools::warnTextColor()
{
  return p_theme->colors().warn_text_color().c_str();
}

QString
ConfigTools::shadowColor()
{
  return p_theme->colors().shadow_color().c_str();
}

QString
ConfigTools::borderColor()
{
  return p_theme->colors().border_color().c_str();
}

QString
ConfigTools::deepColor()
{
  return p_theme->colors().deep_color().c_str();
}

QString
ConfigTools::deepTextColor()
{
  return p_theme->colors().deep_text_color().c_str();
}

QString
ConfigTools::styleColor()
{
  return p_theme->colors().style_color().c_str();
}

QString
ConfigTools::styleTextColor()
{
  return p_theme->colors().style_color().c_str();
}

QString
ConfigTools::trayStylish()
{
  return p_theme->tray().stylish().c_str();
}

QString
ConfigTools::trayColor()
{
  return p_theme->tray().color().c_str();
}

int
ConfigTools::borderRadius()
{
  return p_theme->border().radius();
}

int
ConfigTools::borderWidth()
{
  return p_theme->border().width();
}

int
ConfigTools::itemSpacing()
{
  return p_theme->item().spacing();
}

QString
ConfigTools::iconStyle()
{
  return p_theme->icon().style().c_str();
}

QString
ConfigTools::currentTheme()
{
  return p_interface->theme().theme().c_str();
}

bool
ConfigTools::socksEnable()
{
  return p_inbound->socks5().enable();
}

QString
ConfigTools::socksPort()
{
  return std::to_string(p_inbound->socks5().port()).c_str();
}

QString
ConfigTools::socksUsername()
{
  return p_inbound->socks5().auth().username().c_str();
}

QString
ConfigTools::socksPassword()
{
  return p_inbound->socks5().auth().password().c_str();
}

bool
ConfigTools::httpEnable()
{
  return p_inbound->http().enable();
}

QString
ConfigTools::httpPort()
{
  return std::to_string(p_inbound->http().port()).c_str();
}

QString
ConfigTools::httpUsername()
{
  return p_inbound->http().auth().username().c_str();
}

QString
ConfigTools::httpPassword()
{
  return p_inbound->http().auth().password().c_str();
}

void
ConfigTools::setCorePath(const QUrl& val)
{
  QString path = val.toLocalFile();
  if (path == p_core->core_path().c_str())
    return;
  if (isFileExist(path)) {
    p_core->set_core_path(path.toStdString());
    emit configChanged();
    emit corePathChanged();
  }
}

void
ConfigTools::setAssetsPath(const QUrl& val)
{
  QString path = val.toLocalFile();
  if (path == p_core->assets_path().c_str() || path.isEmpty())
    return;
  p_core->set_assets_path(path.toStdString());
  emit configChanged();
  emit assetsPathChanged();
}

void
ConfigTools::setLogLevel(const QString& log_level)
{
  if (log_level == p_core->log_level().c_str())
    return;
  p_core->set_log_level(log_level.toStdString());
  emit configChanged();
  emit logLevelChanged();
}

void
ConfigTools::setLogLines(int log_lines)
{
  if (log_lines == p_core->log_lines())
    return;
  p_core->set_log_lines(log_lines);
  emit configChanged();
  emit logLinesChanged(log_lines);
}

void
ConfigTools::setApiEnable(bool val)
{
  if (val == p_core->api().enable())
    return;
  p_core->mutable_api()->set_enable(val);
  emit configChanged();
  emit apiEnableChanged();
}

void
ConfigTools::setApiPort(QString& portStr)
{
  uint port = portStr.toUInt();
  if (port == p_core->api().port() || portStr.isEmpty())
    return;
  if (port >= 0 && port <= 65535) {
    p_core->mutable_api()->set_port(port);
    emit configChanged();
    emit apiPortChanged();
  }
}

void
ConfigTools::setInboundAddress(const QString& addr)
{
  if (addr.isEmpty() || addr == p_inbound->socks5().listen().c_str())
    return;
  p_inbound->mutable_socks5()->set_listen(addr.toStdString());
  p_inbound->mutable_http()->set_listen(addr.toStdString());
  emit configChanged();
  emit inboundAddressChanged();
}

void
ConfigTools::setTextColor(const QString& val)
{
  if (val == p_theme->colors().text_color().c_str())
    return;
  p_theme->mutable_colors()->set_text_color(val.toStdString());
  emit textColorChanged();
}

void
ConfigTools::setBackgroundColor(const QString& val)
{
  if (val == p_theme->colors().background_color().c_str())
    return;
  p_theme->mutable_colors()->set_background_color(val.toStdString());
  emit backgroundColorChanged();
}

void
ConfigTools::setHighlightColor(const QString& val)
{
  if (val == p_theme->colors().highlight_color().c_str())
    return;
  p_theme->mutable_colors()->set_highlight_color(val.toStdString());
  emit highlightColorChanged();
}

void
ConfigTools::setHighlightTextColor(const QString& val)
{
  if (val == p_theme->colors().highlight_text_color().c_str())
    return;
  p_theme->mutable_colors()->set_highlight_text_color(val.toStdString());
  emit highlightTextColorChanged();
}

void
ConfigTools::setWarnColor(const QString& val)
{
  if (val == p_theme->colors().warn_color().c_str())
    return;
  p_theme->mutable_colors()->set_warn_color(val.toStdString());
  emit warnColorChanged();
}

void
ConfigTools::setWarnTextColor(const QString& val)
{
  if (val == p_theme->colors().warn_text_color().c_str())
    return;
  p_theme->mutable_colors()->set_warn_text_color(val.toStdString());
  emit warnTextColorChanged();
}

void
ConfigTools::setShadowColor(const QString& val)
{
  if (val == p_theme->colors().shadow_color().c_str())
    return;
  p_theme->mutable_colors()->set_shadow_color(val.toStdString());
  emit shadowColorChanged();
}

void
ConfigTools::setBorderColor(const QString& val)
{
  if (val == p_theme->colors().border_color().c_str())
    return;
  p_theme->mutable_colors()->set_border_color(val.toStdString());
  emit borderColorChanged();
}

void
ConfigTools::setDeepColor(const QString& val)
{
  if (val == p_theme->colors().deep_color().c_str())
    return;
  p_theme->mutable_colors()->set_deep_color(val.toStdString());
  emit deepColorChanged();
}

void
ConfigTools::setDeepTextColor(const QString& val)
{
  if (val == p_theme->colors().deep_text_color().c_str())
    return;
  p_theme->mutable_colors()->set_deep_text_color(val.toStdString());
  emit deepTextColorChanged();
}

void
ConfigTools::setStyleColor(const QString& val)
{
  if (val == p_theme->colors().style_color().c_str())
    return;
  p_theme->mutable_colors()->set_style_color(val.toStdString());
  emit styleColorChanged();
}

void
ConfigTools::setStyleTextColor(const QString& val)
{
  if (val == p_theme->colors().style_text_color().c_str())
    return;
  p_theme->mutable_colors()->set_style_text_color(val.toStdString());
  emit styleTextColorChanged();
}

void
ConfigTools::setTrayStylish(const QString& val)
{
  if (val == p_theme->tray().stylish().c_str())
    return;
  p_theme->mutable_tray()->set_stylish(val.toStdString());
  emit trayStylishChanged();
}

void
ConfigTools::setTrayColor(const QString& val)
{
  if (val == p_theme->tray().color().c_str())
    return;
  p_theme->mutable_tray()->set_color(val.toStdString());
  emit trayColorChanged();
}

void
ConfigTools::setBorderRadius(int val)
{
  if (val == p_theme->border().radius())
    return;
  p_theme->mutable_border()->set_radius(val);
  emit borderRadiusChanged();
}

void
ConfigTools::setBorderWidth(int val)
{
  if (val == p_theme->border().width())
    return;
  p_theme->mutable_border()->set_width(val);
  emit borderWidthChanged();
}

void
ConfigTools::setItemSpacing(int val)
{
  if (val == p_theme->item().spacing())
    return;
  p_theme->mutable_item()->set_spacing(val);
  emit itemSpacingChanged();
}

void
ConfigTools::setIconStyle(const QString& val)
{
  if (val == p_theme->icon().style().c_str())
    return;
  p_theme->mutable_icon()->set_style(val.toStdString());
  emit iconStyleChanged();
}

void
ConfigTools::setCurrentTheme(const QString& val)
{
  if (val == p_interface->theme().theme().c_str() || val.isEmpty() ||
      val.contains("current"))
    return;
  p_interface->mutable_theme()->set_theme(val.toStdString());
  loadThemeConfig();
}

void
ConfigTools::setSocksEnable(bool val)
{
  if (val == p_inbound->socks5().enable())
    return;
  p_inbound->mutable_socks5()->set_enable(val);
  emit configChanged();
  emit socksEnableChanged();
}

void
ConfigTools::setSocksPort(const QString& portStr)
{
  uint port = portStr.toUInt();
  if (port == p_inbound->socks5().port())
    return;
  p_inbound->mutable_socks5()->set_port(port);
  emit configChanged();
  emit socksPortChanged();
}

void
ConfigTools::setSocksUsername(const QString& val)
{
  if (val == p_inbound->socks5().auth().username().c_str())
    return;
  if (auto auth = p_inbound->mutable_socks5()->mutable_auth(); val.isEmpty()) {
    auth->set_enable(false);
  } else {
    auth->set_enable(true);
    auth->set_username(val.toStdString());
    emit socksUsernameChanged();
  }
  emit configChanged();
}

void
ConfigTools::setSocksPassword(const QString& val)
{
  if (val == p_inbound->socks5().auth().password().c_str())
    return;
  if (auto auth = p_inbound->mutable_socks5()->mutable_auth(); auth->enable()) {
    auth->set_password(val.toStdString());
    emit configChanged();
    emit socksPasswordChanged();
  }
}

void
ConfigTools::setHttpEnable(bool val)
{
  if (val == p_inbound->http().enable())
    return;
  p_inbound->mutable_http()->set_enable(val);
  emit configChanged();
  emit httpEnableChanged();
}

void
ConfigTools::setHttpPort(QString& portStr)
{
  uint port = portStr.toUInt();
  if (port == p_inbound->http().port())
    return;
  p_inbound->mutable_http()->set_port(port);
  emit configChanged();
  emit httpPortChanged();
}

void
ConfigTools::setHttpUsername(const QString& val)
{
  if (val == p_inbound->http().auth().username().c_str())
    return;
  if (auto auth = p_inbound->mutable_http()->mutable_auth(); val.isEmpty()) {
    auth->set_enable(false);
  } else {
    auth->set_enable(true);
    auth->set_username(val.toStdString());
    emit configChanged();
  }
  emit httpUsernameChanged();
}

void
ConfigTools::setHttpPassword(const QString& val)
{
  if (val == p_inbound->http().auth().password().c_str())
    return;
  if (auto auth = p_inbound->mutable_http()->mutable_auth(); auth->enable()) {
    auth->set_password(val.toStdString());
    emit configChanged();
    emit httpPasswordChanged();
  }
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
  if (p_core->core_path().empty())
    return "";

  core_process.start(p_core->core_path().c_str(), { "-version" });
  core_process.waitForFinished();

  if (core_process.exitStatus() == QProcess::NormalExit)
    if (auto raw_info = core_process.readAllStandardOutput();
        !raw_info.isEmpty())
      return raw_info;

  return "";
}

QString
ConfigTools::coreVersion()
{
  if (auto info = coreInfo(); info.isEmpty())
    return "";
  else if (auto version =
             info.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts).at(1);
           !version.isEmpty())
    return version;
  return "";
}

QString
ConfigTools::corePath()
{
  return p_core->core_path().c_str();
}

QString
ConfigTools::assetsPath()
{
  return p_core->assets_path().c_str();
}

QString
ConfigTools::logLevel()
{
  return p_core->log_level().c_str();
}

int
ConfigTools::logLines()
{
  return p_core->log_lines();
}

QString
ConfigTools::dbPath()
{
  return p_db->db_path().c_str();
}

bool
ConfigTools::apiEnable()
{
  return p_core->api().enable();
}

QString
ConfigTools::apiPort()
{
  return std::to_string(p_core->api().port()).c_str();
}

QString
ConfigTools::apiResultText()
{
  return m_api_result_text;
}

QString
ConfigTools::inboundAddress()
{
  return p_inbound->socks5().listen().c_str();
}

QString
ConfigTools::currentLanguage()
{
  return p_interface->language().c_str();
}

bool
ConfigTools::enableTray()
{
  return p_interface->tray().enable();
}

QString
ConfigTools::networkTestMethod()
{
  return p_network->test_method().c_str();
}

QString
ConfigTools::networkTestURL()
{
  return p_network->test_url().c_str();
}

QString
ConfigTools::networkUserAgent()
{
  return p_network->user_agent().c_str();
}

void
ConfigTools::setCurrentLanguage(const QString& val)
{
  if (val == p_interface->language().c_str() || val.isEmpty() ||
      val.contains("current"))
    return;
  p_interface->set_language(val.toStdString());
  emit configChanged();
  emit currentLanguageChanged(val);
}

void
ConfigTools::setEnableTray(bool val)
{
  if (val == p_interface->tray().enable())
    return;
  p_interface->mutable_tray()->set_enable(val);
  emit configChanged();
  emit enableTrayChanged();
}

void
ConfigTools::setNetworkTestMethod(const QString& val)
{
  if (val == p_network->test_method().c_str())
    return;
  p_network->set_test_method(val.toStdString());
  emit configChanged();
  emit networkTestMethodChanged();
}

void
ConfigTools::setNetworkTestURL(const QString& val)
{
  if (val == p_network->test_url().c_str())
    return;
  p_network->set_test_url(val.toStdString());
  emit configChanged();
  emit networkTestURLChanged();
}

void
ConfigTools::setNetworkUserAgent(const QString& val)
{
  if (val == p_network->user_agent().c_str())
    return;
  p_network->set_user_agent(val.toStdString());
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
