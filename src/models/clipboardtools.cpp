#include "clipboardtools.h"

using namespace across::utils;

ClipboardTools::ClipboardTools(QObject* parent)
{
}

void ClipboardTools::add(const QString& msg)
{
    auto* p_clipboard = QGuiApplication::clipboard();

    p_clipboard->setText(msg);
}

void ClipboardTools::add(const QImage& img)
{
    auto* p_clipboard = QGuiApplication::clipboard();

    p_clipboard->setImage(img);
}
