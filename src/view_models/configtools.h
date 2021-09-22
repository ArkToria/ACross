#ifndef CONFIGTOOLS_H
#define CONFIGTOOLS_H

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHostAddress>
#include <QObject>
#include <QRegularExpression>
#include <QSharedPointer>
#include <QString>
#include <QTextStream>
#include <QUrl>
#include <toml++/toml.h>
#include <wordexp.h>

#include "../models/apitools.h"
#include "../models/envtools.h"
#include "../models/jsontools.h"
#include "buildinfo.h"
#include "logtools.h"

namespace across {
namespace setting {
struct Table
{

  template<typename T>
  static void fromNodeView(
    toml::v2::node_view<toml::node> node,
    const toml::v2::node_view<toml::node>& default_config,
    T& config,
    const std::string& key);
  template<typename T>
  static void toNodeView(toml::v2::node_view<toml::node> node,
                         const toml::v2::node_view<toml::node>& default_config,
                         const T& config,
                         const std::string& key);
};
struct Interface
{
  struct Language
  {
    QString language = "current";

    void fromNodeView(toml::v2::node_view<toml::v2::node> language,
                      const toml::v2::node_view<toml::node>& default_config);
    void toNodeView(const toml::v2::node_view<toml::v2::node>& language,
                    const toml::v2::node_view<toml::node>& default_config);
  } language;

  struct Theme
  {
    QString theme = "current";
    QString include_dir = "./themes/";

    void fromNodeView(toml::v2::node_view<toml::v2::node> theme,
                      const toml::v2::node_view<toml::node>& default_config);
    void toNodeView(const toml::v2::node_view<toml::v2::node>& theme,
                    const toml::v2::node_view<toml::node>& default_config);
    static void fromNodeView(
      toml::v2::node_view<toml::node> theme,
      const toml::v2::node_view<toml::node>& default_config,
      QString& config,
      const std::string& key);
    static void toNodeView(
      toml::v2::node_view<toml::node> tray,
      const toml::v2::node_view<toml::node>& default_config,
      const QString& config,
      const std::string& key);
  } theme;

  struct Tray
  {
    bool enable = false;

    void fromNodeView(toml::v2::node_view<toml::v2::node> tray,
                      const toml::v2::node_view<toml::node>& default_config);
    void toNodeView(const toml::v2::node_view<toml::v2::node>& tray,
                    const toml::v2::node_view<toml::node>& default_config);
    static void fromNodeView(
      toml::v2::node_view<toml::node> theme,
      const toml::v2::node_view<toml::node>& default_config,
      bool& config,
      const std::string& key);
    static void toNodeView(
      toml::v2::node_view<toml::node> theme,
      const toml::v2::node_view<toml::node>& default_config,
      const bool& config,
      const std::string& key);
  } tray;

  template<typename T>
  static void fromNodeView(
    toml::v2::node_view<toml::node> interface,
    const toml::v2::node_view<toml::node>& default_config,
    T& config,
    const std::string& key);
  template<typename T>
  static void toNodeView(toml::v2::node_view<toml::node> interface,
                         const toml::v2::node_view<toml::node>& default_config,
                         T& config,
                         const std::string& key);
  void fromNodeView(toml::v2::node_view<toml::v2::node> interface,
                    const toml::v2::node_view<toml::node>& default_config);
  void toNodeView(const toml::v2::node_view<toml::v2::node>& interface,
                  const toml::v2::node_view<toml::node>& default_config);
};

struct Network
{
  QString test_method = "tcping";
  QString test_url = "https://www.google.com";
  QString user_agent = "Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 "
                       "(KHTML, like Gecko) Chrome/99.0.7113.93 Safari/537.36";

  void fromNodeView(toml::v2::node_view<toml::v2::node> network,
                    const toml::v2::node_view<toml::node>& default_config);
  void toNodeView(const toml::v2::node_view<toml::v2::node>& network,
                  const toml::v2::node_view<toml::node>& default_config);
};

struct Update
{
  bool auto_update = false;
  bool check_update = true;
  QString update_channel = "stable";
  bool update_from_proxy = true;

  void fromNodeView(toml::v2::node_view<toml::v2::node> update,
                    const toml::v2::node_view<toml::node>& default_config);
  void toNodeView(const toml::v2::node_view<toml::v2::node>& update,
                  const toml::v2::node_view<toml::node>& default_config);
};

struct DataBase
{
  QString path = "./across.db";
  QString backend = "sqlite3";

  struct Auth
  {
    bool enable = false;
    QString username = "";
    QString password = "";
    QString address = "";
    uint port = 0;
  } auth;

  void fromNodeView(toml::v2::node_view<toml::v2::node> database,
                    const toml::v2::node_view<toml::node>& default_config);
  void toNodeView(const toml::v2::node_view<toml::v2::node>& database,
                  const toml::v2::node_view<toml::node>& default_config);
};

struct Core
{
  QString core_path = "";
  QString assets_path = "";
  QString log_level = "warning";
  int log_lines = 500;
  QString core_version = "";

  struct API
  {
    bool enable = false;
    uint port = 15491;
  } api;

  void fromNodeView(toml::v2::node_view<toml::v2::node> core,
                    const toml::v2::node_view<toml::node>& default_config);
  void toNodeView(const toml::v2::node_view<toml::v2::node>& core,
                  const toml::v2::node_view<toml::node>& default_config);
};

struct Theme
{
  QString name = "dark";

  struct Tray
  {
    QString stylish = "line";
    QString color = "dark";
  } tray;

  struct Banner
  {
    bool enable = false;
    QString background_image = "";
    float background_opacity = 1.0;
  } banner;

  struct Border
  {
    int radius = 8;
    int width = 1;
  } border;

  struct Item
  {
    int spacing = 8;
  } item;

  struct Icon
  {
    QString style = "dark";
  } icon;

  struct Colors
  {
    QString text_color = "#f3f3f3";
    QString background_color = "#3b4252";
    QString highlight_color = "#5e81ac";
    QString highlight_text_color = "#eceff4";
    QString warn_color = "#bf616a";
    QString warn_text_color = "#ffffff";
    QString shadow_color = "#29000000";
    QString border_color = "#3381a1c1";
    QString deep_color = "#2e3440";
    QString deep_text_color = "#ffffff";
    QString style_color = "#bbde5e";
    QString style_text_color = "#ffffff";

    void fromNodeView(toml::v2::node_view<toml::v2::node> colors_node);
  };

  Colors colors;
};

struct InboundSettings
{
  struct SOCKS
  {
    bool enable;

    QString listen = "127.0.0.1";
    uint port = 1089;
    const QString protocol = "socks";

    bool udp = false;
    QString ip = "";
    int user_level = 0;

    QString username; // Leave blank to disable
    QString password; // Need to be encrypted

    void fromNodeView(toml::v2::node_view<toml::v2::node> socks,
                      const toml::v2::node_view<toml::node>& default_config);
    void toNodeView(const toml::v2::node_view<toml::v2::node>& socks,
                    const toml::v2::node_view<toml::node>& default_config);
    across::config::InboundObject toInboundObject();
  } socks;

  struct HTTP
  {
    bool enable;

    QString listen = "127.0.0.1";
    uint port = 8888;
    const QString protocol = "http";

    bool allow_transparent = false;
    int timeout = 0;
    int user_level = 0;

    QString username; // Leave blank to disable
    QString password; // Need to be encrypted

    void fromNodeView(toml::v2::node_view<toml::v2::node> http,
                      const toml::v2::node_view<toml::node>& default_config);
    void toNodeView(const toml::v2::node_view<toml::v2::node>& http,
                    const toml::v2::node_view<toml::node>& default_config);
    across::config::InboundObject toInboundObject();
  } http;

  void fromNodeView(toml::v2::node_view<toml::v2::node> inbound,
                    const toml::v2::node_view<toml::node>& default_config);

  void toNodeView(const toml::v2::node_view<toml::v2::node>& inbound,
                  const toml::v2::node_view<toml::node>& default_config);

  void setObject(Json::Value& root);
};

class ConfigTools : public QObject
{
  Q_OBJECT
  // database setting
  Q_PROPERTY(QString dbPath READ dbPath WRITE setDBPath NOTIFY dbPathChanged)

  // core setting
  Q_PROPERTY(QString coreInfo READ coreInfo NOTIFY coreInfoChanged)
  Q_PROPERTY(QString coreVersion READ coreVersion NOTIFY coreVersionChanged)
  Q_PROPERTY(QString guiVersion READ guiVersion CONSTANT)
  Q_PROPERTY(
    QString corePath READ corePath WRITE setCorePath NOTIFY corePathChanged)
  Q_PROPERTY(QString assetsPath READ assetsPath WRITE setAssetsPath NOTIFY
               assetsPathChanged)
  Q_PROPERTY(
    QString logLevel READ logLevel WRITE setLogLevel NOTIFY logLevelChanged)
  Q_PROPERTY(
    int logLines READ logLines WRITE setLogLines NOTIFY logLinesChanged)
  Q_PROPERTY(
    bool apiEnable READ apiEnable WRITE setApiEnable NOTIFY apiEnableChanged)
  Q_PROPERTY(
    QString apiPort READ apiPort WRITE setApiPort NOTIFY apiPortChanged)
  Q_PROPERTY(
    QString apiResultText READ apiResultText NOTIFY apiResultTextChanged)

  // inbounds setting
  Q_PROPERTY(QString inboundAddress READ inboundAddress WRITE setInboundAddress
               NOTIFY inboundAddressChanged)
  Q_PROPERTY(bool socksEnable READ socksEnable WRITE setSocksEnable NOTIFY
               socksEnableChanged)
  Q_PROPERTY(
    QString socksPort READ socksPort WRITE setSocksPort NOTIFY socksPortChanged)
  Q_PROPERTY(QString socksUsername READ socksUsername WRITE setSocksUsername
               NOTIFY socksUsernameChanged)
  Q_PROPERTY(QString socksPassword READ socksPassword WRITE setSocksPassword
               NOTIFY socksPasswordChanged)
  Q_PROPERTY(bool httpEnable READ httpEnable WRITE setHttpEnable NOTIFY
               httpEnableChanged)
  Q_PROPERTY(
    QString httpPort READ httpPort WRITE setHttpPort NOTIFY httpPortChanged)
  Q_PROPERTY(QString httpUsername READ httpUsername WRITE setHttpUsername NOTIFY
               httpUsernameChanged)
  Q_PROPERTY(QString httpPassword READ httpPassword WRITE setHttpPassword NOTIFY
               httpPasswordChanged)

  // user interface setting
  Q_PROPERTY(
    QString textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
  Q_PROPERTY(QString backgroundColor READ backgroundColor WRITE
               setBackgroundColor NOTIFY backgroundColorChanged)
  Q_PROPERTY(QString highlightColor READ highlightColor WRITE setHighlightColor
               NOTIFY highlightColorChanged)
  Q_PROPERTY(QString highlightTextColor READ highlightTextColor WRITE
               setHighlightTextColor NOTIFY highlightTextColorChanged)
  Q_PROPERTY(
    QString warnColor READ warnColor WRITE setWarnColor NOTIFY warnColorChanged)
  Q_PROPERTY(QString warnTextColor READ warnTextColor WRITE setWarnTextColor
               NOTIFY warnTextColorChanged)
  Q_PROPERTY(QString shadowColor READ shadowColor WRITE setShadowColor NOTIFY
               shadowColorChanged)
  Q_PROPERTY(QString borderColor READ borderColor WRITE setBorderColor NOTIFY
               borderColorChanged)
  Q_PROPERTY(
    QString deepColor READ deepColor WRITE setDeepColor NOTIFY deepColorChanged)
  Q_PROPERTY(QString deepTextColor READ deepTextColor WRITE setDeepTextColor
               NOTIFY deepTextColorChanged)
  Q_PROPERTY(QString styleColor READ styleColor WRITE setStyleColor NOTIFY
               styleColorChanged)
  Q_PROPERTY(QString styleTextColor READ styleTextColor WRITE setStyleTextColor
               NOTIFY styleTextColorChanged)
  Q_PROPERTY(QString trayStylish READ trayStylish WRITE setTrayStylish NOTIFY
               trayStylishChanged)
  Q_PROPERTY(
    QString trayColor READ trayColor WRITE setTrayColor NOTIFY trayColorChanged)
  Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY
               borderRadiusChanged)
  Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY
               borderWidthChanged)
  Q_PROPERTY(int itemSpacing READ itemSpacing WRITE setItemSpacing NOTIFY
               itemSpacingChanged)
  Q_PROPERTY(
    QString iconStyle READ iconStyle WRITE setIconStyle NOTIFY iconStyleChanged)
  Q_PROPERTY(QString currentTheme READ currentTheme WRITE setCurrentTheme NOTIFY
               currentThemeChanged)
  Q_PROPERTY(QString currentLanguage READ currentLanguage WRITE
               setCurrentLanguage NOTIFY currentLanguageChanged)
  Q_PROPERTY(bool enableTray READ enableTray WRITE setEnableTray NOTIFY
               enableTrayChanged)

  // network setting
  Q_PROPERTY(QString networkTestMethod READ networkTestMethod WRITE
               setNetworkTestMethod NOTIFY networkTestMethodChanged)
  Q_PROPERTY(QString networkTestURL READ networkTestURL WRITE setNetworkTestURL
               NOTIFY networkTestURLChanged)
  Q_PROPERTY(QString networkUserAgent READ networkUserAgent WRITE
               setNetworkUserAgent NOTIFY networkUserAgentChanged)

  // help page
  Q_PROPERTY(QString buildInfo READ buildInfo CONSTANT)
  Q_PROPERTY(QString extraInfo READ extraInfo CONSTANT)
  Q_PROPERTY(QString buildTime READ buildTime CONSTANT)
  Q_PROPERTY(QString sourceCodeURL READ sourceCodeURL CONSTANT)
  Q_PROPERTY(QString reportURL READ reportURL CONSTANT)
  Q_PROPERTY(QString licenseURL READ licenseURL CONSTANT)

public:
  explicit ConfigTools(QObject* parent = nullptr);

  bool init(QSharedPointer<LogView> logview, const QString& file_path = "");

  QString loadConfigPath(const QString& file_path);

  toml::v2::table getConfig();

  Core getCore();
  bool loadInterfaceConfig();
  bool loadInterfaceTheme();
  bool loadInterfaceLanguage();
  bool loadUpdateConfig();
  bool loadDBConfig();
  bool loadThemeConfig();
  bool loadCoreConfig();
  bool loadInboundConfig();
  bool loadNetworkConfig();

  InboundSettings getInboundConfig();
  DataBase getDBConfig();

  Q_INVOKABLE QString getConfigVersion();
  Q_INVOKABLE QString getConfigTomlVersion();
  Q_INVOKABLE QString getLanguage();
  Q_INVOKABLE void freshColors();
  Q_INVOKABLE bool testApi();
  Q_INVOKABLE bool testAndSetAddr(const QString& addr);
  Q_INVOKABLE void freshInbound();
  Q_INVOKABLE bool saveConfig(QString config_path = "");

  static bool isFileExist(QString file_path);

public:
  // database setting
  QString dbPath();

  // core setting
  QString coreInfo();
  QString coreVersion();
  QString guiVersion();
  QString corePath();
  QString assetsPath();
  QString logLevel();
  int logLines();
  bool apiEnable();
  QString apiPort();
  QString apiResultText();

  // inbounds setting
  QString inboundAddress();
  bool socksEnable();
  QString socksPort();
  QString socksUsername();
  QString socksPassword();
  bool httpEnable();
  QString httpPort();
  QString httpUsername();
  QString httpPassword();

  // user interface setting
  QString textColor();
  QString backgroundColor();
  QString highlightColor();
  QString highlightTextColor();
  QString warnColor();
  QString warnTextColor();
  QString shadowColor();
  QString borderColor();
  QString deepColor();
  QString deepTextColor();
  QString styleColor();
  QString styleTextColor();
  QString trayStylish();
  QString trayColor();
  int borderRadius();
  int borderWidth();
  int itemSpacing();
  const QString& iconStyle() const;
  const QString& currentTheme() const;
  const QString& currentLanguage() const;
  bool enableTray();

  // network setting
  const QString& networkTestMethod() const;
  const QString& networkTestURL() const;
  const QString& networkUserAgent() const;

  // help page
  QString buildInfo();
  QString extraInfo();
  QString buildTime();
  QString sourceCodeURL();
  QString reportURL();
  QString licenseURL();

public slots:
  void setDBPath(const QString& db_path, bool init = false);
  void setCorePath(const QUrl& val);
  void setAssetsPath(const QUrl& val);
  void setLogLevel(const QString& log_level);
  void setLogLines(int log_lines);
  void setApiEnable(bool val);
  void setApiPort(QString& portStr);
  void setInboundAddress(const QString& addr);
  void setSocksEnable(bool val);
  void setSocksPort(const QString& portStr);
  void setSocksUsername(const QString& name);
  void setSocksPassword(const QString& pass);
  void setHttpEnable(bool val);
  void setHttpPort(QString& portStr);
  void setHttpUsername(const QString& name);
  void setHttpPassword(const QString& pass);
  void setTextColor(const QString& val);
  void setBackgroundColor(const QString& val);
  void setHighlightColor(const QString& val);
  void setHighlightTextColor(const QString& val);
  void setWarnColor(const QString& val);
  void setWarnTextColor(const QString& val);
  void setShadowColor(const QString& val);
  void setBorderColor(const QString& val);
  void setDeepColor(const QString& val);
  void setDeepTextColor(const QString& val);
  void setStyleColor(const QString& val);
  void setStyleTextColor(const QString& val);
  void setTrayStylish(const QString& val);
  void setTrayColor(const QString& val);
  void setBorderRadius(int radius);
  void setBorderWidth(int width);
  void setItemSpacing(int spacing);
  void setIconStyle(const QString& newIconStyle);
  void setCurrentTheme(const QString& newCurrentTheme);
  void setCurrentLanguage(const QString& newCurrentLanguage);
  void setEnableTray(bool val);

  // network setting
  void setNetworkTestMethod(const QString& val);
  void setNetworkTestURL(const QString& val);
  void setNetworkUserAgent(const QString& val);

signals:
  void dbPathChanged();
  void coreInfoChanged();
  void coreVersionChanged();
  void corePathChanged();
  void assetsPathChanged();
  void logLevelChanged();
  void logLinesChanged(int max_lines);
  void apiEnableChanged();
  void apiPortChanged();
  void apiResultTextChanged();
  void inboundAddressChanged();
  void socksEnableChanged();
  void socksPortChanged();
  void socksUsernameChanged();
  void socksPasswordChanged();
  void httpEnableChanged();
  void httpPortChanged();
  void httpUsernameChanged();
  void httpPasswordChanged();
  void textColorChanged();
  void backgroundColorChanged();
  void highlightColorChanged();
  void highlightTextColorChanged();
  void warnColorChanged();
  void warnTextColorChanged();
  void shadowColorChanged();
  void borderColorChanged();
  void deepColorChanged();
  void deepTextColorChanged();
  void styleColorChanged();
  void styleTextColorChanged();
  void trayStylishChanged();
  void trayColorChanged();
  void borderRadiusChanged();
  void borderWidthChanged();
  void itemSpacingChanged();
  void iconStyleChanged();
  void currentThemeChanged();
  void currentLanguageChanged(const QString& lang);
  void enableTrayChanged();
  void buildInfoChanged();
  void configChanged();

  // network setting
  void networkTestMethodChanged();
  void networkTestURLChanged();
  void networkUserAgentChanged();

private:
  std::shared_ptr<across::utils::LogTools> p_logger;
  const QString m_config_name = "across.toml";
  QString m_config_path = "./" + m_config_name;
  QString m_api_result_text = "";
  toml::v2::table m_config;
  toml::v2::table m_default_config;

  Core m_core;
  DataBase m_db;
  EnvTools m_envs;
  Theme m_theme;
  Update m_update;
  Interface m_interface;
  Network m_network;
  InboundSettings m_inbound;
};
}
}

#endif // CONFIGTOOLS_H
