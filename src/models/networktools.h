#ifndef NETWORKTOOLS_H
#define NETWORKTOOLS_H

#include "curl/curl.h"
#include "nlohmann/json.hpp"
#include "semver.hpp"

#include <QDnsLookup>
#include <QFuture>
#include <QHostAddress>
#include <QList>
#include <QObject>
#include <QQueue>
#include <QString>
#include <QTcpSocket>
#include <QThread>
#include <QTime>
#include <QtConcurrent>

#include <iostream>
#include <sstream>
#include <string>

#ifdef Q_OS_WIN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

namespace across {
namespace network {
using Json = nlohmann::json;

class DNSTools : QObject {
    Q_OBJECT;

  public:
    DNSTools(QObject *parent = nullptr);

    DNSTools(const QString &domain, QDnsLookup::Type type,
             const QHostAddress &dns_server = QHostAddress::LocalHost);

    void query(const QString &domain, QDnsLookup::Type type,
               const QHostAddress &dns_server = QHostAddress::LocalHost);

  public slots:
    QList<QString> callback();

  private:
    std::shared_ptr<QDnsLookup> p_dns = nullptr;
};

class TCPPing : QObject {
    Q_OBJECT
  public:
    TCPPing(QObject *parent = nullptr);

    TCPPing(const QString &addr, unsigned int port);
    TCPPing(const QHostAddress &addr, unsigned int port);

    void setAddr(const QString &addr);
    void setAddr(const QHostAddress &addr);
    void setPort(unsigned int port);
    void setTimes(int times);
    int getAvgLatency();

    static int getLatency(const QString &addr, unsigned int port);

  private:
    QString m_addr = "127.0.0.1";
    unsigned int m_port = 80;
    unsigned int m_times = 3;
    static const int PING_TIMEOUT = 3000;
};

struct DownloadTask {
    qint64 id;
    QString name;
    QString url;
    QString user_agent;
    QString proxy;
    QString content;
};

class CURLTools : public QObject {
    Q_OBJECT
  public:
    CURLTools(QObject *parent = nullptr);

    ~CURLTools();

    CURLcode download(const DownloadTask &task);

  public slots:
    void handleResult(const QVariant &content);

  signals:
    void operate(const QVariant &data);

    void downloadFinished(const QVariant &content);

  private:
    QQueue<QFuture<void>> m_tasks;

    static size_t dataCallback(void *contents, size_t size, size_t nmemb,
                               void *p_data);
};

class UpdateTools {
  public:
    static QString getVersion(const QString &content);
    static int compareVersion(const QString &ver_a, const QString &ver_b);
};
} // namespace network
} // namespace across

#endif // NETWORKTOOLS_H
