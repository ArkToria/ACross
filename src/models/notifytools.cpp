#include "notifytools.h"

#ifdef _MSC_VER
using winrt::Windows::UI::Notifications::ToastNotificationManager;
using winrt::Windows::UI::Notifications::ToastNotification;
using winrt::Windows::UI::Notifications::ToastTemplateType;
#endif // _MSC_VER

using namespace across::utils;

#ifdef Q_OS_LINUX
void NotifyTools::send(const QString &msg, const QString &summary,
                       QSharedPointer<QSystemTrayIcon> tray) {
    DBusTools dbus_tools;

    dbus_tools.sendNotify(msg, summary);
}
#endif // Q_OS_LINUX

#ifdef __MINGW32__
void NotifyTools::send(const QString &msg, const QString &summary,
                       QSharedPointer<QSystemTrayIcon> tray) {
    if (tray.isNull())
        return;
    bool isVisible = tray->isVisible();
    if (!isVisible)
        tray->show();
    tray->showMessage(summary, msg, QIcon(":misc/design/logo.svg"), 3500);
    if (!isVisible)
        tray->hide();
}
#endif // __MINGW32__

#ifdef _MSC_VER
void NotifyTools::send(const QString &msg, const QString &summary,
                       QSharedPointer<QSystemTrayIcon> tray) {
    if (!aumid_initialized) {
        if (SUCCEEDED(SetCurrentProcessExplicitAppUserModelID(L"ACross"))) {
            aumid_initialized = true;
        } else {
            return;
        }
    }
    auto doc = ToastNotificationManager::GetTemplateContent(
        ToastTemplateType::ToastText02);
    auto node = doc.CreateTextNode(summary.toStdWString());
    doc.GetElementsByTagName(L"text").GetAt(0).AppendChild(std::move(node));
    node = doc.CreateTextNode(msg.toStdWString());
    doc.GetElementsByTagName(L"text").GetAt(1).AppendChild(std::move(node));
    ToastNotificationManager::CreateToastNotifier(L"ACross").Show(ToastNotification{doc});
}
#endif // _MSC_VER
