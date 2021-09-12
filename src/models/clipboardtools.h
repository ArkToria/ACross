#ifndef CLIPBOARDTOOLS_H
#define CLIPBOARDTOOLS_H

#include <QApplication>
#include <QClipboard>
#include <QImage>

namespace across {
namespace utils {

class ClipboardTools
{
public:
  ClipboardTools();

  void send(const QString& msg);

  void send(const QImage& img);
};
}
}

#endif // CLIPBOARDTOOLS_H
