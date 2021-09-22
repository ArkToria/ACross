#include "imageprovider.h"

using namespace across;

ImageProvider::ImageProvider()
  : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap
ImageProvider::requestPixmap(const QString& id,
                             QSize* size,
                             const QSize& requestedSize)
{
  int width = 100;
  int height = 100;

  if (size)
    *size = QSize(width, height);
  QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                 requestedSize.height() > 0 ? requestedSize.height() : height);
  pixmap.fill(QColor(id).rgba());

  return pixmap;
}
