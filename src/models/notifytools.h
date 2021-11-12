#ifndef NOTIFYTOOLS_H
#define NOTIFYTOOLS_H
#include <QObject>

#ifdef Q_OS_LINUX
#include "dbustools.h"
#endif

#ifdef _MSC_VER
#include <ShObjIdl.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.Notifications.h>
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
#ifdef _MSC_VER
    inline static bool aumid_initialized{false};
#endif
};
} // namespace utils
} // namespace across

#endif // NOTIFYTOOLS_H
