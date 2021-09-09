#include "logview.h"

using namespace across;

LogView::LogView(QObject* parent)
  : QObject(parent)
{}

void
LogView::init(setting::ConfigTools& config)
{
  colors_map = { { "Info", config.styleColor() },
                 { "Warning", config.warnColor() },
                 { "Error", config.warnColor() },
                 { "accepted", config.highlightColor() } };
}

void
LogView::clean()
{
  // TODO: use string list to impliment
  this->m_coreLog.clear();
}

void
LogView::push(const QString& msg)
{
  auto temp = msg;
  styleFomatter(temp);
  this->m_coreLog.append(temp);

  emit coreLogChanged();
}

const QString&
LogView::coreLog() const
{
  return m_coreLog;
}

void
LogView::styleFomatter(QString& msg)
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
LogView::setCoreLog(const QString& newCoreLog)
{
  // disable setting log from qml
}
