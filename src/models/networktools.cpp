#include "networktools.h"

using namespace across::network;
using namespace across::config;

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

size_t
CURLWorker::dataCallback(void* contents,
                         size_t size,
                         size_t nmemb,
                         void* p_data)
{
  size_t real_size = size * nmemb;
  auto stream = reinterpret_cast<std::stringstream*>(p_data);
  stream->write(reinterpret_cast<const char*>(contents), real_size);
  return real_size;
}

int
CURLWorker::xferInfo(void* p,
                     curl_off_t dltotal,
                     curl_off_t dlnow,
                     curl_off_t ultotal,
                     curl_off_t ulnow)
{
  auto worker = static_cast<CURLWorker*>(p);

  if (!qFuzzyCompare(dltotal, 0.0)) {
    double result = (double)dlnow / dltotal;
    worker->setProgress(result);
  }

  return 0;
}

void
CURLWorker::run(const QVariant& data)
{
  auto task = data.value<DownloadTask>();
  if (!isRunning()) {
    setIsRunning(true);
  } else {
    return;
  }

  // create buffer
  std::stringstream buffer;

  // create handle
  CURL* handle = curl_easy_init();

  // download setting
  curl_easy_setopt(handle, CURLOPT_URL, task.url.toStdString().c_str());
  if (!task.user_agent.isEmpty()) {
    curl_easy_setopt(
      handle, CURLOPT_USERAGENT, task.user_agent.toStdString().c_str());
  }
  if (!task.proxy.isEmpty()) {
    curl_easy_setopt(handle, CURLOPT_PROXY, task.proxy.toStdString().c_str());
  }

  // progress callback
  curl_easy_setopt(handle, CURLOPT_XFERINFOFUNCTION, xferInfo);
  curl_easy_setopt(handle, CURLOPT_XFERINFODATA, this);
  curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0L);

  // data callback
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, dataCallback);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &buffer);

  // execute
  if (auto err = curl_easy_perform(handle); err == CURLE_OK) {
    task.content = QString::fromStdString(buffer.str());
  }

  // notify finished
  emit done(QVariant::fromValue<DownloadTask>(task));

  // clean
  curl_easy_cleanup(handle);
  setIsRunning(false);
}

double
CURLWorker::progress() const
{
  return m_progress;
}

void
CURLWorker::setProgress(double newProgress)
{
  if (qFuzzyCompare(m_progress, newProgress))
    return;
  m_progress = newProgress;
  emit progressChanged(m_progress);
}

bool
CURLWorker::isRunning() const
{
  return m_isRunning;
}

void
CURLWorker::setIsRunning(bool newIsRunning)
{
  if (m_isRunning == newIsRunning)
    return;

  m_isRunning = newIsRunning;

  emit isRunningChanged(m_isRunning);
}

CURLTools::CURLTools(QObject* parent)
  : QObject(parent)
{
  curl_global_init(CURL_GLOBAL_DEFAULT);
}

CURLTools::~CURLTools()
{
  // destroy pointer
  for (auto& p_thread : m_threads) {
    if (p_thread != nullptr) {
      p_thread->quit();
      p_thread->wait();
    }
  }

  curl_global_cleanup();
}

CURLcode
CURLTools::download(DownloadTask& task)
{
  if (m_threads.contains(task.filename)) {
    if (auto& p_thread = m_threads[task.filename]; p_thread != nullptr) {
      p_thread->quit();
      p_thread->wait();
    } else {
      p_thread = new QThread(this);
    }
  } else {
    m_threads.insert(task.filename, new QThread(this));
  }

  auto worker = new CURLWorker();
  worker->moveToThread(m_threads.value(task.filename));

  // connect signals
  connect(this, &CURLTools::operate, worker, &CURLWorker::run);
  connect(m_threads.value(task.filename),
          &QThread::finished,
          worker,
          &QObject::deleteLater);
  connect(worker, &CURLWorker::done, this, &CURLTools::handleResult);

  // start thread process
  m_threads.value(task.filename)->start();

  emit operate(QVariant::fromValue<DownloadTask>(task));

  // TODO: handle error
  return CURLE_OK;
}

void
CURLTools::handleResult(const QVariant& content)
{
  emit downloadFinished(content);
}

QString
UpdateTools::getVersion(const QString& content)
{
  Json::Value root;
  Json::String err_msg;

  if (JsonTools::jsonParse(content.toStdString(), root, err_msg)) {
    if (auto tag = root["tag_name"]; !tag.isNull() && tag.isString()) {
      auto result = QString::fromStdString(tag.asString());
      return result.remove("v");
    }
  }

  return QString();
}

int
UpdateTools::compareVersion(const QString& ver_a, const QString& ver_b)
{
  // Output values
  //   : <0 : if ver1<ver2 0
  //   : if ver1 == ver2> 0 : if ver1>
  //   ver2

  constexpr auto qstr2ver = [](const QString& qstr_ver) -> semver::version {
    auto std_str = qstr_ver.toStdString();
    std::string_view view_str(std_str.data(), std_str.size());

    return semver::version(view_str);
  };

  if (auto v_a = qstr2ver(ver_a), v_b = qstr2ver(ver_b); v_a < v_b)
    return -1;
  else if (v_a == v_b)
    return 0;
  else
    return 1;
}
