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

bool CoreTools::init(QSharedPointer<ConfigTools> config) {
    p_config = std::move(config);

    this->setIsRunning(p_acolors->core().isRunning().first);
    /*
        // lambda
        auto setCore = [&]() { p_core = p_config->config()->mutable_core(); };

        auto setWorkingDirectory = [&]() {
            QFileInfo db_path(p_config->dbPath());
            QString config_dir = db_path.dir().absolutePath();
        };

        setCore();

        setWorkingDirectory();

    connect(p_config.get(), &across::setting::ConfigTools::coreInfoChanged,
            this, [=]() { setCore(); });

    connect(p_config.get(), &across::setting::ConfigTools::dbPathChanged, this,
            [=]() { setWorkingDirectory(); });

    connect(p_process.get(), SIGNAL(readyReadStandardOutput()), this,
            SLOT(onReadData()));

    connect(p_process.get(), &QProcess::stateChanged, this,
            [&](QProcess::ProcessState state) {
                if (state == QProcess::NotRunning)
                    this->setIsRunning(false);
            });
    */
    connect(&p_acolors->notifications(),
            &acolorsapi::AColoRSNotifications::updateCoreStatus, this,
            [&]() { this->setIsRunning(p_acolors->core().isRunning().first); });

    return true;
}

/*
void CoreTools::setConfig(const QString &stdin_str) {
    if (!stdin_str.isEmpty() && m_config != stdin_str)
        m_config = stdin_str;
}
*/
void CoreTools::setConfigByNodeID(int32_t node_id) {
    this->p_acolors->core().setConfigByNodeId(node_id);
}

int CoreTools::run() {
    /*
    if (m_config.isEmpty())
        return -1;
        */

    if (m_running)
        this->stop();

    /*
        p_process->start(p_core->core_path().c_str(), {"--config=stdin:"},
                         QIODevice::ReadWrite | QIODevice::Text);
        p_process->write(m_config.toUtf8());
        p_process->waitForBytesWritten();
        p_process->closeWriteChannel();
        p_process->waitForStarted();

    */
    auto status = this->p_acolors->core().run();

    if (status.ok()) {
        p_logger->info("Core is running...");
        this->setIsRunning(true);
    } else {
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

/*
void CoreTools::onReadData() {
    QString content = QString::fromUtf8(p_process->readAllStandardOutput());

    // remove datetime
    content.remove(QRegularExpression(R"((\d+/?)*\s(\d+:?)*\s)"));

    // replace warning
    if (content.contains("[Warning]")) {
        p_logger->warn("{}", content.replace("[Warning]", "").toStdString());
        return;
    }

    p_logger->info("{}", content.toStdString());
}

*/