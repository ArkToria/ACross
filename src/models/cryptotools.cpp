#include "cryptotools.h"

using namespace across::utils;

int CryptoTools::initAndCheck()
{
    int result = 0;

    do {
        result = sodium_init();
        if (result < 0) {
            break;
        }

        result = crypto_aead_aes256gcm_is_available();
        if (result < 0) {
            break;
        }

        result = detectEntropyQuality();
    } while (false);

    return result;
}

std::optional<std::string> CryptoTools::base64Encode(const std::string& data, int variant)
{
    quint64 bf64_size = sodium_base64_encoded_len(data.length(), variant);

    std::unique_ptr<char> p_buffer(new char[bf64_size]);

    std::string result = sodium_bin2base64(p_buffer.get(), bf64_size,
                                           reinterpret_cast<const unsigned char*>(data.c_str()),
                                           data.length(), variant);

    return result;
}

std::optional<std::string> CryptoTools::base64Decode(const std::string& data, int variant)
{
    size_t bin_max_len = sodium_base64_encoded_len(data.length(), variant);

    std::unique_ptr<unsigned char> p_buffer(new unsigned char[bin_max_len]);

    size_t bin_len;

    int rc = sodium_base642bin(p_buffer.get(), bin_max_len,
                               data.data(), data.length(),
                               "\n\r ", &bin_len, nullptr, variant);

    if (rc != 0) {
        return {};
    }

    std::string result =  reinterpret_cast<const char*>(p_buffer.get());

    return result;
}

bool CryptoTools::detectEntropyQuality()
{
    int fd, c;
    bool result = false;

    do {
        if ((fd = open("/dev/random", O_RDONLY)) == -1) {
            break;
        }

        if (ioctl(fd, RNDGETENTCNT, &c) == 0 && c < 160) {
            break;
        }

        result = true;

    } while (false);

    (void)close(fd);

    return result;
}

std::optional<QByteArray> CryptoTools::aes256gcmEncode(const QByteArray& data, const QByteArray& password)
{
    QByteArray ciphertext(data.size() + crypto_aead_aes256gcm_ABYTES, 0);
    quint64 ciphertext_len;
    QByteArray nonce(crypto_aead_aes256gcm_NPUBBYTES, 0);
    randombytes_buf(nonce.data(), nonce.size());

    int err = crypto_aead_aes256gcm_encrypt(reinterpret_cast<unsigned char*>(ciphertext.data()), &ciphertext_len,
                                            reinterpret_cast<const unsigned char*>(data.data()), data.size(),
                                            nullptr, 0, nullptr,
                                            reinterpret_cast<const unsigned char*>(nonce.data()),
                                            reinterpret_cast<const unsigned char*>(password.data()));

    if (err != 0) {
        return {};
    }

    return nonce + ciphertext.left(ciphertext_len);
}

std::optional<QByteArray> CryptoTools::aes256gcmDecode(const QByteArray& data, const QByteArray& password)
{
    if (static_cast<quint64>(data.size()) < crypto_aead_aes256gcm_NPUBBYTES) {
        return {};
    }

    QByteArray plaintext(data.size() - crypto_aead_aes256gcm_ABYTES, 0);
    quint64 plaintext_len;
    QByteArray nonce = data.left(crypto_aead_aes256gcm_NPUBBYTES);
    QByteArray ciphertext = data.mid(crypto_aead_aes256gcm_NPUBBYTES);

    int err = crypto_aead_aes256gcm_decrypt(reinterpret_cast<unsigned char*>(plaintext.data()), &plaintext_len,
                                            nullptr,
                                            reinterpret_cast<const unsigned char*>(ciphertext.data()),
                                            ciphertext.size(),
                                            nullptr, 0,
                                            reinterpret_cast<const unsigned char*>(nonce.data()),
                                            reinterpret_cast<const unsigned char*>(password.data()));
    if (err != 0) {
        return {};
    }

    return plaintext.left(plaintext_len);
}
