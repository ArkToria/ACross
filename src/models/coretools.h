#ifndef CORETOOLS_H
#define CORETOOLS_H

#include "../view_models/configtools.h"
#include "../view_models/logtools.h"

#include <QByteArray>
#include <QObject>
#include <QProcess>
#include <QProcessEnvironment>
#include <QRegularExpression>
#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <memory>

namespace across::core {
class CoreTools : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning WRITE setIsRunning NOTIFY
                   isRunningChanged)
  public:
    explicit CoreTools(QObject *parent = nullptr);

    ~CoreTools() override;

    bool init(QSharedPointer<across::setting::ConfigTools> config);

    void setConfig(const QString &stdin_str);

    Q_INVOKABLE int run();

    Q_INVOKABLE int stop();

    Q_INVOKABLE int restart();

    [[nodiscard]] bool isRunning() const;

  public slots:
    void setIsRunning(bool value);

    void onReadData();

  signals:
    void isRunningChanged();

  private:
    across::config::Core *p_core{};
    QSharedPointer<QProcess> p_process;
    QSharedPointer<across::setting::ConfigTools> p_config;
    std::shared_ptr<spdlog::logger> p_logger;

    QString m_config;
    bool m_running = false;
};
} // namespace across

#endif // CORETOOLS_H
