#include "systemtray.h"
#include <QSystemTrayIcon>

using namespace across;
using namespace across::core;

SystemTray::SystemTray(QObject *parent): QObject(parent){
    trayIcon = new QSystemTrayIcon();

}

void SystemTray::init(across::setting::ConfigTools& config,
                    CoreTools& core_tools,
                    const QIcon &acrossIconConnected,
                    const QIcon &acrossIconDisconnected){
    p_config = &config;

    p_core = &core_tools;

    connectedIcon= acrossIconConnected;
    disconnectedIcon= acrossIconDisconnected;

    trayIcon->setToolTip("Across "+p_config->guiVersion());
    trayIcon->setIcon(QIcon::fromTheme("org.arktoria.across"));
    onRunningChanged();
    trayIcon->show();

    actionToggleVisibility->setText(tr("Show"));
    actionQuit->setText(tr("Quit"));

    connect(actionToggleVisibility, &QAction::triggered, this, &SystemTray::signalShow);
    connect(actionQuit, &QAction::triggered, this, &SystemTray::signalQuit);

    rootMenu->addAction(actionToggleVisibility);
    rootMenu->addAction(actionQuit);

    trayIcon->setContextMenu(rootMenu);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    connect(p_core,SIGNAL(isRunningChanged()),this,SLOT(onRunningChanged()));
}
void SystemTray::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason){
    case QSystemTrayIcon::Trigger:
        emit signalIconActivated();
        break;
    default:
        break;
    }
}
 
void SystemTray::hideIconTray()
{
    trayIcon->hide();
}
void SystemTray::toggleVisibilitySetText(bool vis){
    if(vis==Visibility::Minimized){
        actionToggleVisibility->setText(tr("Show"));
    }else{
        actionToggleVisibility->setText(tr("Hide"));
    }
}

void SystemTray::onRunningChanged(){
    if(p_core->isRunning()){
        trayIcon->setIcon(connectedIcon);
    }else {
        trayIcon->setIcon(disconnectedIcon);
    }
}