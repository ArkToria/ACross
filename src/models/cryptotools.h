#ifndef CRYPTOTOOLS_H
#define CRYPTOTOOLS_H

#include <QByteArray>
#include <QCryptographicHash>
#include <QString>

#include <fcntl.h>
#include <linux/random.h>
#include <memory>
#include <optional>
#include <sodium.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace across {
namespace utils {
class CryptoTools
{
public:
  static int initAndCheck();

  static std::optional<QByteArray> aes256gcmEncode(const QByteArray& data,
                                                   const QByteArray& password);

  static std::optional<QByteArray> aes256gcmDecode(const QByteArray& data,
                                                   const QByteArray& password);

  static std::optional<QByteArray> sha256sums(const QByteArray& data);

  static bool detectEntropyQuality();
};
}
}

#endif // CRYPTOTOOLS_H
