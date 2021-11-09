#include "clipboardtools.h"

using namespace across::utils;

void ClipboardTools::send(const QString &msg) {
    QApplication::clipboard()->setText(msg);
}

void ClipboardTools::send(const QImage &img) {
    QApplication::clipboard()->setImage(img);
}
