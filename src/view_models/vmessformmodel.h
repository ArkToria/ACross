#ifndef VMESSFORMMODEL_H
#define VMESSFORMMODEL_H

#include <QObject>

namespace across {
class VMessFormModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString alterID READ alterID WRITE setAlterID NOTIFY alterIDChanged)
    Q_PROPERTY(QString security READ security WRITE setSecutiry NOTIFY secutiryChanged)
    Q_PROPERTY(QString network READ network WRITE setNetwork NOTIFY networkChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(bool tlsEnable READ tlsEnable WRITE setTlsEnable NOTIFY tlsEnableChanged)
public:
    explicit VMessFormModel(QObject *parent = nullptr);

public:
    QString alterID() const;

    const QString &security() const;

    const QString &network() const;

    const QString &host() const;

    const QString &path() const;

    bool tlsEnable() const;

public slots:
    void setAlterID(const QString &newAlterID);

    void setSecutiry(const QString &newSecutiry);

    void setNetwork(const QString &newNetwork);

    void setHost(const QString &newHost);

    void setPath(const QString &newPath);

    void setTlsEnable(bool newTlsEnable);

signals:
    void alterIDChanged();

    void secutiryChanged();

    void networkChanged();

    void hostChanged();

    void pathChanged();

    void tlsEnableChanged();

private:
    QString m_alterID = "0";
    QString m_secutiry = "";
    QString m_network = "";
    QString m_host = "";
    QString m_path = "";
    bool m_tlsEnable = false;
};
}

#endif // VMESSFORMMODEL_H
