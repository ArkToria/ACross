#ifndef CRYPTOTOOLS_H
#define CRYPTOTOOLS_H

#include <QByteArray>
#include <QString>
#include <fcntl.h>
#include <linux/random.h>
#include <memory>
#include <sodium.h>
#include <optional>
#include <sys/ioctl.h>
#include <unistd.h>

#include "logtools.h"

namespace across {
namespace utils {
class CryptoTools {
public:
    static int initAndCheck();

    static std::optional<std::string> base64Encode(const std::string& data, int variant = sodium_base64_VARIANT_ORIGINAL);

    static std::optional<std::string> base64Decode(const std::string& data, int variant = sodium_base64_VARIANT_ORIGINAL);

    static std::optional<QByteArray> aes256gcmEncode(const QByteArray& data, const QByteArray& password);

    static std::optional<QByteArray> aes256gcmDecode(const QByteArray& data, const QByteArray& password);

    static bool detectEntropyQuality();
};
}
}

#endif // CRYPTOTOOLS_H
