#ifndef LOGVIEW_H
#define LOGVIEW_H

#include "configtools.h"

#include "fmt/format.h"

#include <QMap>
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

  void init(across::setting::ConfigTools& config);

  void clean();

  void push(const QString& msg);

  const QString& coreLog() const;

  void styleFomatter(QString& msg);

public slots:
  void setCoreLog(const QString& newCoreLog);

signals:
  void coreLogChanged();

private:
  QString m_coreLog;

  QMap<QString, QString> colors_map;
};
}

#endif // LOGVIEW_H
