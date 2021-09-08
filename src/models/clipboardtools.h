#ifndef CLIPBOARDTOOLS_H
#define CLIPBOARDTOOLS_H

#include <QClipboard>
#include <QGuiApplication>
#include <QImage>
#include <memory>

namespace across {
namespace utils {

class ClipboardTools : public QObject
{
  Q_OBJECT
public:
  explicit ClipboardTools(QObject* parent = nullptr);

  void add(const QString& msg);

  void add(const QImage& img);
};
}
}

#endif // CLIPBOARDTOOLS_H
