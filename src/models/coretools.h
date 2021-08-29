#ifndef CORETOOLS_H
#define CORETOOLS_H

#include "../view_models/configtools.h"
#include <QByteArray>
#include <QObject>
#include <QProcess>
#include <QRegExp>
#include <QString>
#include <QStringList>

namespace across {
namespace core {
class CoreTools : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning WRITE setIsRunning NOTIFY isRunningChanged)
public:
    CoreTools(QObject* parent = nullptr);

    ~CoreTools();

    bool init(const across::setting::Core& core_info);

    void setConfig(const QString& stdin_str);

    void setConfig(const std::string& stdin_str);

    Q_INVOKABLE int run();

    Q_INVOKABLE int stop();

    bool isRunning();

public slots:
    void onReadData();

    void setIsRunning(bool value);

signals:
    void isRunningChanged();

private:
    across::setting::Core m_core;
    QProcess* p_process;
    QString m_config;
    bool m_running = false;
};
}
}

#endif // CORETOOLS_H
