#ifndef SHADOWSOCKSFORMMODEL_H
#define SHADOWSOCKSFORMMODEL_H

#include <QObject>
namespace across {
class ShadowsocksFormModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString security READ security WRITE setSecurity NOTIFY securityChanged)
    Q_PROPERTY(bool ivCheck READ ivCheck WRITE setIvCheck NOTIFY ivCheckChanged)
public:
    explicit ShadowsocksFormModel(QObject *parent = nullptr);

public:
    const QString &security() const;

    bool ivCheck() const;
    void setIvCheck(bool newIvCheck);

public slots:
    void setSecurity(const QString &newSecurity);

signals:
    void securityChanged();

    void ivCheckChanged();

private:
    QString m_security = "aes-256-gcm";
    bool m_ivCheck = false;
};
}


#endif // SHADOWSOCKSFORMMODEL_H
