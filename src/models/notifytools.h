#ifndef NOTIFYTOOLS_H
#define NOTIFYTOOLS_H
#include <QObject>

#ifdef Q_OS_LINUX
#include "dbustools.h"
#endif

#ifdef Q_OS_MSDOS
#endif

#ifdef __MINGW32__
#include <QSharedPointer>
#include <QSystemTrayIcon>
#endif

namespace across {
namespace utils {
class NotifyTools {
  public:
  #ifndef __MINGW32__
    static void send(const QString &msg, const QString &summary = "");
  #else
    static void send(const QString &msg, const QString &summary = "", QSharedPointer<QSystemTrayIcon> tray = nullptr);
  #endif
};
} // namespace utils
} // namespace across

#endif // NOTIFYTOOLS_H
