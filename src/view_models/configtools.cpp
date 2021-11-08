#include "configtools.h"

using namespace across::setting;
using namespace across::core;
using namespace across::config;
using namespace across::network;
using namespace across::utils;

ConfigTools::ConfigTools(QObject* parent) {}

bool
ConfigTools::init(QSharedPointer<CURLTools> curl, const QString& file_path)
{
  connect(
    this, &ConfigTools::configChanged, this, [&]() { this->saveConfig(); });

  if (loadConfigPath()) {
    if (auto json_str = ConfigHelper::readFromFile(m_config_path);
        !json_str.empty()) {
      mergeConfigFromJSON(json_str);
    } else {
      qDebug() << "Create a new config on: " << m_config_path;
    }
  }

  if (curl != nullptr) {
    p_curl = curl;
  } else {
    return false;
  }

  p_core = m_config.mutable_core();
  p_db = m_config.mutable_database();
  p_interface = m_config.mutable_interface();
  p_network = m_config.mutable_network();
  p_inbound = m_config.mutable_inbound();

  loadThemeConfig();
  emit configChanged();

  return true;
}

bool
ConfigTools::loadConfigPath(const QString& file_path)
{
  bool result = true;

  // Setting Path > Env Path > Current Path > Default Path
  do {
    if (file_path == this->m_config_path)
      break;

    if (isFileExist(file_path)) {
      this->m_config_path = file_path;
      break;
    }

    if (auto env_path = m_envs.getInfo().ACROSS_CONFIG_PATH;
        !env_path.isEmpty() && isFileExist(env_path)) {
      this->m_config_path = env_path;
      break;
    }

    QDir config_dir("./");

    if (auto temp_path =
          QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        !temp_path.isEmpty())
      config_dir = temp_path;

    if (result = config_dir.mkpath("."); result == false) {
      qFatal("Failed to create config_dir");
      break;
    }

    this->m_config_path = config_dir.filePath(m_config_name);

    if (!isFileExist(this->m_config_path)) {
      saveConfig();
      result = false;
    }
  } while (false);

  return result;
}

void
ConfigTools::loadThemeConfig()
{
  auto interface_theme = p_interface->mutable_theme();

  for (auto& theme : *m_config.mutable_themes()) {
    if (interface_theme->theme() == theme.name()) {
      p_theme = &theme;
      break;
    }
  }

  freshColors();
  emit currentThemeChanged(*p_theme);
  emit trayColorChanged();
  emit trayStylishChanged();
  emit borderColorChanged();
  emit borderRadiusChanged();
  emit enableBannerChanged();
  emit backgroundImageChanged();
  emit backgroundOpacityChanged();
}

Config*
ConfigTools::configPtr()
{
  return &m_config;
}

void
ConfigTools::setInboundObject(v2ray::config::V2rayConfig& config)
{
  if (auto http_setting = p_inbound->http(); http_setting.enable()) {
    auto inbound = config.add_inbounds();
    inbound->set_listen(http_setting.listen());
    inbound->set_port(http_setting.port());
    inbound->set_protocol("http");
    inbound->set_tag("HTTP_IN");

    auto http = inbound->mutable_settings()->mutable_http();
    http->set_allowtransparent(http_setting.allow_transparent());
    http->set_timeout(http_setting.timeout());
    http->set_userlevel(http_setting.user_level());

    if (auto auth_setting = http_setting.auth(); auth_setting.enable()) {
      auto auth = http->add_accounts();
      auth->set_user(auth_setting.username());
      auth->set_pass(auth_setting.password());
    }
  }

  if (auto socks5_setting = p_inbound->socks5(); socks5_setting.enable()) {
    auto inbound = config.add_inbounds();
    inbound->set_listen(socks5_setting.listen());
    inbound->set_port(socks5_setting.port());
    inbound->set_protocol("socks");
    inbound->set_tag("SOCKS_IN");

    auto socks = inbound->mutable_settings()->mutable_socks();
    socks->set_userlevel(socks5_setting.user_level());

    if (auto auth_setting = socks5_setting.auth(); auth_setting.enable()) {
      auto auth = socks->add_accounts();
      auth->set_user(auth_setting.username());
      auth->set_pass(auth_setting.password());
    }

    if (socks5_setting.udp_enable()) {
      socks->set_udp(true);
      socks->set_ip(socks5_setting.udp_ip());
    }
  }

  if (auto api_setting = p_core->api(); api_setting.enable()) {
    auto inbound = config.add_inbounds();

    inbound->set_listen(api_setting.listen());
    inbound->set_port(api_setting.port());
    inbound->set_protocol("dokodemo-door");
    inbound->set_tag("ACROSS_API_INBOUND");

    auto dokodemo_door = inbound->mutable_settings()->mutable_dokodemo_door();
    dokodemo_door->set_address(api_setting.listen());
    dokodemo_door->set_port(api_setting.port());
  }
}

QUrl
ConfigTools::getInboundProxy()
{
  QUrl proxy;

  if (p_inbound->has_http() && p_inbound->http().enable()) {
    auto http = p_inbound->http();
    proxy.setScheme("http");
    proxy.setHost(http.listen().c_str());
    proxy.setPort(http.port());
  } else if (p_inbound->has_socks5() && p_inbound->socks5().enable()) {
    auto socks5 = p_inbound->socks5();

    proxy.setScheme("socks5");
    proxy.setHost(socks5.listen().c_str());
    proxy.setPort(socks5.port());
  }

  return proxy;
}

QString
ConfigTools::getConfigVersion()
{
  return QString::fromStdString(m_config.config_version());
}

QString
ConfigTools::getLanguage()
{
  return QString::fromStdString(m_config.interface().language());
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

  if (m_config.core().api().enable()) {
    APITools client(m_config.core().api().port());
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
  auto json_str = SerializeTools::MessageToJson(m_config);

  if (!config_path.isEmpty()) {
    ConfigHelper::saveToFile(json_str, config_path);
  } else {
    ConfigHelper::saveToFile(json_str, m_config_path);
  }
}

void
ConfigTools::checkUpdate()
{
  if (p_curl != nullptr) {
    DownloadTask task = { .name = "github_api.json",
                          .url = apiURL(),
                          .user_agent = networkUserAgent() };

    connect(p_curl.get(),
            &CURLTools::downloadFinished,
            this,
            &ConfigTools::handleUpdated);

    p_curl->download(task);
  }
}

void
ConfigTools::setDataDir(const QString& dir)
{
  auto path = QUrl(dir).toLocalFile();
  if (path.toStdString() == m_config.data_dir())
    return;

  m_config.set_data_dir(path.toStdString());
  p_db->set_db_path(QDir(path).filePath("across.db").toStdString());

  emit dbPathChanged();
  emit dataDirChanged();
  emit configChanged();
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
ConfigTools::currentThemeName()
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
    emit coreInfoChanged();
    emit coreVersionChanged();
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
  if (log_level == p_core->log_level().c_str() || log_level == "current")
    return;

  p_core->set_log_level(log_level.toStdString());
  emit configChanged();
  emit logLevelChanged();
}

void
ConfigTools::setLogLines(int log_lines)
{
  if (log_lines == m_config.log_lines())
    return;
  m_config.set_log_lines(log_lines);

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
  emit configChanged();
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
ConfigTools::dataDir()
{
  return m_config.data_dir().c_str();
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
  return m_config.log_lines();
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

bool
ConfigTools::enableStartFromMinimized()
{
  return p_interface->tray().start_from_minimize();
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
ConfigTools::setEnableStartFromMinimized(bool val)
{
  void setEnableStartFromMinimized(bool val);
  if (val == p_interface->tray().start_from_minimize())
    return;
  p_interface->mutable_tray()->set_start_from_minimize(val);
  emit configChanged();
  emit enableStartFromMinimizedChanged();
}

bool
ConfigTools::enableBanner()
{
  return p_theme->banner().enable();
}

bool
ConfigTools::enableAutoConnect()
{
  return p_interface->auto_connect();
}

bool
ConfigTools::enableAutoStart()
{
  return p_interface->auto_start();
}

void
ConfigTools::setEnableBanner(bool val)
{
  if (val == p_theme->banner().enable())
    return;
  p_theme->mutable_banner()->set_enable(val);

  emit configChanged();
  emit enableBannerChanged();
}

void
ConfigTools::setEnableAutoConnect(bool val)
{
  if (val == p_interface->auto_connect())
    return;
  p_interface->set_auto_connect(val);

  emit configChanged();
  emit enableAutoConnectChanged();
}

void
ConfigTools::setEnableAutoStart(bool val)
{
  if (val == p_interface->auto_start())
    return;
  p_interface->set_auto_start(val);

  emit configChanged();
  emit enableAutoStartChanged();
}

double
ConfigTools::backgroundOpacity()
{
  return p_theme->banner().background_opacity();
}

void
ConfigTools::setBackgroundOpacity(double val)
{
  if (qFuzzyCompare(val, p_theme->banner().background_opacity()))
    return;

  p_theme->mutable_banner()->set_background_opacity(val);

  emit configChanged();
  emit backgroundOpacityChanged();
}

QString
ConfigTools::backgroundImage()
{
    auto url = QUrl::fromLocalFile(p_theme->banner().background_image().c_str());
    if (!url.isEmpty())
        return url.url();

    return QString();
}

void
ConfigTools::setBackgroundImage(const QString& file_url)
{
    QUrl url(file_url);
    if (!url.isLocalFile())
        return ;

    auto file = QFileInfo(url.toLocalFile());
    if (!file.exists())
        return;

    auto path = file.absoluteFilePath();
    if ( path == p_theme->banner().background_image().c_str())
        return;

    p_theme->mutable_banner()->set_background_image(path.toStdString());

    emit configChanged();
    emit backgroundImageChanged();
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

void
ConfigTools::handleUpdated(const QVariant& content)
{
  if (auto task = content.value<DownloadTask>(); !task.content.isEmpty()) {

#ifdef QT_DEBUG
    QString test_content("{\"tag_name\": \"v0.1.1\"}");
    if (auto new_ver = UpdateTools::getVersion(test_content);
        new_ver.isEmpty()) {
#else
    if (auto new_ver = UpdateTools::getVersion(task.content);
        new_ver.isEmpty()) {
#endif

    } else if (UpdateTools::compareVersion(new_ver, guiVersion()) > 0)
      emit updatedChanged(new_ver);
    else {
      emit updatedChanged("");
    }
  }
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

QString
ConfigTools::apiURL()
{
  return getAPIURL();
}

QString
ConfigTools::releaseURL()
{
  return getReleaseURL();
}

void
ConfigTools::mergeConfigFromJSON(const std::string& json_str)
{
  auto origin_conf = SerializeTools::JsonToACrossConfig(json_str);
  if (!origin_conf.IsInitialized())
    return;

  QList<Theme> temp_themes;
  auto default_themes = m_config.themes();

  m_config.mutable_themes()->Clear();
  m_config.MergeFrom(origin_conf);

  for (auto& default_theme : default_themes) {
    auto iter = std::find_if(
      m_config.themes().begin(), m_config.themes().end(), [&](Theme theme) {
        if (theme.name() == default_theme.name())
          return true;
        else
          return false;
      });

    if (iter == m_config.themes().end()) {
      temp_themes.emplace_back(default_theme);
    }
  }

  for (uint i = 0; i < temp_themes.size(); ++i) {
    m_config.mutable_themes()->UnsafeArenaAddAllocated(&temp_themes[i]);
  }

  if (auto inbound = m_config.mutable_inbound(); inbound != nullptr) {
    inbound->mutable_socks5()->set_enable(
      origin_conf.inbound().socks5().enable());
    inbound->mutable_http()->set_enable(origin_conf.inbound().http().enable());
  }

  if (auto core = m_config.mutable_core(); core != nullptr) {
    core->mutable_api()->set_enable(origin_conf.core().api().enable());
  }
}

Theme*
ConfigTools::currentTheme()
{
  return p_theme;
}

QString
ConfigTools::logMode()
{
  return m_config.log_mode().c_str();
}

void
ConfigTools::setLogMode(const QString& log_mode)
{
  if (m_config.log_mode().c_str() == log_mode)
    return;

  m_config.set_log_mode(log_mode.toStdString());
  emit logModeChanged(log_mode);
}
