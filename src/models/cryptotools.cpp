#include "cryptotools.h"

using namespace across::utils;

int
CryptoTools::initAndCheck()
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

bool
CryptoTools::detectEntropyQuality()
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

std::optional<QByteArray>
CryptoTools::aes256gcmEncode(const QByteArray& data, const QByteArray& password)
{
  QByteArray ciphertext(data.size() + crypto_aead_aes256gcm_ABYTES, 0);
  quint64 ciphertext_len;
  QByteArray nonce(crypto_aead_aes256gcm_NPUBBYTES, 0);
  randombytes_buf(nonce.data(), nonce.size());

  int err = crypto_aead_aes256gcm_encrypt(
    reinterpret_cast<unsigned char*>(ciphertext.data()),
    &ciphertext_len,
    reinterpret_cast<const unsigned char*>(data.data()),
    data.size(),
    nullptr,
    0,
    nullptr,
    reinterpret_cast<const unsigned char*>(nonce.data()),
    reinterpret_cast<const unsigned char*>(password.data()));

  if (err != 0) {
    return {};
  }

  return nonce + ciphertext.left(ciphertext_len);
}

std::optional<QByteArray>
CryptoTools::aes256gcmDecode(const QByteArray& data, const QByteArray& password)
{
  if (static_cast<quint64>(data.size()) < crypto_aead_aes256gcm_NPUBBYTES) {
    return {};
  }

  QByteArray plaintext(data.size() - crypto_aead_aes256gcm_ABYTES, 0);
  quint64 plaintext_len;
  QByteArray nonce = data.left(crypto_aead_aes256gcm_NPUBBYTES);
  QByteArray ciphertext = data.mid(crypto_aead_aes256gcm_NPUBBYTES);

  int err = crypto_aead_aes256gcm_decrypt(
    reinterpret_cast<unsigned char*>(plaintext.data()),
    &plaintext_len,
    nullptr,
    reinterpret_cast<const unsigned char*>(ciphertext.data()),
    ciphertext.size(),
    nullptr,
    0,
    reinterpret_cast<const unsigned char*>(nonce.data()),
    reinterpret_cast<const unsigned char*>(password.data()));
  if (err != 0) {
    return {};
  }

  return plaintext.left(plaintext_len);
}

std::optional<QByteArray>
CryptoTools::sha256sums(const QByteArray& data)
{
  return QCryptographicHash::hash(data, QCryptographicHash::Sha256);
}
