#ifndef CONFIGTOOLS_H
#define CONFIGTOOLS_H

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHostAddress>
#include <QObject>
#include <QRegExp>
#include <QSharedPointer>
#include <QString>
#include <QTextStream>
#include <QUrl>
#include <toml++/toml.h>
#include <wordexp.h>

#include "../models/apitools.h"
#include "../models/envtools.h"
#include "../models/jsontools.h"
#include "../models/logtools.h"
#include "version_config.h"

namespace across {
namespace setting {
struct Interface
{
  struct Language {
    QString language = "current";

    void fromNodeView(toml::v2::node_view<toml::v2::node> language);
    void toNodeView(const toml::v2::node_view<toml::v2::node>& language);
  } language;

  struct Theme
  {
    QString theme = "current";
    QString include_dir = "./themes/";

    void fromNodeView(toml::v2::node_view<toml::v2::node> theme);
    void toNodeView(const toml::v2::node_view<toml::v2::node>& theme);
  } theme;

  void fromNodeView(toml::v2::node_view<toml::v2::node> interface);
  void toNodeView(const toml::v2::node_view<toml::v2::node>& interface);
};

struct Update
{
  bool auto_update = false;
  bool check_update = true;
  QString update_channel = "stable";
  bool update_from_proxy = true;

  void fromNodeView(toml::v2::node_view<toml::v2::node> update);
  void toNodeView(const toml::v2::node_view<toml::v2::node>& update);
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

  void fromNodeView(toml::v2::node_view<toml::v2::node> database);
  void toNodeView(const toml::v2::node_view<toml::v2::node>& database);
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

  void fromNodeView(toml::v2::node_view<toml::v2::node> core);
  void toNodeView(const toml::v2::node_view<toml::v2::node>& core);
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

    void fromNodeView(toml::v2::node_view<toml::v2::node> socks);
    void toNodeView(const toml::v2::node_view<toml::v2::node>& socks);
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

    void fromNodeView(toml::v2::node_view<toml::v2::node> http);

    void toNodeView(const toml::v2::node_view<toml::v2::node>& http);

    across::config::InboundObject toInboundObject();
  } http;

  void fromNodeView(toml::v2::node_view<toml::v2::node> inbound);

  void toNodeView(const toml::v2::node_view<toml::v2::node>& inbound);

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
    QString themeName READ themeName WRITE setThemeName NOTIFY themeNameChanged)
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
public:
  explicit ConfigTools(QObject* parent = nullptr);

  bool init(std::shared_ptr<spdlog::details::thread_pool> thread_pool,
            QString file_path = "");

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
  QString themeName();

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

  int borderRadius();

  int borderWidth();

  int itemSpacing();

  const QString& iconStyle() const;

  const QString& currentTheme() const;

  const QString& currentLanguage() const;

public slots:
  void setDBPath(const QString& db_path);

  void setCorePath(const QUrl& val);

  void setAssetsPath(const QUrl& val);

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

  void setThemeName(const QString& val);

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

  void setBorderRadius(int radius);

  void setBorderWidth(int width);

  void setItemSpacing(int spacing);

  void setIconStyle(const QString& newIconStyle);

  void setCurrentTheme(const QString& newCurrentTheme);

  void setCurrentLanguage(const QString& newCurrentLanguage);

signals:
  void dbPathChanged();

  void coreInfoChanged();

  void coreVersionChanged();

  void corePathChanged();

  void assetsPathChanged();

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

  void themeNameChanged();

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

  void borderRadiusChanged();

  void borderWidthChanged();

  void itemSpacingChanged();

  void iconStyleChanged();

  void currentThemeChanged();

  void currentLanguageChanged(const QString& lang);

private:
  std::shared_ptr<LogTools> p_logger;
  const QString m_config_name = "across.toml";
  QString m_config_path = "./" + m_config_name;
  QString m_api_result_text = "";
  toml::v2::table m_config;

  Core m_core;
  DataBase m_db;
  EnvTools m_envs;
  Theme m_theme;
  Update m_update;
  Interface m_interface;
  InboundSettings m_inbound;
};
}
}

#endif // CONFIGTOOLS_H
