#include "systemtray.h"
#include <QSystemTrayIcon>

using namespace across;
using namespace across::core;
using namespace across::setting;

SystemTray::SystemTray(QObject *parent): QObject(parent){
    trayIcon = new QSystemTrayIcon();
}

void
SystemTray::init(ConfigTools& config, CoreTools& core_tools)
{
  p_config = &config;
  p_core = &core_tools;

  connect(
    p_config, &across::setting::ConfigTools::trayColorChanged, this, [&]() {
      loadTrayIcons(p_config->trayStylish(), p_config->trayColor());
    });

  connect(
    p_config, &across::setting::ConfigTools::trayStylishChanged, this, [&]() {
      loadTrayIcons(p_config->trayStylish(), p_config->trayColor());
    });

  connect(
    p_config, &across::setting::ConfigTools::enableTrayChanged, this, &SystemTray::onEnableTrayChanged);

  loadTrayIcons(p_config->trayStylish(), p_config->trayColor());

  trayIcon->setToolTip("Across " + p_config->guiVersion());
  onRunningChanged();
  onEnableTrayChanged();

  actionToggleVisibility->setText(tr("Show"));
  actionStart->setText(tr("Connect"));
  actionStop->setText(tr("Disconnect"));
  actionRestart->setText(tr("Reconnect"));
  actionQuit->setText(tr("Quit"));

  actionToggleVisibility->setIcon(QIcon::fromTheme("org.arktoria.across"));

  connect(
    actionToggleVisibility, &QAction::triggered, this, &SystemTray::signalShow);
  /*
  connect(actionStart, &QAction::triggered,[this] {p_core->run();});
  connect(actionStop, &QAction::triggered,[this] {p_core->stop();});
  connect(actionRestart, &QAction::triggered,[this]
  {p_core->stop();p_core->run();});
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
  connect(trayIcon,
          SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this,
          SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

  connect(
    p_core, &CoreTools::isRunningChanged, this, &SystemTray::onRunningChanged);
}

void
SystemTray::loadTrayIcons(const QString& stylish, const QString& color)
{
  if (p_config->trayColor() == "light") {
    connectedIcon = QIcon::fromTheme("org.arktoria.across.light.running.svg");
    disconnectedIcon = QIcon::fromTheme("org.arktoria.across.light.stop.svg");
  } else {
    connectedIcon = QIcon::fromTheme("org.arktoria.across.dark.running.svg");
    disconnectedIcon = QIcon::fromTheme("org.arktoria.across.dark.stop.svg");
  }

  onRunningChanged();
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
void SystemTray::retranslate(){
    actionToggleVisibility->setText(tr("Show"));
    actionStart->setText(tr("Connect"));
    actionStop->setText(tr("Disconnect"));
    actionRestart->setText(tr("Reconnect"));
    actionQuit->setText(tr("Quit"));
}

void SystemTray::onEnableTrayChanged(){
   if(p_config->enableTray()){
     trayIcon->show();
   }else{
     trayIcon->hide();
   }
}