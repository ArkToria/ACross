#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include "../models/qrcodetools.h"

#include <QImage>
#include <QQuickImageProvider>

namespace across {
class ImageProvider : public QQuickImageProvider
{
public:
  ImageProvider(ImageType type = ImageType::Image, Flags flags = Flags());

  ~ImageProvider();

  void setContent(const QString& id, const QString& content);

  QImage requestImage(const QString& id,
                      QSize* size,
                      const QSize& requestedSize) override;

private:
  QString m_id;
  QString m_content;
};
}

#endif // IMAGEPROVIDER_H
