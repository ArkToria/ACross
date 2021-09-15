#include "coretools.h"

using namespace across::core;
using namespace across::setting;
using namespace across::utils;

CoreTools::CoreTools(QObject* parent)
{
  p_process = new QProcess(parent);
}

CoreTools::~CoreTools()
{
  this->stop();

  if (p_process != nullptr) {
    delete p_process;
    p_process = nullptr;
  }
}

bool
CoreTools::init(LogView& log_view, ConfigTools& config)
{
  p_config = &config;

  auto setCore = [&]() { m_core = p_config->getCore(); };

  auto setWorkingDirectory = [&]() {
    QFileInfo db_path = p_config->dbPath();
    QString config_dir = db_path.dir().absolutePath();

    p_process->setWorkingDirectory(config_dir);
  };

  setCore();

  setWorkingDirectory();

  p_logger = std::make_shared<LogTools>(log_view, "core", LoggerEnum::core);

  p_process->setProcessChannelMode(QProcess::MergedChannels);

  connect(p_config,
          &across::setting::ConfigTools::coreInfoChanged,
          this,
          [=]() { setCore(); });

  connect(p_config, &across::setting::ConfigTools::dbPathChanged, this, [=]() {
    setWorkingDirectory();
  });

  connect(
    p_process, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadData()));

  return true;
}

void
CoreTools::setConfig(const QString& stdin_str)
{
  if (!stdin_str.isEmpty() && m_config != stdin_str) {
    m_config = stdin_str;
  }
}

void
CoreTools::setConfig(const std::string& stdin_str)
{
  auto config = QString::fromStdString(stdin_str);

  setConfig(config);
}

int
CoreTools::run()
{
  if (m_config.isEmpty()) {
    return -1;
  }

  if (p_process == nullptr) {
    p_process = new QProcess();

    p_process->setProcessChannelMode(QProcess::MergedChannels);
    connect(
     p_process, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadData()));
  }
  if (m_running)
    this->stop();

  p_process->setProcessEnvironment(m_env);

  p_process->start(
    m_core.core_path, {}, QIODevice::ReadWrite | QIODevice::Text);

  p_process->write(m_config.toUtf8());

  p_process->waitForBytesWritten();

  p_process->closeWriteChannel();

  p_process->waitForStarted();

  int exit_code = p_process->exitCode();

  if (exit_code != 0) {
    qCritical() << "Failed to start v2ray process";
  } else {
    this->setIsRunning(true);
  }

  return exit_code;
}

int
CoreTools::stop()
{
  if (p_process != nullptr &&
      p_process->state() == QProcess::ProcessState::Running) {
    p_process->kill();

    p_process->waitForFinished();

    this->setIsRunning(false);

    return p_process->exitCode();
  }

  return -1;
}

bool
CoreTools::isRunning()
{
  return m_running;
}

void
CoreTools::setIsRunning(bool value)
{
  if (value == m_running) {
    return;
  }

  m_running = value;

  emit isRunningChanged();
}

void
CoreTools::onReadData()
{
  QString content = QString::fromUtf8(p_process->readAllStandardOutput());
  content.append("<br/>");

  if (content.contains("[Warn]")) {
    p_logger->warn("{}", content.toStdString());

    return;
  }

  p_logger->info("{}", content.toStdString());
}
