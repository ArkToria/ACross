#include "notifytools.h"

using namespace across::utils;

NotifyTools::NotifyTools(QObject *parent) : QObject(parent)
{
}

void NotifyTools::send(const QString& msg, const QString& summary)
{
#ifdef Q_OS_LINUX
    DBusTools dbus_tools;

    dbus_tools.sendNotify(msg, summary);
#endif
}
