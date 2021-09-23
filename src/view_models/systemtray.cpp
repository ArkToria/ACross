#include "systemtray.h"
#include <QSystemTrayIcon>

using namespace across;
using namespace across::core;
using namespace across::setting;

SystemTray::SystemTray(QObject* parent)
  : QObject(parent)
{
  p_tray_icon = QSharedPointer<QSystemTrayIcon>(new QSystemTrayIcon());
}

void
SystemTray::init(QSharedPointer<across::setting::ConfigTools> config,
                 QSharedPointer<across::core::CoreTools> core)
{
  p_config = config;
  p_core = core;

  connect(
    p_config.get(),
    &across::setting::ConfigTools::trayColorChanged,
    this,
    [&]() { loadTrayIcons(p_config->trayStylish(), p_config->trayColor()); });

  connect(
    p_config.get(),
    &across::setting::ConfigTools::trayStylishChanged,
    this,
    [&]() { loadTrayIcons(p_config->trayStylish(), p_config->trayColor()); });

  connect(p_config.get(),
          &across::setting::ConfigTools::enableTrayChanged,
          this,
          &SystemTray::onEnableTrayChanged);

  loadTrayIcons(p_config->trayStylish(), p_config->trayColor());

  p_tray_icon->setToolTip("Across " + p_config->guiVersion());
  onRunningChanged();
  onEnableTrayChanged();

  actionToggleVisibility->setText(tr("Show"));
  actionStart->setText(tr("Connect"));
  actionStop->setText(tr("Disconnect"));
  actionRestart->setText(tr("Reconnect"));
  actionQuit->setText(tr("Quit"));

  actionStart->setEnabled(false);
  actionStop->setEnabled(false);
  actionRestart->setEnabled(false);

  actionToggleVisibility->setIcon(QIcon::fromTheme("org.arktoria.across"));

  connect(
    actionToggleVisibility, &QAction::triggered, this, &SystemTray::signalShow);
  connect(actionStart, &QAction::triggered,[this] {p_core->run();});
  connect(actionStop, &QAction::triggered,[this] {p_core->stop();});
  connect(actionRestart, &QAction::triggered,[this]
  {p_core->stop();p_core->run();});
  connect(actionQuit, &QAction::triggered, this, &SystemTray::signalQuit);

  rootMenu->addAction(actionToggleVisibility);
  rootMenu->addSeparator();
  rootMenu->addAction(actionStart);
  rootMenu->addAction(actionStop);
  rootMenu->addAction(actionRestart);
  rootMenu->addSeparator();
  rootMenu->addAction(actionQuit);

  p_tray_icon->setContextMenu(rootMenu);
  connect(p_tray_icon.get(),
          SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this,
          SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

  connect(p_core.get(),
          &CoreTools::isRunningChanged,
          this,
          &SystemTray::onRunningChanged);
}

void
SystemTray::loadTrayIcons(const QString& stylish, const QString& color)
{
  if (p_config->trayColor() == "light") {
    /*
    connectedIcon = QIcon::fromTheme("org.arktoria.across.light.running.svg");
    disconnectedIcon = QIcon::fromTheme("org.arktoria.across.light.stop.svg");
    */
    connectedIcon = QIcon(":misc/icons/light/tray_logo_running.svg");
    disconnectedIcon = QIcon(":misc/icons/light/tray_logo_stop.svg");
  } else {
    /*
    connectedIcon = QIcon::fromTheme("org.arktoria.across.dark.running.svg");
    disconnectedIcon = QIcon::fromTheme("org.arktoria.across.dark.stop.svg");
    */
    connectedIcon = QIcon(":misc/icons/dark/tray_logo_running.svg");
    disconnectedIcon = QIcon(":misc/icons/dark/tray_logo_stop.svg");
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
  if (p_core->isRunning()) {
    p_tray_icon->setIcon(connectedIcon);
    actionStart->setEnabled(false);
    actionStop->setEnabled(true);
  } else {
    p_tray_icon->setIcon(disconnectedIcon);
    actionStart->setEnabled(true);
    actionStop->setEnabled(false);
  }
    actionRestart->setEnabled(true);
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
     p_tray_icon->show();
   }else{
     p_tray_icon->hide();
   }
}
