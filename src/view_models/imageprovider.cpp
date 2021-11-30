#include "imageprovider.h"

using namespace across;
using namespace across::utils;

ImageProvider::ImageProvider(ImageType type, Flags flags)
    : QQuickImageProvider(type, flags) {}

void ImageProvider::setContent(const QString &id, const QString &content) {
    m_id = QUrl(id).toString(QUrl::FullyEncoded);
    m_content = content;
}

QImage ImageProvider::requestImage(const QString &id, QSize *size,
                                   const QSize &requestedSize) {
    if (QUrl(id).toString(QUrl::FullyEncoded) == m_id) {
        return QRCodeTools().write(m_content);
    }

    return {};
}
