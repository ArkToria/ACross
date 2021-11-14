#include "coretools.h"

using namespace across::core;
using namespace across::setting;
using namespace across::utils;

CoreTools::CoreTools(QObject *parent) { p_process = new QProcess(parent); }

CoreTools::~CoreTools() {
    this->stop();
    if (p_process != nullptr) {
        delete p_process;
        p_process = nullptr;
    }
}

bool CoreTools::init(QSharedPointer<ConfigTools> config) {
    p_logger = spdlog::get("core");
    if (p_logger == nullptr) {
        qCritical("Failed to start logger");
        return false;
    }

    p_config = config;

    p_process->setProcessChannelMode(QProcess::MergedChannels);

    // lambda
    auto setCore = [&]() { p_core = p_config->config()->mutable_core(); };

    auto setWorkingDirectory = [&]() {
        QFileInfo db_path(p_config->dbPath());
        QString config_dir = db_path.dir().absolutePath();

        p_process->setWorkingDirectory(config_dir);
    };

    setCore();

    setWorkingDirectory();

    connect(p_config.get(), &across::setting::ConfigTools::coreInfoChanged,
            this, [=]() { setCore(); });

    connect(p_config.get(), &across::setting::ConfigTools::dbPathChanged, this,
            [=]() { setWorkingDirectory(); });

    connect(p_process, SIGNAL(readyReadStandardOutput()), this,
            SLOT(onReadData()));

    connect(p_process, &QProcess::stateChanged, this,
            [&](QProcess::ProcessState state) {
                if (state == QProcess::NotRunning)
                    this->setIsRunning(false);
            });

    return true;
}

void CoreTools::setConfig(const QString &stdin_str) {
    if (!stdin_str.isEmpty() && m_config != stdin_str)
        m_config = stdin_str;
}

int CoreTools::run() {
    if (m_config.isEmpty())
        return -1;

    if (m_running)
        this->stop();

    if (p_process == nullptr) {
        p_process = new QProcess();
        p_process->setProcessChannelMode(QProcess::MergedChannels);

        connect(p_process, SIGNAL(readyReadStandardOutput()), this,
                SLOT(onReadData()));

        connect(p_process, &QProcess::stateChanged, this,
                [&](QProcess::ProcessState state) {
                    if (state == QProcess::NotRunning) {
                        this->setIsRunning(false);
                    }
                });
    }

    p_process->setProcessEnvironment(m_env);
    p_process->start(p_core->core_path().c_str(), {},
                     QIODevice::ReadWrite | QIODevice::Text);
    p_process->write(m_config.toUtf8());
    p_process->waitForBytesWritten();
    p_process->closeWriteChannel();
    p_process->waitForStarted();

    auto exit_code = p_process->exitCode();
    if (exit_code != 0 || p_process->state() == QProcess::NotRunning) {
        p_logger->error("Failed to start v2ray process");
    } else {
        p_logger->info("Core is running...");
        this->setIsRunning(true);
    }

    return exit_code;
}

int CoreTools::stop() {
    if (p_process != nullptr &&
        p_process->state() == QProcess::ProcessState::Running) {
        p_process->kill();
        p_process->waitForFinished();

        this->setIsRunning(false);

        return p_process->exitCode();
    }

    return -1;
}

int CoreTools::restart() {
    if (auto err = this->stop(); err < 0)
        return err;

    if (auto err = this->run(); err < 0)
        return err;

    return 0;
}

bool CoreTools::isRunning() { return m_running; }

void CoreTools::setIsRunning(bool value) {
    if (value == m_running)
        return;

    m_running = value;
    emit isRunningChanged();
}

void CoreTools::onReadData() {
    QString content = QString::fromUtf8(p_process->readAllStandardOutput());

    // remove datetime
    content.remove(QRegularExpression("(\\d+/?)*\\s(\\d+:?)*\\s"));

    // replace warning
    if (content.contains("[Warning]")) {
        p_logger->warn("{}", content.replace("[Warning]", "").toStdString());
        return;
    }

    p_logger->info("{}", content.toStdString());
}
