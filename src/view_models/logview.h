#ifndef LOGVIEW_H
#define LOGVIEW_H

#include "configtools.h"

#include "fmt/format.h"

#include <QContiguousCache>
#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <QString>

namespace across {
class LogView : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString coreLog READ coreLog NOTIFY coreLogChanged)

public:
  explicit LogView(QObject* parent = nullptr);

  ~LogView();

  void init(across::setting::ConfigTools& config);

  void clean();

  void push(const QString& msg);

  QString& coreLog();

  void styleFomatter(QString& msg);

signals:
  void coreLogChanged();

private:
  QString m_core_logs;
  QSharedPointer<QContiguousCache<QString>> p_core_cache;
  QMap<QString, QString> colors_map;

  static void doDeleteLater(QContiguousCache<QString>* obj);
};
}

#endif // LOGVIEW_H
