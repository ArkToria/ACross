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

  p_tray_icon->setToolTip(titleString() + inboundString());

  connect(
    p_config.get(),
    &across::setting::ConfigTools::socksEnableChanged,
    this,
    [this]() { p_tray_icon->setToolTip(titleString() + inboundString()); });

  connect(
    p_config.get(),
    &across::setting::ConfigTools::socksPortChanged,
    this,
    [this]() { p_tray_icon->setToolTip(titleString() + inboundString()); });

  connect(
    p_config.get(),
    &across::setting::ConfigTools::httpEnableChanged,
    this,
    [this]() { p_tray_icon->setToolTip(titleString() + inboundString()); });

  connect(
    p_config.get(),
    &across::setting::ConfigTools::httpPortChanged,
    this,
    [this]() { p_tray_icon->setToolTip(titleString() + inboundString()); });

  connect(p_nodes.get(),
          &across::NodeList::uploadTrafficChanged,
          this,
          [this](const QString& uploadTraffic) {
            this->uploadTraffic = uploadTraffic;
            onTrafficChanged();
          });
  connect(p_nodes.get(),
          &across::NodeList::downloadTrafficChanged,
          this,
          [this](const QString& downloadTraffic) {
            this->downloadTraffic = downloadTraffic;
            onTrafficChanged();
          });

  onRunningChanged();
  onEnableTrayChanged();

  actionToggleVisibility->setText(tr("Show"));
  actionStart->setText(tr("Connect"));
  actionStop->setText(tr("Disconnect"));
  actionRestart->setText(tr("Reconnect"));
  actionQuit->setText(tr("Quit"));

  actionStart->setEnabled(true);
  actionStop->setEnabled(false);
  actionRestart->setEnabled(true);

  actionToggleVisibility->setIcon(QIcon::fromTheme("org.arktoria.across"));

  connect(
    actionToggleVisibility, &QAction::triggered, this, &SystemTray::signalShow);
  connect(actionStart, &QAction::triggered, p_core.get(), &CoreTools::run);
  connect(actionStop, &QAction::triggered, p_core.get(), &CoreTools::stop);
  connect(
    actionRestart, &QAction::triggered, p_core.get(), &CoreTools::restart);
  connect(actionQuit, &QAction::triggered, this, &SystemTray::signalQuit);

  rootMenu->addAction(actionToggleVisibility);

  rootMenu->addSeparator();

  rootMenu->addAction(actionStart);
  rootMenu->addAction(actionStop);
  rootMenu->addAction(actionRestart);

  rootMenu->addSeparator();

  inboundMenuAction = rootMenu->addMenu(inboundMenu);
  inboundMenuAction->setText(tr("Inbounds"));
  {
    inboundMenu->addAction(actionInboundToggleHttp);
    inboundMenu->addAction(actionInboundToggleSocks);
    actionInboundToggleHttp->setText("HTTP");
    actionInboundToggleSocks->setText("SOCKS5");
    actionInboundToggleHttp->setCheckable(true);
    actionInboundToggleSocks->setCheckable(true);
    actionInboundToggleHttp->setChecked(p_config->httpEnable());
    actionInboundToggleSocks->setChecked(p_config->socksEnable());
    connect(actionInboundToggleHttp, &QAction::triggered, p_config.get(), &ConfigTools::setHttpEnable);
    connect(actionInboundToggleSocks, &QAction::triggered, p_config.get(), &ConfigTools::setSocksEnable);
    connect(p_config.get(), &ConfigTools::httpEnableChanged, actionInboundToggleHttp, [this]() {
      actionInboundToggleHttp->setChecked(p_config->httpEnable());
    });
    connect(p_config.get(), &ConfigTools::httpEnableChanged, actionInboundToggleSocks, [this]() {
      actionInboundToggleSocks->setChecked(p_config->socksEnable());
    });
  }

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
  switch (reason) {
    case QSystemTrayIcon::Trigger:
      emit signalIconActivated();
      break;
    default:
      break;
  }
}

void
SystemTray::toggleVisibilitySetText(bool vis)
{
  if (vis == Visibility::Minimized) {
    actionToggleVisibility->setText(tr("Show"));
  } else {
    actionToggleVisibility->setText(tr("Hide"));
  }
}

void
SystemTray::onRunningChanged()
{
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
  inboundMenuAction->setText(tr("Inbounds"));
}

QString
SystemTray::titleString()
{
  return QString("ACross %1\n").arg(p_config->guiVersion());
}

QString
SystemTray::inboundString()
{
  QString socks5_port, http_port = tr("OFF");

  if (p_config->socksEnable()) {
    socks5_port = p_config->socksPort();
  }

  if (p_config->httpEnable()) {
    http_port = p_config->httpPort();
  }

  return QString("socks5: %1 | "
                 "http: %2\n")
    .arg(socks5_port, http_port);
}

void
SystemTray::onEnableTrayChanged()
{
  if (p_config->enableTray()) {
    p_tray_icon->show();
  } else {
    p_tray_icon->hide();
  }
}

void
SystemTray::onTrafficChanged()
{
  auto content = titleString() + inboundString() +
                 QString("↑ %1/s"
                         "↓ %2/s")
                   .arg(uploadTraffic, downloadTraffic);

  p_tray_icon->setToolTip(content);
}
