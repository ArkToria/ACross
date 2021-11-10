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
class SystemTray : public QObject {
    Q_OBJECT
  public:
    enum Visibility { Minimized = false, Visible = true };
    explicit SystemTray(QObject *parent = 0);

    void init(QSharedPointer<across::setting::ConfigTools> config,
              QSharedPointer<across::core::CoreTools> core,
              QSharedPointer<across::NodeList> nodes);

    void loadTrayIcons(const QString &stylish = "", const QString &color = "");
    void retranslate();

    QString titleString();
    QString inboundString();

    QSharedPointer<QSystemTrayIcon> getTrayIcon() {
      return p_tray_icon;
    };

    Q_INVOKABLE void toggleVisibilitySetText(bool vis);
    Q_INVOKABLE static inline bool isSystemTrayAvailable() {
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

    std::shared_ptr<spdlog::logger> p_logger;

    QString upload_traffic = "";
    QString download_traffic = "";

    QIcon connected_icon;
    QIcon disconnected_icon;

    QMenu *tray_root_menu;

    QAction *tray_action_toggle_visibility;
    QAction *tray_action_start;
    QAction *tray_action_stop;
    QAction *tray_action_restart;
    QAction *tray_action_quit;
};
} // namespace across

#endif
