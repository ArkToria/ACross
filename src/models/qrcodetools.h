#ifndef QRCODETOOLS_H
#define QRCODETOOLS_H

#include <QImage>
#include <QString>
#include <QUrl>

#include <BarcodeFormat.h>
#include <BitMatrix.h>
#include <MultiFormatWriter.h>
#include <ZXVersion.h>

#define ZXING_VERSION                                                          \
  QT_VERSION_CHECK(                                                            \
    ZXING_VERSION_MAJOR, ZXING_VERSION_MINOR, ZXING_VERSION_PATCH)

namespace across {
namespace utils {
class QRCodeTools
{
public:
  QRCodeTools();

  QImage write(const QString& text);

  void setSize(uint size);
  void setMargin(int margin);
  void setEccLevel(int level);
  void setFormat(ZXing::BarcodeFormat format);

private:
  uint m_size = 256;
  uint m_margin = 2;
  int m_ecc_level = -1;

#if (ZXING_VERSION > QT_VERSION_CHECK(1, 2, 0))
  ZXing::BarcodeFormat m_format = ZXing::BarcodeFormat::QR_CODE;
#else
  ZXing::BarcodeFormat m_format = ZXing::BarcodeFormat::QRCode;
#endif
};
}
}

#endif // QRCODETOOLS_H
