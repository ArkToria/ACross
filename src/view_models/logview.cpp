#include "logview.h"

LogView::LogView(LogView* parent)
  : QObject(parent)
{
  if (parent != nullptr) {
    p_app_logger = parent->raw().first;
    p_core_logger = parent->raw().first;
    return;
  }

  init();
}

LogView::~LogView() {}

void
LogView::init()
{
  spdlog::init_thread_pool(QUEUE_SIZE, THREAD_NUMS);
  p_thread_pool = spdlog::thread_pool();

  spdlog::sink_ptr stdout_sink =
    std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  spdlog::sink_ptr app_rotating_sink =
    std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      m_app_log_path, MAX_FILE_SIZE, MAX_LOG_FILES);
  spdlog::sink_ptr core_rotating_sink =
    std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      m_core_log_path, MAX_FILE_SIZE, MAX_LOG_FILES);

  app_sinks.emplace_back(stdout_sink);
  app_sinks.emplace_back(app_rotating_sink);

  core_sinks.emplace_back(stdout_sink);
  core_sinks.emplace_back(core_rotating_sink);

  reloadSinks();
}

void
LogView::reloadSinks()
{
  connect(this, &LogView::appLogItemChanged, this, [&]() {
    p_app_logger = std::make_shared<spdlog::async_logger>(
      "app",
      app_sinks.begin(),
      app_sinks.end(),
      p_thread_pool,
      spdlog::async_overflow_policy::block);
  });

  connect(this, &LogView::coreLogItemChanged, this, [&]() {
    p_core_logger = std::make_shared<spdlog::async_logger>(
      "core",
      core_sinks.begin(),
      core_sinks.end(),
      p_thread_pool,
      spdlog::async_overflow_policy::block);
  });
}

void
LogView::clean()
{
  if (p_app_text_editor != nullptr) {
    p_app_text_editor->metaObject()->invokeMethod(
      p_app_text_editor, "clear", Qt::AutoConnection);
  }

  if (p_core_text_editor != nullptr) {
    p_core_text_editor->metaObject()->invokeMethod(
      p_core_text_editor, "clear", Qt::AutoConnection);
  }
}

QQuickItem*
LogView::appLogItem() const
{
  return p_app_text_editor;
}

void
LogView::setAppLogItem(QQuickItem* newAppLogItem)
{
  if (p_app_text_editor == newAppLogItem)
    return;

  p_app_text_editor = newAppLogItem;

  spdlog::sink_ptr app_qt_sink = std::make_shared<spdlog::sinks::qt_sink_mt>(
    this->p_app_text_editor, "append");

  app_sinks.emplace_back(app_qt_sink);

  emit appLogItemChanged();
}

QQuickItem*
LogView::coreLogItem() const
{
  return p_core_text_editor;
}

void
LogView::setCoreLogItem(QQuickItem* newCoreLogItem)
{
  if (p_core_text_editor == newCoreLogItem)
    return;

  p_core_text_editor = newCoreLogItem;

  spdlog::sink_ptr core_qt_sink = std::make_shared<spdlog::sinks::qt_sink_mt>(
    this->p_core_text_editor, "append");

  core_sinks.emplace_back(core_qt_sink);

  emit coreLogItemChanged();
}

std::pair<std::shared_ptr<spdlog::async_logger>,
          std::shared_ptr<spdlog::async_logger>>
LogView::raw()
{
  return { this->p_app_logger, this->p_core_logger };
}
