#include "notifytools.h"

#ifdef _MSC_VER
#include <ShObjIdl.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.Notifications.h>

using winrt::Windows::UI::Notifications::ToastNotificationManager;
using winrt::Windows::UI::Notifications::ToastTemplateType;
#endif // _MSC_VER

using namespace across::utils;

void NotifyTools::send(const QString &msg, const QString &summary,
                       QSharedPointer<QSystemTrayIcon> tray) {
#ifdef Q_OS_LINUX
    DBusTools dbus_tools;

    dbus_tools.sendNotify(msg, summary);
#endif // Q_OS_LINUX

#ifdef __MINGW32__
    if (tray.isNull())
        return;
    bool isVisible = tray->isVisible();
    if (!isVisible)
        tray->show();
    tray->showMessage(summary, msg, QIcon(":misc/design/logo.svg"), 3500);
    if (!isVisible)
        tray->hide();
#endif // __MINGW32__

#ifdef _MSC_VER
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
    ToastNotificationManager::CreateToastNotifier(L"ACross").Show({doc});
#endif // _MSC_VER
}