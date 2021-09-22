#include "qrcodetools.h"

using namespace across::utils;

QRCodeTools::QRCodeTools() {}

QImage
QRCodeTools::write(const QString& text)
{
  auto writer =
    ZXing::MultiFormatWriter(m_format).setMargin(m_margin).setEccLevel(
      m_ecc_level);

  // text should be encoded
  auto bitmap = ZXing::ToMatrix<uint8_t>(
    writer.encode(text.toStdWString(), m_size, m_size));

  QImage img(bitmap.width(), bitmap.height(), QImage::Format_RGB888);

  const auto black = qRgb(0, 0, 0);
  const auto white = qRgb(255, 255, 255);

  img.fill(black);

  for (int i = 0; i < bitmap.width(); ++i) {
    for (int j = 0; j < bitmap.height(); ++j) {
      if (bitmap.get(i, j)) {
        img.setPixel(i, j, white);
      }
    }
  }

#ifdef QT_DEBUG
  img.save("test.png");
#endif

  return img;
}

void
QRCodeTools::setSize(uint size)
{
  if (size > 0 && m_size != size) {
    m_size = size;
  }
}

void
QRCodeTools::setFormat(ZXing::BarcodeFormat format)
{
  if (m_format == format) {
    return;
  }

  m_format = format;
}

void
QRCodeTools::setEccLevel(int level)
{
  if (m_ecc_level == level) {
    return;
  }

  m_ecc_level = level;
}
