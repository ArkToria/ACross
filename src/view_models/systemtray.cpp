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
  {
    tray_root_menu = new QMenu;
    tray_action_toggle_visibility = new QAction(tray_root_menu);
    tray_action_start = new QAction(tray_root_menu);
    tray_action_stop = new QAction(tray_root_menu);
    tray_action_restart = new QAction(tray_root_menu);
    tray_action_quit = new QAction(tray_root_menu);

    tray_inbound_menu = new QMenu;
    {
      tray_inbound_menu_action = new QAction(this);
      tray_inbound_toggle_http = new QAction(this);
      tray_inbound_toggle_socks = new QAction(this);
    }
  }
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
          [this](const QString& upload_traffic) {
            this->upload_traffic = upload_traffic;
            onTrafficChanged();
          });
  connect(p_nodes.get(),
          &across::NodeList::downloadTrafficChanged,
          this,
          [this](const QString& download_traffic) {
            this->download_traffic = download_traffic;
            onTrafficChanged();
          });

  onRunningChanged();
  onEnableTrayChanged();
  {
    tray_action_toggle_visibility->setText(tr("Show"));
    tray_action_start->setText(tr("Connect"));
    tray_action_stop->setText(tr("Disconnect"));
    tray_action_restart->setText(tr("Reconnect"));
    tray_action_quit->setText(tr("Quit"));

    tray_action_start->setEnabled(true);
    tray_action_stop->setEnabled(false);
    tray_action_restart->setEnabled(true);

    tray_action_toggle_visibility->setIcon(QIcon::fromTheme("org.arktoria.across"));

    connect(
      tray_action_toggle_visibility, &QAction::triggered, this, &SystemTray::signalShow);
    connect(tray_action_start, &QAction::triggered, p_core.get(), &CoreTools::run);
    connect(tray_action_stop, &QAction::triggered, p_core.get(), &CoreTools::stop);
    connect(
      tray_action_restart, &QAction::triggered, p_core.get(), &CoreTools::restart);
    connect(tray_action_quit, &QAction::triggered, this, &SystemTray::signalQuit);

    tray_root_menu->addAction(tray_action_toggle_visibility);

    tray_root_menu->addSeparator();

    tray_root_menu->addAction(tray_action_start);
    tray_root_menu->addAction(tray_action_stop);
    tray_root_menu->addAction(tray_action_restart);

    tray_root_menu->addSeparator();

    tray_inbound_menu_action = tray_root_menu->addMenu(tray_inbound_menu);
    tray_inbound_menu_action->setText(tr("Inbounds"));
    {
      tray_inbound_menu->addAction(tray_inbound_toggle_http);
      tray_inbound_menu->addAction(tray_inbound_toggle_socks);
      tray_inbound_toggle_http->setText("HTTP");
      tray_inbound_toggle_socks->setText("SOCKS5");
      tray_inbound_toggle_http->setCheckable(true);
      tray_inbound_toggle_socks->setCheckable(true);
      tray_inbound_toggle_http->setChecked(p_config->httpEnable());
      tray_inbound_toggle_socks->setChecked(p_config->socksEnable());
      connect(tray_inbound_toggle_http,
              &QAction::triggered,
              p_config.get(),
              &ConfigTools::setHttpEnable);
      connect(tray_inbound_toggle_socks,
              &QAction::triggered,
              p_config.get(),
              &ConfigTools::setSocksEnable);
      connect(p_config.get(),
              &ConfigTools::httpEnableChanged,
              tray_inbound_toggle_http,
              [this]() {
                tray_inbound_toggle_http->setChecked(p_config->httpEnable());
              });
      connect(p_config.get(),
              &ConfigTools::httpEnableChanged,
              tray_inbound_toggle_socks,
              [this]() {
                tray_inbound_toggle_socks->setChecked(p_config->socksEnable());
              });
    }

    tray_root_menu->addSeparator();
    tray_root_menu->addAction(tray_action_quit);

    p_tray_icon->setContextMenu(tray_root_menu);
  }

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
    connected_icon = QIcon(":misc/icons/light/tray_logo_running.svg");
    disconnected_icon = QIcon(":misc/icons/light/tray_logo_stop.svg");
  } else {
    connected_icon = QIcon(":misc/icons/dark/tray_logo_running.svg");
    disconnected_icon = QIcon(":misc/icons/dark/tray_logo_stop.svg");
  }

  onRunningChanged();
}

void
SystemTray::iconActivated(QSystemTrayIcon::ActivationReason reason)
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
    tray_action_toggle_visibility->setText(tr("Show"));
  } else {
    tray_action_toggle_visibility->setText(tr("Hide"));
  }
}

void
SystemTray::onRunningChanged()
{
  if (p_core->isRunning()) {
    p_tray_icon->setIcon(connected_icon);
    tray_action_start->setEnabled(false);
    tray_action_stop->setEnabled(true);
  } else {
    p_tray_icon->setIcon(disconnected_icon);
    tray_action_start->setEnabled(true);
    tray_action_stop->setEnabled(false);
  }
  tray_action_restart->setEnabled(true);
}
void
SystemTray::retranslate()
{
  tray_action_toggle_visibility->setText(tr("Show"));
  tray_action_start->setText(tr("Connect"));
  tray_action_stop->setText(tr("Disconnect"));
  tray_action_restart->setText(tr("Reconnect"));
  tray_action_quit->setText(tr("Quit"));
  tray_inbound_menu_action->setText(tr("Inbounds"));
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
                   .arg(upload_traffic, download_traffic);

  p_tray_icon->setToolTip(content);
}
