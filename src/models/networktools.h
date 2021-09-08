#ifndef NETWORKTOOLS_H
#define NETWORKTOOLS_H

#include "curl/curl.h"
#include "jsontools.h"
#include "json/json.h"
#include "json/reader.h"
#include "json/value.h"

#include <QDnsLookup>
#include <QHostAddress>
#include <QList>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QTime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ranges>
#include <string_view>

#include "../models/dbtools.h"

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

class CURLTools : public QObject
{
  Q_OBJECT
public:
  CURLTools(QObject* parent = nullptr);

  ~CURLTools();

  using EasyHandle = std::unique_ptr<CURL, std::function<void(CURL*)>>;

  struct DownloadTask
  {
    EasyHandle handle;
    std::string name;
    std::string filename;
    int64_t group_id;
    std::string url;
    std::string user_agent =
      "Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) "
      "Chrome/99.0.7113.93 Safari/537.36";

    DownloadTask(const std::string& name,
                 const std::string& url,
                 int64_t group_id);
  };

  CURLcode download(DownloadTask& task, std::stringstream* p_buf = nullptr);

signals:
  void downloadFinished();

private:
  static EasyHandle createEasyHandle();

  static size_t saveToFileCallback(void* contents,
                                   size_t size,
                                   size_t nmemb,
                                   void* p_ofstream);

  static size_t writeToDataCallback(void* contents,
                                    size_t size,
                                    size_t nmemb,
                                    void* p_data);
};
}
}

#endif // NETWORKTOOLS_H
