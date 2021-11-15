#include "systemtray.h"
#include <QSystemTrayIcon>
#include <cmath>

using namespace across;
using namespace across::core;
using namespace across::setting;
using namespace across::utils;

SystemTray::SystemTray(QObject *parent) : QObject(parent) {
    p_tray_icon = QSharedPointer<QSystemTrayIcon>::create();

    tray_root_menu = QSharedPointer<QMenu>::create();
    tray_action_toggle_visibility =
        QSharedPointer<QAction>::create(tray_root_menu.get());
    tray_action_start = QSharedPointer<QAction>::create(tray_root_menu.get());
    tray_action_stop = QSharedPointer<QAction>::create(tray_root_menu.get());
    tray_action_restart = QSharedPointer<QAction>::create(tray_root_menu.get());
    tray_action_quit = QSharedPointer<QAction>::create(tray_root_menu.get());
}

SystemTray::~SystemTray() {}

void SystemTray::init(QSharedPointer<across::setting::ConfigTools> config,
                      QSharedPointer<across::core::CoreTools> core,
                      QSharedPointer<across::NodeList> nodes) {
    if (auto app_logger = spdlog::get("app"); app_logger != nullptr) {
        p_logger = app_logger->clone("tray");
    } else {
        qCritical("Failed to start logger");
        return;
    }

    p_core = core;
    p_nodes = nodes;
    p_config = config;

    // connect config changed signals
    connect(p_config.get(), &across::setting::ConfigTools::trayColorChanged,
            this, [&]() {
                loadTrayIcons(p_config->trayStylish(), p_config->trayColor());
            });

    connect(p_config.get(), &across::setting::ConfigTools::trayStylishChanged,
            this, [&]() {
                loadTrayIcons(p_config->trayStylish(), p_config->trayColor());
            });

    connect(p_config.get(), &across::setting::ConfigTools::enableTrayChanged,
            this, &SystemTray::onEnableTrayChanged);

    connect(
        p_config.get(), &across::setting::ConfigTools::socksEnableChanged, this,
        [this]() { p_tray_icon->setToolTip(titleString() + inboundString()); });

    connect(
        p_config.get(), &across::setting::ConfigTools::socksPortChanged, this,
        [this]() { p_tray_icon->setToolTip(titleString() + inboundString()); });

    connect(
        p_config.get(), &across::setting::ConfigTools::httpEnableChanged, this,
        [this]() { p_tray_icon->setToolTip(titleString() + inboundString()); });

    connect(
        p_config.get(), &across::setting::ConfigTools::httpPortChanged, this,
        [this]() { p_tray_icon->setToolTip(titleString() + inboundString()); });

    connect(p_nodes.get(), &across::NodeList::uploadTrafficChanged, this,
            [this](const QString &upload_traffic) {
                this->upload_traffic = upload_traffic;
                onTrafficChanged();
            });
    connect(p_nodes.get(), &across::NodeList::downloadTrafficChanged, this,
            [this](const QString &download_traffic) {
                this->download_traffic = download_traffic;
                onTrafficChanged();
            });

    connect(p_tray_icon.get(),
            SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    connect(p_core.get(), &CoreTools::isRunningChanged, this,
            &SystemTray::onRunningChanged);

    this->setTrayMenu();
    this->loadTrayIcons(p_config->trayStylish(), p_config->trayColor());
    p_tray_icon->setToolTip(titleString() + inboundString());

    onRunningChanged();
    onEnableTrayChanged();
}

void SystemTray::setTrayMenu() {
    connect(tray_action_toggle_visibility.get(), &QAction::triggered, this,
            &SystemTray::signalShow);
    connect(tray_action_start.get(), &QAction::triggered, p_core.get(),
            &CoreTools::run);
    connect(tray_action_stop.get(), &QAction::triggered, p_core.get(),
            &CoreTools::stop);
    connect(tray_action_restart.get(), &QAction::triggered, p_core.get(),
            &CoreTools::restart);
    connect(tray_action_quit.get(), &QAction::triggered, this,
            &SystemTray::signalQuit);

    retranslate();

    tray_action_start->setEnabled(true);
    tray_action_stop->setEnabled(false);
    tray_action_restart->setEnabled(true);
    tray_action_toggle_visibility->setIcon(
        QIcon::fromTheme("org.arktoria.across"));

    tray_root_menu->addAction(tray_action_toggle_visibility.get());
    tray_root_menu->addSeparator();
    tray_root_menu->addAction(tray_action_start.get());
    tray_root_menu->addAction(tray_action_stop.get());
    tray_root_menu->addAction(tray_action_restart.get());
    tray_root_menu->addSeparator();
    tray_root_menu->addAction(tray_action_quit.get());

    p_tray_icon->setContextMenu(tray_root_menu.get());
}

void SystemTray::loadTrayIcons(const QString &stylish, const QString &color) {
    if (p_config->trayColor() == "light") {
        connected_icon = QIcon(":misc/icons/light/tray_logo_running.svg");
        disconnected_icon = QIcon(":misc/icons/light/tray_logo_stop.svg");
    } else {
        connected_icon = QIcon(":misc/icons/dark/tray_logo_running.svg");
        disconnected_icon = QIcon(":misc/icons/dark/tray_logo_stop.svg");
    }

    onRunningChanged();
}

void SystemTray::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        emit signalIconActivated();
        break;
    default:
        break;
    }
}

void SystemTray::toggleVisibilitySetText(bool vis) {
    if (vis == Visibility::Minimized) {
        tray_action_toggle_visibility->setText(tr("Show"));
    } else {
        tray_action_toggle_visibility->setText(tr("Hide"));
    }
}

void SystemTray::onRunningChanged() {
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

void SystemTray::retranslate() {
    tray_action_toggle_visibility->setText(tr("Show"));
    tray_action_start->setText(tr("Connect"));
    tray_action_stop->setText(tr("Disconnect"));
    tray_action_restart->setText(tr("Reconnect"));
    tray_action_quit->setText(tr("Quit"));
}

QString SystemTray::titleString() {
    return QString("ACross %1\n").arg(p_config->guiVersion());
}

QString SystemTray::inboundString() {
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

void SystemTray::onEnableTrayChanged() {
    if (p_config->enableTray()) {
        p_tray_icon->show();
    } else {
        p_tray_icon->hide();
    }
}

void SystemTray::onTrafficChanged() {
    auto content = titleString() + inboundString() +
                   QString("↑ %1/s"
                           "↓ %2/s")
                       .arg(upload_traffic, download_traffic);

    p_tray_icon->setToolTip(content);
}
