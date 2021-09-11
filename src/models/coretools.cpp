#include "coretools.h"

using namespace across::core;
using namespace across::setting;

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
CoreTools::init(const Core& core_info, LogView& log_view)
{
  if (core_info.core_path.isEmpty()) {
    return false;
  }

  m_core = core_info;

  p_log_view = &log_view;

  p_process->setProcessChannelMode(QProcess::MergedChannels);

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

  if (p_log_view != nullptr) {
    p_log_view->clean();
  }

  //  if (p_log_list != nullptr) {
  //    p_log_list->clean();
  //  }

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
  auto data = p_process->readAllStandardOutput();

  //  p_log_list->append(data);

  p_log_view->append(data);
}
