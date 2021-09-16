#ifndef CORETOOLS_H
#define CORETOOLS_H

#include "../view_models/configtools.h"
#include "../view_models/logtools.h"

#include <QByteArray>
#include <QObject>
#include <QProcess>
#include <QProcessEnvironment>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <memory>

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

  bool init(QSharedPointer<LogView> log_view,
            QSharedPointer<across::setting::ConfigTools> config);

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
  across::setting::Core m_core;
  QSharedPointer<across::setting::ConfigTools> p_config;
  std::shared_ptr<across::utils::LogTools> p_logger;

  QString m_config;
  bool m_running = false;

  QProcess* p_process;
  QProcessEnvironment m_env;
};
}
}

#endif // CORETOOLS_H
