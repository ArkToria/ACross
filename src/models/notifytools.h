#ifndef NOTIFYTOOLS_H
#define NOTIFYTOOLS_H
#include <QObject>

#ifdef Q_OS_LINUX
#include "dbustools.h"
#endif

#include <QSharedPointer>
#include <QSystemTrayIcon>

namespace across {
namespace utils {
class NotifyTools {
  public:
    static void send(const QString &msg, const QString &summary = "",
                     QSharedPointer<QSystemTrayIcon> tray = nullptr);

  private:
    inline static bool aumid_initialized{false};
};
} // namespace utils
} // namespace across

#endif // NOTIFYTOOLS_H
