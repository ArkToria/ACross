#ifndef QRCODETOOLS_H
#define QRCODETOOLS_H

#include <QImage>
#include <QString>
#include <QUrl>

#ifdef USE_FETCH_ZXING_CPP
#include <BarcodeFormat.h>
#include <BitMatrix.h>
#include <MultiFormatWriter.h>
#else
#include "ZXing/BarcodeFormat.h"
#include "ZXing/BitMatrix.h"
#include "ZXing/MultiFormatWriter.h"
#endif

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
  ZXing::BarcodeFormat m_format = ZXing::BarcodeFormat::QRCode;
};
}
}

#endif // QRCODETOOLS_H
