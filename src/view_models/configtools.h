#ifndef CONFIGTOOLS_H
#define CONFIGTOOLS_H

#include "../models/apitools.h"
#include "../models/confighelper.h"
#include "../models/envtools.h"
#include "../models/networktools.h"
#include "../models/serializetools.h"
#include "buildinfo.h"
#include "logtools.h"

#include "nlohmann/json.hpp"

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

#ifdef Q_OS_WIN
#else
#include <wordexp.h>
#endif

namespace across {
namespace setting {
using Json = nlohmann::json;

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
  Q_PROPERTY(QString currentThemeName READ currentThemeName WRITE setCurrentTheme NOTIFY
               currentThemeChanged)
  Q_PROPERTY(QString currentLanguage READ currentLanguage WRITE
               setCurrentLanguage NOTIFY currentLanguageChanged)
  Q_PROPERTY(bool enableTray READ enableTray WRITE setEnableTray NOTIFY
               enableTrayChanged)
  Q_PROPERTY(
    bool enableStartFromMinimized READ enableStartFromMinimized WRITE
      setEnableStartFromMinimized NOTIFY enableStartFromMinimizedChanged)
  Q_PROPERTY(bool enableBanner READ enableBanner WRITE setEnableBanner NOTIFY
               enableBannerChanged)
  Q_PROPERTY(bool enableAutoConnect READ enableAutoConnect WRITE
               setEnableAutoConnect NOTIFY enableAutoConnectChanged)
  Q_PROPERTY(bool enableAutoStart READ enableAutoStart WRITE setEnableAutoStart
               NOTIFY enableAutoStartChanged)
  Q_PROPERTY(QString backgroundImage READ backgroundImage WRITE
               setBackgroundImage NOTIFY backgroundImageChanged)
  Q_PROPERTY(double backgroundOpacity READ backgroundOpacity WRITE
               setBackgroundOpacity NOTIFY backgroundOpacityChanged)

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
  Q_PROPERTY(QString apiURL READ apiURL CONSTANT)
  Q_PROPERTY(QString releaseURL READ releaseURL CONSTANT)

public:
  explicit ConfigTools(QObject* parent = nullptr);
  bool init(QSharedPointer<across::network::CURLTools> curl,
            const QString& file_path = "");
  bool loadConfigPath(const QString& file_path = "");
  void loadThemeConfig();
  across::config::Config* configPtr();
  void setInboundObject(v2ray::config::V2rayConfig& config);
  void mergeConfigFromJSON(const std::string& json_str);
  across::config::Theme* currentTheme();

  Q_INVOKABLE QString getConfigVersion();
  Q_INVOKABLE QString getLanguage();
  Q_INVOKABLE void freshColors();
  Q_INVOKABLE bool testAPI();
  Q_INVOKABLE bool testAndSetAddr(const QString& addr);
  Q_INVOKABLE void freshInbound();
  Q_INVOKABLE void saveConfig(QString config_path = "");
  Q_INVOKABLE void checkUpdate();

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
  QString iconStyle();
  QString currentThemeName();
  QString currentLanguage();
  bool enableTray();
  bool enableStartFromMinimized();
  bool enableBanner();
  bool enableAutoConnect();
  bool enableAutoStart();
  QString backgroundImage();
  double backgroundOpacity();

  // network setting
  QString networkTestMethod();
  QString networkTestURL();
  QString networkUserAgent();

  // help page
  QString buildInfo();
  QString extraInfo();
  QString buildTime();
  QString sourceCodeURL();
  QString reportURL();
  QString licenseURL();
  QString apiURL();
  QString releaseURL();

public slots:
  void setDBPath(const QString& val = "", bool init = false);
  void setCorePath(const QUrl& val);
  void setAssetsPath(const QUrl& val);
  void setLogLevel(const QString& val);
  void setLogLines(int val);
  void setApiEnable(bool val);
  void setApiPort(QString& val);
  void setInboundAddress(const QString& val);
  void setSocksEnable(bool val);
  void setSocksPort(const QString& val);
  void setSocksUsername(const QString& val);
  void setSocksPassword(const QString& val);
  void setHttpEnable(bool val);
  void setHttpPort(QString& val);
  void setHttpUsername(const QString& val);
  void setHttpPassword(const QString& val);
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
  void setBorderRadius(int val);
  void setBorderWidth(int val);
  void setItemSpacing(int val);
  void setIconStyle(const QString& val);
  void setCurrentTheme(const QString& val);
  void setCurrentLanguage(const QString& val);
  void setEnableTray(bool val);
  void setEnableStartFromMinimized(bool val);
  void setEnableBanner(bool val);
  void setEnableAutoConnect(bool val);
  void setEnableAutoStart(bool val);
  void setBackgroundImage(const QString& val);
  void setBackgroundOpacity(double val);

  // network setting
  void setNetworkTestMethod(const QString& val);
  void setNetworkTestURL(const QString& val);
  void setNetworkUserAgent(const QString& val);

  // help page
  void handleUpdated(const QVariant& content);

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
  void enableStartFromMinimizedChanged();
  void enableAutoConnectChanged();
  void enableAutoStartChanged();
  void buildInfoChanged();
  void configChanged();
  void enableBannerChanged();
  void backgroundImageChanged();
  void backgroundOpacityChanged();

  // network setting
  void networkTestMethodChanged();
  void networkTestURLChanged();
  void networkUserAgentChanged();

  // help page
  void updatedChanged(const QString& version);

private:
  const QString m_config_name = "across.json";
  QString m_config_path = "./" + m_config_name;
  QString m_api_result_text = "";
  QList<across::config::Theme> temp_themes;

  EnvTools m_envs;
  QSharedPointer<across::network::CURLTools> p_curl;
  across::config::Config m_conf = ConfigHelper::defaultConfig();
  across::config::Core* p_core;
  across::config::Database* p_db;
  across::config::Interface* p_interface;
  across::config::Theme* p_theme;
  across::config::Inbound* p_inbound;
  across::config::Network* p_network;
};
}
}

#endif // CONFIGTOOLS_H
