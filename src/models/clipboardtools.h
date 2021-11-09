#ifndef CLIPBOARDTOOLS_H
#define CLIPBOARDTOOLS_H

#include <QApplication>
#include <QClipboard>
#include <QImage>

namespace across {
namespace utils {

class ClipboardTools {
  public:
    static void send(const QString &msg);

    static void send(const QImage &img);
};
} // namespace utils
} // namespace across

#endif // CLIPBOARDTOOLS_H
