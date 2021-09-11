#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H
#include "../models/coretools.h"
#include "../view_models/configtools.h"

#include <QObject>
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>

namespace across {
class SystemTray : public QObject {
    Q_OBJECT
public:
    enum Visibility{
        Minimized=false,
        Visible=true
    };
    explicit SystemTray(QObject* parent = 0);

    void init(across::setting::ConfigTools& config,
              across::core::CoreTools& core_tools);

    void loadTrayIcons(const QString& stylish = "", const QString& color = "");

    Q_INVOKABLE void toggleVisibilitySetText(bool vis);
    void retranslate();

signals:
    void signalIconActivated();
    void signalShow();
    void signalQuit();

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

public slots:
    void hideIconTray();
    void onRunningChanged();

private:
    QSystemTrayIcon *trayIcon;
    across::setting::ConfigTools* p_config;
    across::core::CoreTools* p_core;
    QIcon connectedIcon;
    QIcon disconnectedIcon;


#define DECL_ACTION(parent, name) QAction *name = new QAction(parent)
    QMenu *rootMenu = new QMenu();

    DECL_ACTION(rootMenu, actionToggleVisibility);
    DECL_ACTION(rootMenu, actionStart);
    DECL_ACTION(rootMenu, actionStop);
    DECL_ACTION(rootMenu, actionRestart);
    DECL_ACTION(rootMenu, actionQuit);
};
}

#endif
