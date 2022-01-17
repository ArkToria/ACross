#ifndef NOTIFICATIONMODEL_H
#define NOTIFICATIONMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QProperty>
#include <QSharedPointer>

namespace across {

class Notification : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString title READ getTitle WRITE setTitle BINDABLE bindableTitle
                   NOTIFY titleChanged);
    Q_PROPERTY(QString message READ getMessage WRITE setMessage BINDABLE
                   bindableMessage NOTIFY messageChanged);
    Q_PROPERTY(qreal from READ getFrom WRITE setFrom BINDABLE bindableFrom
                   NOTIFY fromChanged);
    Q_PROPERTY(
        qreal to READ getTo WRITE setTo BINDABLE bindableTo NOTIFY toChanged);
    Q_PROPERTY(qreal value READ getValue WRITE setValue BINDABLE bindableValue
                   NOTIFY valueChanged);
    Q_PROPERTY(bool pin READ getPin WRITE setPin NOTIFY pinChanged);
    Q_PROPERTY(int index READ getIndex WRITE setIndex NOTIFY indexChanged);

  public:
    explicit Notification(int id, QObject *parent = nullptr);

    QString getTitle() const { return title.value(); }
    void setTitle(const QString &title) { this->title = title; };
    QBindable<QString> bindableTitle() { return &title; }
    QString getMessage() const { return message.value(); }
    void setMessage(const QString &message) { this->message = message; }
    QBindable<QString> bindableMessage() { return &message; }
    qreal getFrom() const { return from.value(); }
    void setFrom(const qreal &from) { this->from = from; }
    QBindable<qreal> bindableFrom() { return &from; }
    qreal getTo() const { return to.value(); }
    void setTo(const qreal &to) { this->to = to; }
    QBindable<qreal> bindableTo() { return &to; }
    qreal getValue() const { return value.value(); }
    void setValue(const qreal &value) { this->value = value; }
    QBindable<qreal> bindableValue() { return &value; }
    bool getPin() const { return pin; }
    void setPin(const bool pin) { this->pin = pin; }
    int getIndex() const { return index; }
    void setIndex(int index) { this->index = index;}

  signals:
    void titleChanged();
    void messageChanged();
    void fromChanged();
    void toChanged();
    void valueChanged();
    void pinChanged();
    void indexChanged();
    void propertiesChanged();

  private:
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(Notification, QString, title, "",
                                         &Notification::titleChanged);
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(Notification, QString, message, "",
                                         &Notification::messageChanged);
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(Notification, qreal, from, 0.0,
                                         &Notification::fromChanged);
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(Notification, qreal, to, 1.0,
                                         &Notification::toChanged);
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(Notification, qreal, value, 0.0,
                                         &Notification::valueChanged);

    bool pin;
    int index = 0;
};

class NotificationModel : public QAbstractListModel {
    Q_OBJECT

  public:
    explicit NotificationModel(QObject *parent = nullptr);

    enum NotificationRoles {
        NotificationIndexRole = Qt::UserRole,
        TitleRole,
        MessageRole,
        FromRole,
        ToRole,
        ValueRole,
    };

    Q_ENUM(NotificationRoles);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Q_INVOKABLE across::Notification *append(const QString &title,
                                             const QString &message, qreal from,
                                             qreal to, qreal value);

    Q_INVOKABLE void remove(const int id);

  private:
    QVector<Notification *> notifications;
};

} // namespace across

#endif // NOTIFICATIONMODEL_H
