#include "systemtray.h"
#include <QSystemTrayIcon>
#include <cmath>

using namespace across;
using namespace across::core;
using namespace across::setting;
using namespace across::utils;

SystemTray::SystemTray(QObject* parent)
  : QObject(parent)
{
  p_tray_icon = QSharedPointer<QSystemTrayIcon>(new QSystemTrayIcon());
}

inline QString across::unitConvert(double bytes){
  if (bytes == 0.0) return QString("0 B");
  QString sizes[] = {"B","KB","MB","GB","TB","PB","EB","ZB","YB"};
  int index = int(floor(log(bytes)/log(1024)));
  return QString("%1 %2")
    .arg(bytes/pow(1024,index),0,'g',3)
    .arg(sizes[index]);
}

void
SystemTray::init(QSharedPointer<LogView> log_view,
                 QSharedPointer<across::setting::ConfigTools> config,
                 QSharedPointer<across::core::CoreTools> core,
                 QSharedPointer<across::NodeList> nodes)
{
  p_logger = std::make_shared<LogTools>(log_view, "system_tray");
  p_config = config;
  p_core = core;
  p_nodes = nodes;

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
  connect(p_nodes.get(),&across::NodeList::uploadTrafficChanged,this,
  [this](double uploadTraffic){
    this->uploadTraffic=uploadTraffic;
    onTrafficChanged();
  });
  connect(p_nodes.get(),&across::NodeList::downloadTrafficChanged,this,
  [this](double downloadTraffic){
    this->downloadTraffic=downloadTraffic;
    onTrafficChanged();
  });


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
    connectedIcon = QIcon(":misc/icons/light/tray_logo_running.svg");
    disconnectedIcon = QIcon(":misc/icons/light/tray_logo_stop.svg");
  } else {
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

inline void SystemTray::onTrafficChanged() 
{
    p_tray_icon->setToolTip("Across " + p_config->guiVersion() + "\r\n" + tr("Up: ") + unitConvert(uploadTraffic) + "/s" + tr(" Down: ") + unitConvert(downloadTraffic) + "/s");
}
