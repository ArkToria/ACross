#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>

namespace across {
class ImageProvider : public QQuickImageProvider
{
public:
  ImageProvider();

  QPixmap requestPixmap(const QString& id,
                        QSize* size,
                        const QSize& requestedSize) override;
};
}

#endif // IMAGEPROVIDER_H
