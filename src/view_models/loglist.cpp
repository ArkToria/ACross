#include "loglist.h"

using namespace across;

LogList::LogList(QObject* parent)
  : QObject(parent)
{
  
}

LogList::~LogList() {}

void
LogList::init(setting::ConfigTools& config)
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
LogList::clean()
{
  emit preItemsReset();

  p_core_cache->clear();

  emit postItemsReset();
}

void
LogList::append(const QString& msg)
{
  while (p_core_cache->isFull()) {
    remove();
  }

  emit preItemAppended();

  p_core_cache->append(msg);

  emit postItemAppended();
}

void
LogList::remove()
{
  emit preFirstItemRemoved();

  p_core_cache->removeFirst();

  emit postFirstItemRemoved();
}

QPair<int, QString>
LogList::getLastItem()
{
  return { p_core_cache->lastIndex(), p_core_cache->last() };
}

QPair<int, QString>
LogList::getFirstItem()
{
  return { p_core_cache->firstIndex(), p_core_cache->first() };
}

QString
LogList::getItem(int index)
{
  if (index >= p_core_cache->size()) {
    return {};
  }

  return p_core_cache->at(index);
}

size_t
LogList::getSize()
{
  return p_core_cache->size();
}

void
LogList::styleFomatter(QString& msg)
{
  for (auto iter = colors_map.begin(); iter != colors_map.end(); ++iter) {
    auto replace_str = fmt::format("<span style='color: {}'>{}</span>",
                                   iter.value().toStdString(),
                                   iter.key().toStdString());

    msg.replace(iter.key(), QString().fromStdString(replace_str));
  }

  msg.replace("\n", "<br/>");
}

void
LogList::doDeleteLater(QContiguousCache<QString>* obj)
{
  obj->clear();
}
