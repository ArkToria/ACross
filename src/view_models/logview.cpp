#include "logview.h"

using namespace across;

LogView::LogView(LogView* parent)
  : QObject(parent)
{
  if (parent != nullptr) {
    p_app_logger = parent->raw().first;
    p_core_logger = parent->raw().second;
    return;
  }

  init();
}

LogView::~LogView() {}

void
LogView::init()
{
  if (auto [app_log, core_log, _] = getLogsInfo();
      !app_log.isEmpty() && !core_log.isEmpty()) {
    m_app_log_path = app_log.toStdString();
    m_core_log_path = core_log.toStdString();
  }

  spdlog::init_thread_pool(QUEUE_SIZE, THREAD_NUMS);
  p_thread_pool = spdlog::thread_pool();

  auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  auto app_rotating_sink =
    std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      m_app_log_path, MAX_FILE_SIZE, MAX_LOG_FILES);
  auto core_rotating_sink =
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
    p_app_logger->info("app logger initialize...");
  });

  connect(this, &LogView::coreLogItemChanged, this, [&]() {
    p_core_logger = std::make_shared<spdlog::async_logger>(
      "core",
      core_sinks.begin(),
      core_sinks.end(),
      p_thread_pool,
      spdlog::async_overflow_policy::block);
    p_core_logger->info("core logger initialize...");
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
  if (newAppLogItem == nullptr)
    return;

  const auto ptr =
    qvariant_cast<QQuickTextDocument*>(newAppLogItem->property("textDocument"))
      ->textDocument();

  if (m_theme.IsInitialized()) {
    appLogHighlighter.setTheme(m_theme);
  }
  appLogHighlighter.init();
  appLogHighlighter.setDocument(ptr);

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
  if (newCoreLogItem == nullptr)
    return;

  const auto ptr =
    qvariant_cast<QQuickTextDocument*>(newCoreLogItem->property("textDocument"))
      ->textDocument();

  if (m_theme.IsInitialized()) {
    coreLogHighlighter.setTheme(m_theme);
  }
  coreLogHighlighter.init();
  coreLogHighlighter.setDocument(ptr);

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

LogsInfo
LogView::getLogsInfo()
{
  QString app_file;
  QString core_file;
  QDir log_dir;

  if (auto data_home = EnvTools().getInfo().ACROSS_DATA_DIR;
      !data_home.isEmpty()) {
    log_dir = QDir(data_home).filePath("logs");
    app_file = log_dir.filePath(APP_FILE_NAME);
    core_file = log_dir.filePath(CORE_FILE_NAME);
  } else {
    QDir config_dir("./");
    if (auto temp_path = QStandardPaths::writableLocation(
          QStandardPaths::AppLocalDataLocation);
        !temp_path.isEmpty()) {
      config_dir = temp_path;
    }

    if (!config_dir.mkpath("logs")) {
      qFatal("Failed to create logs_dir");
    }

    log_dir = config_dir.filePath("logs");
    app_file = log_dir.filePath(APP_FILE_NAME);
    core_file = log_dir.filePath(CORE_FILE_NAME);
  }

  return { app_file, core_file, log_dir.absolutePath() };
}

QString
LogView::logDir()
{
  return QUrl::fromLocalFile(getLogsInfo().log_dir).toString();
}

void
LogView::setTheme(const across::config::Theme& theme)
{
  m_theme.Clear();
  m_theme.CopyFrom(theme);

  setAppLogItem(p_app_text_editor);
  setCoreLogItem(p_core_text_editor);
}
