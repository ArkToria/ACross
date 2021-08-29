#include "vmessformmodel.h"

using namespace across;

VMessFormModel::VMessFormModel(QObject *parent) : QObject(parent)
{

}

QString VMessFormModel::alterID() const
{
    return m_alterID;
}

void VMessFormModel::setAlterID(const QString &newAlterID)
{
    if (m_alterID == newAlterID)
        return;
    m_alterID = newAlterID;
    emit alterIDChanged();
}

const QString &VMessFormModel::security() const
{
    return m_secutiry;
}

void VMessFormModel::setSecutiry(const QString &newSecutiry)
{
    if (m_secutiry == newSecutiry)
        return;
    m_secutiry = newSecutiry;
    emit secutiryChanged();
}

const QString &VMessFormModel::network() const
{
    return m_network;
}

void VMessFormModel::setNetwork(const QString &newNetwork)
{
    if (m_network == newNetwork)
        return;
    m_network = newNetwork;
    emit networkChanged();
}

const QString &VMessFormModel::host() const
{
    return m_host;
}

void VMessFormModel::setHost(const QString &newHost)
{
    if (m_host == newHost)
        return;

    m_host = newHost;

    emit hostChanged();
}

const QString &VMessFormModel::path() const
{
    return m_path;
}

void VMessFormModel::setPath(const QString &newPath)
{
    if (m_path == newPath)
        return;

    m_path = newPath;

    emit pathChanged();
}


bool VMessFormModel::tlsEnable() const
{
    return m_tlsEnable;
}

void VMessFormModel::setTlsEnable(bool newTlsEnable)
{
    if (m_tlsEnable == newTlsEnable)
        return;

    m_tlsEnable = newTlsEnable;

    emit tlsEnableChanged();
}
