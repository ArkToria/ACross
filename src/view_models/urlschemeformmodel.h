#ifndef URLSCHEMEFORMMODEL_H
#define URLSCHEMEFORMMODEL_H

#include <QObject>

namespace across {
class URLSchemeFormModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString urlScheme READ urlScheme WRITE setURLScheme NOTIFY urlSchemeChanged)
    QString m_urlScheme;

public:
    explicit URLSchemeFormModel(QObject *parent = nullptr);

    const QString &urlScheme() const;
    void setURLScheme(const QString &newUrlScheme);

signals:
    void urlSchemeChanged();
};
}

#endif // URLSCHEMEFORMMODEL_H
