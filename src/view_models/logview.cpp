#include "logview.h"

using namespace across;

LogView::LogView(QObject* parent)
  : QObject(parent)
{}

LogView::~LogView() {}

void
LogView::init(setting::ConfigTools& config)
{
  colors_map = { { "Info", config.styleColor() },
                 { "Warning", config.warnColor() },
                 { "Error", config.warnColor() },
                 { "accepted", config.highlightColor() } };

  p_core_cache = QSharedPointer<QContiguousCache<QString>>(
    new QContiguousCache<QString>, doDeleteLater);

  p_core_cache->setCapacity(config.logLines() - 1);

  connect(&config,
          &across::setting::ConfigTools::logLinesChanged,
          this,
          [&](int max_lines) { p_core_cache->setCapacity(max_lines - 1); });
}

void
LogView::clean()
{
  m_core_logs.clear();
  p_core_cache->clear();
}

void
LogView::append(const QString& msg)
{
  auto temp = msg;

  styleFomatter(temp);

  p_core_cache->append(temp);

  emit coreLogChanged();
}

QString&
LogView::coreLog()
{
  if (!m_core_logs.isEmpty()) {
    m_core_logs.clear();
  }

  // very slow!
  if (!p_core_cache->isEmpty()) {
    for (auto iter = p_core_cache->firstIndex();
         iter != p_core_cache->lastIndex();
         ++iter) {
      m_core_logs.append(p_core_cache->at(iter));
    }
  }

  return m_core_logs;
}

void
LogView::styleFomatter(QString& msg)
{
  for (auto iter = colors_map.begin(); iter != colors_map.end(); ++iter) {
    auto replace_str = fmt::format("<span style='color: {}'>{}</span>",
                                   iter.value().toStdString(),
                                   iter.key().toStdString());

    msg.replace(iter.key(), QString::fromStdString(replace_str));
  }

  msg.replace("\n", "<br/>");
}

void
LogView::doDeleteLater(QContiguousCache<QString>* obj)
{
  obj->clear();
}
