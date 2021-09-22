#include "imageprovider.h"

using namespace across;
using namespace across::utils;

ImageProvider::ImageProvider()
  : QQuickImageProvider(QQuickImageProvider::Image)
{}

void
ImageProvider::setContent(const QString& id, const QString& content)
{
  m_id = id;
  m_content = content;
}

QImage
ImageProvider::requestImage(const QString& id,
                            QSize* size,
                            const QSize& requestedSize)
{
  if (id == m_id) {
    return QRCodeTools().write(m_content);
  }

  return QImage();
}
