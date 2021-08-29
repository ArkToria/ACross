#ifndef TROJANFORMMODEL_H
#define TROJANFORMMODEL_H

#include <QObject>

namespace across {
class TrojanFormModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString network READ network WRITE setNetwork NOTIFY networkChanged)
    Q_PROPERTY(QString security READ security WRITE setSecurity NOTIFY securityChanged)
    Q_PROPERTY(QString serverName READ serverName WRITE setServerName NOTIFY serverNameChanged)
    Q_PROPERTY(QString alpn READ alpn WRITE setAlpn NOTIFY alpnChanged)
public:
    explicit TrojanFormModel(QObject* parent = nullptr);

public:
    const QString& serverName() const;

    const QString& network() const;

    const QString& security() const;

    const QString& alpn() const;

public slots:
    void setNetwork(const QString& newNetwork);

    void setSecurity(const QString& newSecurity);

    void setServerName(const QString& newServerName);

    void setAlpn(const QString& newAlpn);

signals:
    void serverNameChanged();

    void listChanged();

    void networkChanged();

    void securityChanged();

    void alpnChanged();

private:
    QString m_network = "tcp";
    QString m_security = "tls";
    QString m_serverName = "";
    QString m_alpn = "h2+http/1.1";
};
}

#endif // TROJANFORMMODEL_H
