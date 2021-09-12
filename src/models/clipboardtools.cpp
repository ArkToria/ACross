#include "clipboardtools.h"

using namespace across::utils;

ClipboardTools::ClipboardTools() {}

void
ClipboardTools::send(const QString& msg)
{
  QApplication::clipboard()->setText(msg);
}

void
ClipboardTools::send(const QImage& img)
{
  QApplication::clipboard()->setImage(img);
}
