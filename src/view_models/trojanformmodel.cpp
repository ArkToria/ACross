#include "trojanformmodel.h"

using namespace across;

TrojanFormModel::TrojanFormModel(QObject* parent)
{
}

const QString& TrojanFormModel::serverName() const
{
    return m_serverName;
}

void TrojanFormModel::setServerName(const QString& newServerName)
{
    if (m_serverName == newServerName) {
        return;
    }

    m_serverName = newServerName;

    emit serverNameChanged();
}

const QString& TrojanFormModel::network() const
{
    return m_network;
}

void TrojanFormModel::setNetwork(const QString& newNetwork)
{
    if (m_network == newNetwork) {
        return;
    }

    m_network = newNetwork;

    emit networkChanged();
}

const QString& TrojanFormModel::security() const
{
    return m_security;
}

void TrojanFormModel::setSecurity(const QString& newSecurity)
{
    if (m_security == newSecurity)
        return;
    m_security = newSecurity;
    emit securityChanged();
}

const QString& TrojanFormModel::alpn() const
{
    return m_alpn;
}

void TrojanFormModel::setAlpn(const QString& newAlpn)
{
    if (m_alpn == newAlpn) {
        return;
    }

    m_alpn = newAlpn;

    emit alpnChanged();
}
