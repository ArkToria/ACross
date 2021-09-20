#ifndef NETWORKTOOLS_H
#define NETWORKTOOLS_H

#include "curl/curl.h"

#include <QDnsLookup>
#include <QHostAddress>
#include <QList>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QThread>
#include <QTime>

#include <iostream>
#include <sstream>
#include <string>

namespace across {
namespace network {
class DNSTools : QObject
{
  Q_OBJECT;

public:
  DNSTools(QObject* parent = nullptr);

  DNSTools(const QString& domain,
           QDnsLookup::Type type,
           const QHostAddress& dns_server = QHostAddress::LocalHost);

  void query(const QString& domain,
             QDnsLookup::Type type,
             const QHostAddress& dns_server = QHostAddress::LocalHost);

public slots:
  QList<QString> callback();

private:
  std::shared_ptr<QDnsLookup> p_dns = nullptr;
};

class TCPPing : QObject
{
  Q_OBJECT
public:
  TCPPing(QObject* parent = nullptr);

  TCPPing(const QString& addr, unsigned int port);
  TCPPing(const QHostAddress& addr, unsigned int port);

  void setAddr(const QString& addr);
  void setAddr(const QHostAddress& addr);
  void setPort(unsigned int port);
  void setTimes(int times);
  int getAvgLatency();

  static int getLatency(const QString& addr, unsigned int port);

private:
  QString m_addr = "127.0.0.1";
  unsigned int m_port = 80;
  unsigned int m_times = 3;
  static const int PING_TIMEOUT = 3000;
};

struct DownloadTask
{
  QString filename;
  QString url;
  QString user_agent;
  QString proxy;
};

class CURLWorker : public QObject
{
  Q_OBJECT
  Q_PROPERTY(
    double progress READ progress WRITE setProgress NOTIFY progressChanged)
  Q_PROPERTY(
    bool isRunning READ isRunning WRITE setIsRunning NOTIFY isRunningChanged)
public:
  double progress() const;

  bool isRunning() const;

public slots:
  void run(const QString& url,
           const QString& user_agent = "",
           const QString& proxy = "");

  void setProgress(double newProgress);

  void setIsRunning(bool newIsRunning);

signals:
  void done(const QString& content);

  void progressChanged(double progress);

  void isRunningChanged(bool isRunning);

private:
  static size_t dataCallback(void* contents,
                             size_t size,
                             size_t nmemb,
                             void* p_data);
  static int xferInfo(void* p,
                      curl_off_t dltotal,
                      curl_off_t dlnow,
                      curl_off_t ultotal,
                      curl_off_t ulnow);

  double m_progress = 0.0;
  bool m_isRunning = false;
};

class CURLTools : public QObject
{
  Q_OBJECT
public:
  CURLTools(QObject* parent = nullptr);

  ~CURLTools();

  CURLcode download(DownloadTask& task);

public slots:
  void handleResult(const QString& content);

signals:
  void operate(const QString& url,
               const QString& user_agent,
               const QString& proxy);

  void downloadFinished(const QString& content);

private:
  QThread* p_thread = nullptr;
};
}
}

#endif // NETWORKTOOLS_H
