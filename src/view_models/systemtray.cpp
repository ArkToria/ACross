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
    onRunningChanged();
    trayIcon->show();

    actionToggleVisibility->setText(tr("Show"));
    actionStart->setText(tr("Connect"));
    actionStop->setText(tr("Disconnect"));
    actionRestart->setText(tr("Reconnect"));
    actionQuit->setText(tr("Quit"));

    actionToggleVisibility->setIcon(this->trayIcon->icon());

    connect(actionToggleVisibility, &QAction::triggered, this, &SystemTray::signalShow);
    /*
    connect(actionStart, &QAction::triggered,[this] {p_core->run();});
    connect(actionStop, &QAction::triggered,[this] {p_core->stop();});
    connect(actionRestart, &QAction::triggered,[this] {p_core->stop();p_core->run();});
    */
    connect(actionQuit, &QAction::triggered, this, &SystemTray::signalQuit);

    rootMenu->addAction(actionToggleVisibility);
    rootMenu->addSeparator();
    rootMenu->addAction(actionStart);
    rootMenu->addAction(actionStop);
    rootMenu->addAction(actionRestart);
    rootMenu->addSeparator();
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
        actionStart->setEnabled(false);
        actionStop->setEnabled(true);
    }else {
        trayIcon->setIcon(disconnectedIcon);
        actionStart->setEnabled(true);
        actionStop->setEnabled(false);
    }
}