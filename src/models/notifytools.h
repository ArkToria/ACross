#ifndef NOTIFYTOOLS_H
#define NOTIFYTOOLS_H
#include <QObject>

#ifdef Q_OS_LINUX
#include "dbustools.h"
#endif

#ifdef Q_OS_MSDOS
#endif

#ifdef Q_OS_WIN
#include <wintoastlib.h>
#endif

namespace across {
namespace utils {
#ifdef Q_OS_WIN
struct ToastHandler : public WinToastLib::IWinToastHandler
{
  virtual void toastActivated() const {}
  virtual void toastActivated([[maybe_unused]] int actionIndex) const {}
  virtual void toastDismissed([[maybe_unused]] WinToastDismissalReason state) const {}
  virtual void toastFailed() const {}
};
#endif

class NotifyTools
{
public:
  static void send(const QString& msg, const QString& summary = "");
};
}
}

#endif // NOTIFYTOOLS_H
