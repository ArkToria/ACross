#include "networktools.h"

using namespace across::config;
using namespace across::network;

DNSTools::DNSTools(QObject* parent)
  : QObject(parent)
{}

DNSTools::DNSTools(const QString& domain,
                   QDnsLookup::Type type,
                   const QHostAddress& dns_server)
{
  query(domain, type, dns_server);
}

void
DNSTools::query(const QString& domain,
                QDnsLookup::Type type,
                const QHostAddress& dns_server)
{
  p_dns = std::make_shared<QDnsLookup>(new QDnsLookup(this));
  connect(p_dns.get(), SIGNAL(finished()), this, SLOT(callback()));

  p_dns->setType(type);
  p_dns->setName(domain);
  p_dns->setNameserver(dns_server);
  p_dns->lookup();
}

QList<QString>
DNSTools::callback()
{
  if (p_dns == nullptr) {
    return QList<QString>();
  }

  if (p_dns->error() != QDnsLookup::NoError) {
    return QList<QString>();
  }

  QList<QString> list;

  switch (p_dns->type()) {
    case QDnsLookup::A:
    case QDnsLookup::AAAA:
      for (auto&& record : p_dns->hostAddressRecords()) {
        list.append(record.value().toString());
      }
      break;
    case QDnsLookup::CNAME:
      for (auto&& record : p_dns->canonicalNameRecords()) {
        list.append(record.value());
      }
      break;
    case QDnsLookup::NS:
      for (auto&& record : p_dns->nameServerRecords()) {
        list.append(record.value());
      }
      break;
    case QDnsLookup::MX:
      for (auto&& record : p_dns->mailExchangeRecords()) {
        list.append(record.exchange());
      }
      break;
    case QDnsLookup::TXT:
      for (auto&& record : p_dns->textRecords()) {
        for (auto&& value : record.values()) {
          list.append(value);
        }
      }
      break;
    default:
      break;
  }

  return list;
}

TCPPing::TCPPing(QObject* parent) {}

TCPPing::TCPPing(const QString& addr, unsigned int port)
  : m_addr(addr)
  , m_port(port)
{}

TCPPing::TCPPing(const QHostAddress& addr, unsigned int port)
  : m_addr(addr.toString())
  , m_port(port)
{}

void
TCPPing::setAddr(const QString& addr)
{
  m_addr = addr;
}

void
TCPPing::setAddr(const QHostAddress& addr)
{
  m_addr = addr.toString();
}

void
TCPPing::setPort(unsigned int port)
{
  m_port = port;
}

void
TCPPing::setTimes(int times)
{
  m_times = times;
}

int
TCPPing::getAvgLatency()
{
  int latency = -1;
  int result = -1;
  std::vector<int> sum;

  for (int i = 0; i < m_times; ++i) {
    latency = getLatency(m_addr, m_port);

    if (latency >= 0) {
      sum.emplace_back(latency);
    }
  }

  if (sum.size() > 0) {
    result = std::accumulate(sum.begin(), sum.end(), 0.0) / sum.size();
  }

  return result;
}

int
TCPPing::getLatency(const QString& addr, unsigned int port)
{
  QTcpSocket socket;
  QTime time = QTime::currentTime();

  socket.connectToHost(addr, port);
  socket.waitForConnected(PING_TIMEOUT);

  int latency = time.msecsTo(QTime::currentTime());

  if (latency >= PING_TIMEOUT) {
    latency = -1;
  }

  return latency;
}

CURLTools::CURLTools(QObject* parent)
  : QObject(parent)
{
  curl_global_init(CURL_GLOBAL_DEFAULT);
}

CURLTools::~CURLTools()
{
  curl_global_cleanup();
}

CURLcode
CURLTools::download(DownloadTask& task, std::stringstream* p_buf)
{
  auto err = CURLE_OK;

  curl_easy_setopt(task.handle.get(), CURLOPT_URL, task.url.c_str());
  curl_easy_setopt(
    task.handle.get(), CURLOPT_USERAGENT, task.user_agent.c_str());

  if (p_buf == nullptr) {
    static std::ofstream file(task.filename, std::ios::binary);
    curl_easy_setopt(
      task.handle.get(), CURLOPT_WRITEFUNCTION, saveToFileCallback);
    curl_easy_setopt(
      task.handle.get(), CURLOPT_WRITEDATA, reinterpret_cast<void*>(&file));
  } else {
    curl_easy_setopt(
      task.handle.get(), CURLOPT_WRITEFUNCTION, writeToDataCallback);
    curl_easy_setopt(task.handle.get(), CURLOPT_WRITEDATA, p_buf);
  }

  err = curl_easy_perform(task.handle.get());
  if (err == CURLE_OK) {
    emit downloadFinished();
  }

  return err;
}

size_t
CURLTools::saveToFileCallback(void* contents,
                              size_t size,
                              size_t nmemb,
                              void* p_ofstream)
{
  size_t real_size = size * nmemb;
  auto file = reinterpret_cast<std::ofstream*>(p_ofstream);
  file->write(reinterpret_cast<const char*>(contents), real_size);

  return real_size;
}

size_t
CURLTools::writeToDataCallback(void* contents,
                               size_t size,
                               size_t nmemb,
                               void* p_data)
{
  size_t real_size = size * nmemb;

  auto stream = reinterpret_cast<std::stringstream*>(p_data);

  stream->write(reinterpret_cast<const char*>(contents), real_size);

  return real_size;
}

CURLTools::DownloadTask::DownloadTask(const std::string& name,
                                      const std::string& url,
                                      int64_t group_id)
{
  if (url.empty()) {
    return;
  } else {
    this->url = url;
  }

  if (!name.empty()) {
    this->name = name;
  }

  auto split_lambda = [](std::string_view strv,
                         std::string delims) -> std::vector<std::string_view> {
    std::vector<std::string_view> output;
    size_t first = 0;

    while (first < strv.size()) {
      const auto second = strv.find_first_of(delims, first);

      if (first != second) {
        if (second == std::string_view::npos) {
          output.emplace_back(strv.substr(first, strv.size() - first));
          break;
        } else {
          output.emplace_back(strv.substr(first, second - first));
        }
      }

      first = second + delims.size();
    }

    return output;
  };

  this->filename = split_lambda(url, "/").back();

  this->handle = createEasyHandle();

  this->group_id = group_id;
}

CURLTools::EasyHandle
CURLTools::createEasyHandle()
{
  auto easy_handle = EasyHandle(curl_easy_init(), curl_easy_cleanup);

  return easy_handle;
}
