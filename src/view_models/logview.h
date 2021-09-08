#ifndef LOGVIEW_H
#define LOGVIEW_H

#include <QObject>
#include <QStringList>

namespace across {
class LogView : public QObject
{
  Q_OBJECT
  Q_PROPERTY(
    QString coreLog READ coreLog WRITE setCoreLog NOTIFY coreLogChanged)

public:
  explicit LogView(QObject* parent = nullptr);

  void clean();

  void push(const QString& msg);

  const QString& coreLog() const;

public slots:
  void setCoreLog(const QString& newCoreLog);

signals:
  void coreLogChanged();

private:
  QString m_coreLog;
};
}

#endif // LOGVIEW_H
