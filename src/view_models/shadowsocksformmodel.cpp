#include "shadowsocksformmodel.h"

using namespace across;

ShadowsocksFormModel::ShadowsocksFormModel(QObject* parent)
  : QObject(parent)
{}

const QString&
ShadowsocksFormModel::security() const
{
  return m_security;
}

void
ShadowsocksFormModel::setSecurity(const QString& newSecurity)
{
  if (m_security == newSecurity)
    return;
  m_security = newSecurity;
  emit securityChanged();
}

bool
ShadowsocksFormModel::ivCheck() const
{
  return m_ivCheck;
}

void
ShadowsocksFormModel::setIvCheck(bool newIvCheck)
{
  if (m_ivCheck == newIvCheck)
    return;
  m_ivCheck = newIvCheck;
  emit ivCheckChanged();
}
