#ifndef CORETOOLS_H
#define CORETOOLS_H

#include "../view_models/configtools.h"
#include "../view_models/logview.h"

#include <QByteArray>
#include <QObject>
#include <QProcess>
#include <QProcessEnvironment>
#include <QRegExp>
#include <QString>
#include <QStringList>

namespace across {
namespace core {
class CoreTools : public QObject
{
  Q_OBJECT
  Q_PROPERTY(
    bool isRunning READ isRunning WRITE setIsRunning NOTIFY isRunningChanged)
public:
  CoreTools(QObject* parent = nullptr);

  ~CoreTools();

  bool init(across::setting::ConfigTools& config, across::LogView& log_view);

  void setConfig(const QString& stdin_str);

  void setConfig(const std::string& stdin_str);

  Q_INVOKABLE int run();

  Q_INVOKABLE int stop();

  bool isRunning();

public slots:
  void setIsRunning(bool value);

  void onReadData();

signals:
  void isRunningChanged();

private:
  across::setting::ConfigTools* p_config;
  across::LogView* p_log_view;

  across::setting::Core m_core;

  QString m_config;
  bool m_running = false;

  QProcess* p_process;
  QProcessEnvironment m_env;
};
}
}

#endif // CORETOOLS_H
