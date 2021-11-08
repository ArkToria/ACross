#include "logview.h"

using namespace across;

LogView::LogView(QObject* parent)
  : QObject(parent)
{
  spdlog::init_thread_pool(QUEUE_SIZE, THREAD_NUMS);

  auto stdout_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();

  std::vector<spdlog::sink_ptr> sinks;
  sinks.emplace_back(stdout_sink);

  auto p_logger = std::make_shared<spdlog::async_logger>(
    "app",
    sinks.begin(),
    sinks.end(),
    spdlog::thread_pool(),
    spdlog::async_overflow_policy::block);

  if (spdlog::get("app") == nullptr) {
    auto core_logger = p_logger->clone("core");

    spdlog::register_logger(p_logger);
    spdlog::register_logger(core_logger);
  }
}

LogView::~LogView()
{
  spdlog::drop_all();
  spdlog::shutdown();
}

// LogsInfo
// LogView::getLogsInfo()
//{
//   QString app_file;
//   QString core_file;
//   QDir log_dir;

//  if (auto data_home = EnvTools().getInfo().ACROSS_DATA_DIR;
//      !data_home.isEmpty()) {
//    log_dir = QDir(data_home).filePath("logs");
//    app_file = log_dir.filePath(APP_FILE_NAME);
//    core_file = log_dir.filePath(CORE_FILE_NAME);
//  } else {
//    QDir config_dir("./");
//    if (auto temp_path = QStandardPaths::writableLocation(
//          QStandardPaths::AppLocalDataLocation);
//        !temp_path.isEmpty()) {
//      config_dir = temp_path;
//    }

//    if (!config_dir.mkpath("logs")) {
//      qFatal("Failed to create logs_dir");
//    }

//    log_dir = config_dir.filePath("logs");
//    app_file = log_dir.filePath(APP_FILE_NAME);
//    core_file = log_dir.filePath(CORE_FILE_NAME);
//  }

//  return { app_file, core_file, log_dir.absolutePath() };
//}

// QString
// LogView::logDir()
//{
//   return QUrl::fromLocalFile(getLogsInfo().log_dir).toString();
// }

void
LogView::setTheme(const across::config::Theme& theme)
{
  m_theme.Clear();
  m_theme.CopyFrom(theme);
}

void
LogView::setLogItem(QQuickItem* item, const QString& name)
{
  if (item == nullptr)
    return;

  if (auto logger = spdlog::get(name.toStdString()); logger != nullptr) {
    spdlog::sink_ptr qt_sink =
      std::make_shared<spdlog::sinks::qt_sink_mt>(item, "append");

    logger->sinks().emplace_back(qt_sink);
    logger->info("Adding qt sinks to logger: {}, Sinks size: {}",
                 logger->name(),
                 logger->sinks().size());

    const auto p_item =
      qvariant_cast<QQuickTextDocument*>(item->property("textDocument"))
        ->textDocument();

    if (logger->name() == "core") {
      if (m_theme.IsInitialized()) {
        m_core_highlighter.setTheme(m_theme);
      }

      m_core_highlighter.init();
      m_core_highlighter.setDocument(p_item);
      return;
    } else if (logger->name() == "app") {
      if (m_theme.IsInitialized()) {
        m_app_highlighter.setTheme(m_theme);
      }

      m_app_highlighter.init();
      m_app_highlighter.setDocument(p_item);
      return;
    }
  }
}
