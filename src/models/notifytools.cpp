#include "notifytools.h"

#ifdef Q_OS_WIN
using namespace WinToastLib;
#endif

using namespace across::utils;

void
NotifyTools::send(const QString& msg, const QString& summary)
{
#ifdef Q_OS_LINUX
  DBusTools dbus_tools;

  dbus_tools.sendNotify(msg, summary);
#endif
#ifdef Q_OS_WIN
  if (!WinToast::isCompatible())
    return;
  auto& instance = *WinToast::instance();
  instance.setAppName(L"ACross");
  instance.setAppUserModelId(L"ACross");
  if (!instance.initialize())
    return;
  WinToastTemplate tpl;
  tpl.setTextField(summary.toStdWString(), WinToastTemplate::FirstLine);
  tpl.setTextField(msg.toStdWString(), WinToastTemplate::SecondLine);
  [[maybe_unused]] auto ret = instance.showToast(tpl, new ToastHandler());
#endif
}
