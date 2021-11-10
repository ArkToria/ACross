#include "notifytools.h"

using namespace across::utils;

#ifndef __MINGW32__

void NotifyTools::send(const QString &msg, const QString &summary) {
#ifdef Q_OS_LINUX
    DBusTools dbus_tools;

    dbus_tools.sendNotify(msg, summary);
#endif
}

#else

void NotifyTools::send(const QString &msg, const QString &summary,  QSharedPointer<QSystemTrayIcon> tray) {
#ifdef Q_OS_WIN
    if(tray.isNull()) return;
    bool isVisible = tray->isVisible();
    if(!isVisible) tray->show();
    tray->showMessage(summary, msg, QIcon(":misc/design/logo.svg"), 3500);
    if(!isVisible) tray->hide();
#endif
}

#endif