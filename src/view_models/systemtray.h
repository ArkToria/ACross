#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H
#include "../models/coretools.h"
#include "../view_models/configtools.h"
#include "../view_models/logtools.h"
#include "../view_models/nodelist.h"

#include <QAction>
#include <QMenu>
#include <QObject>
#include <QSystemTrayIcon>

namespace across {
class SystemTray : public QObject
{
  Q_OBJECT
public:
  enum Visibility
  {
    Minimized = false,
    Visible = true
  };
  explicit SystemTray(QObject* parent = 0);

  void init(QSharedPointer<LogView> log_view,
            QSharedPointer<across::setting::ConfigTools> config,
            QSharedPointer<across::core::CoreTools> core,
            QSharedPointer<across::NodeList> nodes);
  void loadTrayIcons(const QString& stylish = "", const QString& color = "");
  void retranslate();

  Q_INVOKABLE void toggleVisibilitySetText(bool vis);
  Q_INVOKABLE static inline bool isSystemTrayAvailable()
  {
    return QSystemTrayIcon::isSystemTrayAvailable();
  };
public slots:
  void onRunningChanged();
  void onEnableTrayChanged();
  void onTrafficChanged();

private slots:
  void iconActivated(QSystemTrayIcon::ActivationReason reason);

signals:
  void signalIconActivated();
  void signalShow();
  void signalQuit();

private:
  QSharedPointer<QSystemTrayIcon> p_tray_icon;
  QSharedPointer<across::setting::ConfigTools> p_config;
  QSharedPointer<across::core::CoreTools> p_core;
  QSharedPointer<across::NodeList> p_nodes;
  QSharedPointer<across::core::APITools> p_api;

  std::shared_ptr<across::utils::LogTools> p_logger;

  QString uploadTraffic = "";
  QString downloadTraffic = "";

  QIcon connectedIcon;
  QIcon disconnectedIcon;

#define DECL_ACTION(parent, name) QAction* name = new QAction(parent)
  QMenu* rootMenu = new QMenu();

  DECL_ACTION(rootMenu, actionToggleVisibility);
  DECL_ACTION(rootMenu, actionStart);
  DECL_ACTION(rootMenu, actionStop);
  DECL_ACTION(rootMenu, actionRestart);
  DECL_ACTION(rootMenu, actionQuit);
};
}

#endif
