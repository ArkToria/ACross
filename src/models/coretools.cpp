#include "coretools.h"

#include <utility>

using namespace across::core;
using namespace across::setting;
using namespace across::utils;

CoreTools::CoreTools(
    const QSharedPointer<across::acolorsapi::AColoRSAPITools> &acolors,
    QObject *parent)
    : QObject(parent) {
    this->p_acolors = acolors;

    if (p_logger = spdlog::get("core"); p_logger == nullptr) {
        qCritical("Failed to start logger");
    }
}

CoreTools::~CoreTools() {}

bool CoreTools::init(QSharedPointer<ConfigTools> config,
                     QSharedPointer<across::NotificationModel> notifications) {
    p_config = std::move(config);
    p_notifications = std::move(notifications);

    this->setIsRunning(p_acolors->core().isRunning().first);

    connect(&p_acolors->notifications(),
            &acolorsapi::AColoRSNotifications::updateCoreStatus, this,
            [&]() { this->setIsRunning(p_acolors->core().isRunning().first); });

    return true;
}

void CoreTools::setConfigByNodeID(int32_t node_id) {
    this->p_acolors->core().setConfigByNodeId(node_id);
}

int CoreTools::run() {

    if (m_running)
        this->stop();

    auto status = this->p_acolors->core().run();

    if (status.ok()) {
        p_logger->info("Core is running...");
        this->setIsRunning(true);
    } else {
        p_notifications->notify(
            tr("Core Error"),
            tr("Failed to start the process: %1")
                .arg(QString::fromStdString(status.error_message())),
            0.0, 1.0, 0.0, 2000);
        p_logger->error("Failed to start v2ray process: {}",
                        status.error_message());
    }

    return -status.error_code();
}

int CoreTools::stop() {
    if (p_acolors == nullptr)
        return -1;

    if (p_acolors->core().isRunning().first) {
        auto status = p_acolors->core().stop();

        return status.error_code();
    }

    return 0;
}

int CoreTools::restart() {
    if (auto err = this->stop(); err < 0)
        return err;

    if (auto err = this->run(); err < 0)
        return err;

    return 0;
}

bool CoreTools::isRunning() const { return m_running; }

void CoreTools::setIsRunning(bool value) {
    if (value == m_running)
        return;

    m_running = value;
    emit isRunningChanged();
}
