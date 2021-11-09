#ifndef LOGVIEW_H
#define LOGVIEW_H

#include "../models/envtools.h"

#include <QDir>
#include <QMetaProperty>
#include <QObject>
#include <QQuickItem>
#include <QQuickTextDocument>
#include <QStandardPaths>
#include <QString>
#include <QUrl>

#include <string>

#include "loghighlighter.h"

#include "spdlog/async.h"
#include "spdlog/sinks/qt_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

constexpr auto THREAD_NUMS = 2;
constexpr auto QUEUE_SIZE = 8192;
constexpr auto MAX_FILE_SIZE = 1024 * 1024 * 4;
constexpr auto MAX_LOG_FILES = 2;
constexpr auto AUTO_FLUSH_TIME = 10; // seconds
constexpr auto APP_FILE_NAME = "across.log";
constexpr auto CORE_FILE_NAME = "core.log";

struct LogsInfo {
    QString app_log;
    QString core_log;
    QString log_dir;
};

class LogView : public QObject {
    Q_OBJECT
  public:
    explicit LogView(QObject *parent = nullptr);

    ~LogView();

    Q_INVOKABLE void setLogItem(QQuickItem *item, const QString &name = "app");

  public slots:
    void setTheme(const across::config::Theme &theme);

  protected:
    across::LogHighlighter m_core_highlighter;
    across::LogHighlighter m_app_highlighter;

    across::config::Theme m_theme;
};

#endif // LOGVIEW_H
