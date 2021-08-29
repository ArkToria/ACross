#include "dbustools.h"

using namespace across::utils;

DBusTools::DBusTools()
{
}

void DBusTools::sendNotify(const QString& message, const QString& summary)
{
    NotifyDBus notifyDBus;

    notifyDBus.body = message;
    notifyDBus.summary = summary;

    sendNotify(notifyDBus);
}

void DBusTools::sendNotify(NotifyDBus& notifyDBus)
{
    do {
        if (!QDBusConnection::sessionBus().isConnected()) {
            break;
        }

        QDBusInterface dbus_interface(notifyDBus.server, notifyDBus.path,
            notifyDBus.interface, QDBusConnection::sessionBus());

        if (!dbus_interface.isValid()) {
            break;
        }

        QDBusReply<QString> reply = dbus_interface.call(
            notifyDBus.method,
            notifyDBus.app_name,
            notifyDBus.replace_id,
            notifyDBus.app_icon,
            notifyDBus.summary,
            notifyDBus.body,
            notifyDBus.actions,
            notifyDBus.hints,
            notifyDBus.timeout);

        if (!reply.isValid()) {
            break;
        }
    } while (false);
}
