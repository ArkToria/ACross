#ifndef LOGVIEW_H
#define LOGVIEW_H

#include "fmt/format.h"
#include "spdlog/async.h"
#include "spdlog/sinks/qt_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <QMetaProperty>
#include <QObject>
#include <QQuickItem>
#include <QString>
#include <string>

constexpr auto THREAD_NUMS = 2;
constexpr auto QUEUE_SIZE = 8192;
constexpr auto MAX_FILE_SIZE = 1024 * 1024 * 4;
constexpr auto MAX_LOG_FILES = 2;

class LogView : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QQuickItem* textEditor READ textEditor WRITE setTextEditor NOTIFY
               textEditorChanged)
public:
  explicit LogView(LogView* parent = nullptr);

  ~LogView();

  void init();

  void reloadSinks();

  void clean();

  std::shared_ptr<spdlog::async_logger> raw();

public:
  QQuickItem* textEditor() const;

public slots:
  void setTextEditor(QQuickItem* newTextEditor);

signals:
  void textEditorChanged(QQuickItem*);

protected:
  std::string m_path = "./logs/across.log";
  std::shared_ptr<spdlog::async_logger> p_logger;
  std::shared_ptr<spdlog::details::thread_pool> p_thread_pool;
  std::vector<spdlog::sink_ptr> sinks;
  QQuickItem* p_text_editor = nullptr;
};

#endif // LOGVIEW_H
