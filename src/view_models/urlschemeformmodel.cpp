#include "urlschemeformmodel.h"

using namespace across;

URLSchemeFormModel::URLSchemeFormModel(QObject* parent)
  : QObject(parent)
{}

const QString&
URLSchemeFormModel::urlScheme() const
{
  return m_urlScheme;
}

void
URLSchemeFormModel::setURLScheme(const QString& newUrlScheme)
{
  if (m_urlScheme == newUrlScheme)
    return;

  m_urlScheme = newUrlScheme;

  emit urlSchemeChanged();
}
