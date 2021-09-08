#include "logview.h"

using namespace across;

LogView::LogView(QObject* parent)
  : QObject(parent)
{}

void
LogView::clean()
{
  this->m_coreLog.clear();
}

void
LogView::push(const QString& msg)
{
  this->m_coreLog.append(msg);

  emit coreLogChanged();
}

const QString&
LogView::coreLog() const
{
  return m_coreLog;
}

void
LogView::setCoreLog(const QString& newCoreLog)
{
  // disable setting log from qml
}
