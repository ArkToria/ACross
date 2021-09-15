#include "logview.h"

LogView::LogView(LogView* parent)
  : QObject(parent)
{
  if (parent != nullptr) {
    p_logger = parent->raw();
    return;
  }
}

LogView::~LogView() {}

void
LogView::init()
{
  spdlog::init_thread_pool(QUEUE_SIZE, THREAD_NUMS);
  p_thread_pool = spdlog::thread_pool();

  spdlog::sink_ptr stdout_sink =
    std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  spdlog::sink_ptr rotating_sink =
    std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      m_path.toStdString(), MAX_FILE_SIZE, MAX_LOG_FILES);

  if (this->p_text_editor != nullptr) {
    spdlog::sink_ptr qt_sink = std::make_shared<spdlog::sinks::qt_sink_mt>(
      this->p_text_editor, "append");
    sinks.emplace_back(qt_sink);
  }

  sinks.emplace_back(stdout_sink);
  sinks.emplace_back(rotating_sink);

  p_logger = std::make_shared<spdlog::async_logger>(
    "app",
    sinks.begin(),
    sinks.end(),
    p_thread_pool,
    spdlog::async_overflow_policy::block);
}

void
LogView::clean()
{
  if (p_text_editor != nullptr) {
    p_text_editor->metaObject()->invokeMethod(
      p_text_editor, "clear", Qt::AutoConnection);
  }
}

QQuickItem*
LogView::textEditor() const
{
  return p_text_editor;
}

void
LogView::setTextEditor(QQuickItem* newTextEditor)
{
  if (p_text_editor == newTextEditor)
    return;

  p_text_editor = newTextEditor;

  init();
  emit textEditorChanged(p_text_editor);
}

std::shared_ptr<spdlog::async_logger>
LogView::raw()
{
  return this->p_logger;
}
