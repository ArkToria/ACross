#ifndef NOTIFYTOOLS_H
#define NOTIFYTOOLS_H
#include <QObject>

#ifdef Q_OS_LINUX
#include "dbustools.h"
#endif

#ifdef Q_OS_MSDOS
#endif

namespace across {
namespace utils {
class NotifyTools : public QObject
{
  Q_OBJECT
public:
  explicit NotifyTools(QObject* parent = nullptr);

  void send(const QString& msg, const QString& summary = "");
signals:
};
}
}

#endif // NOTIFYTOOLS_H
