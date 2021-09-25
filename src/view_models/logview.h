#ifndef LOGVIEW_H
#define LOGVIEW_H

#include "../models/envtools.h"

#include <QDir>
#include <QMetaProperty>
#include <QObject>
#include <QQuickItem>
#include <QStandardPaths>
#include <QString>
#include <QUrl>

#include <string>

#include "spdlog/async.h"
#include "spdlog/sinks/qt_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

constexpr auto THREAD_NUMS = 2;
constexpr auto QUEUE_SIZE = 8192;
constexpr auto MAX_FILE_SIZE = 1024 * 1024 * 4;
constexpr auto MAX_LOG_FILES = 2;
constexpr auto APP_FILE_NAME = "across.log";
constexpr auto CORE_FILE_NAME = "core.log";

struct LogsInfo
{
  QString app_log;
  QString core_log;
  QString log_dir;
};

class LogView : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QQuickItem* appLogItem READ appLogItem WRITE setAppLogItem NOTIFY
               appLogItemChanged)
  Q_PROPERTY(QQuickItem* coreLogItem READ coreLogItem WRITE setCoreLogItem
               NOTIFY coreLogItemChanged)
public:
  explicit LogView(LogView* parent = nullptr);

  ~LogView();

  void init();

  void reloadSinks();

  void clean();

  std::pair<std::shared_ptr<spdlog::async_logger>,
            std::shared_ptr<spdlog::async_logger>>
  raw();

  static LogsInfo getLogsInfo();

public:
  QQuickItem* appLogItem() const;

  QQuickItem* coreLogItem() const;

  Q_INVOKABLE QString logDir();

public slots:
  void setAppLogItem(QQuickItem* newAppLogItem);

  void setCoreLogItem(QQuickItem* newCoreLogItem);

signals:
  void appLogItemChanged();

  void coreLogItemChanged();

protected:
  std::string m_app_log_path = APP_FILE_NAME;
  std::string m_core_log_path = APP_FILE_NAME;
  std::shared_ptr<spdlog::async_logger> p_app_logger;
  std::shared_ptr<spdlog::async_logger> p_core_logger;
  std::shared_ptr<spdlog::details::thread_pool> p_thread_pool;
  std::vector<spdlog::sink_ptr> app_sinks;
  std::vector<spdlog::sink_ptr> core_sinks;
  QQuickItem* p_app_text_editor = nullptr;
  QQuickItem* p_core_text_editor = nullptr;
};

#endif // LOGVIEW_H
