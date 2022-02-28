#include "notificationmodel.h"

using namespace across;

Notification::Notification(int index, QObject *parent) : QObject(parent) {
    this->index = index;

    connect(this, &Notification::titleChanged,
            &Notification::propertiesChanged);
    connect(this, &Notification::messageChanged,
            &Notification::propertiesChanged);
    connect(this, &Notification::fromChanged, &Notification::propertiesChanged);
    connect(this, &Notification::toChanged, &Notification::propertiesChanged);
    connect(this, &Notification::valueChanged,
            &Notification::propertiesChanged);
    connect(this, &Notification::pinChanged, &Notification::propertiesChanged);
    connect(this, &Notification::indexChanged,
            &Notification::propertiesChanged);
}

NotificationModel::NotificationModel(QObject *parent)
    : QAbstractListModel(parent) {}

void NotificationModel::init(QObject *popNotify) {
    this->p_popNotify = popNotify;
}

QVariant NotificationModel::headerData(int section, Qt::Orientation orientation,
                                       int role) const {
    if (section >= rowCount())
        return {};

    if (role == NotificationRoles::TitleRole)
        return notifications.at(section)->getTitle();

    return {};
}

int NotificationModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    else
        return notifications.size();
}

QVariant NotificationModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= rowCount())
        return QVariant();

    const auto item = notifications.at(index.row());

    switch (role) {
    case NotificationIndexRole:
        return static_cast<qsizetype>(item->getIndex());
    case TitleRole:
        return item->getTitle();
    case MessageRole:
        return item->getMessage();
    case FromRole:
        return item->getFrom();
    case ToRole:
        return item->getTo();
    case ValueRole:
        return item->getValue();
    default:
        return {};
    }
}

QHash<int, QByteArray> NotificationModel::roleNames() const {
    static const QHash<int, QByteArray> roles{
        {NotificationIndexRole, "id"},
        {TitleRole, "title"},
        {MessageRole, "message"},
        {FromRole, "from"},
        {ToRole, "to"},
        {ValueRole, "value"},
    };

    return roles;
}

Qt::ItemFlags NotificationModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled;
}

across::Notification *
NotificationModel::append(const QString &title, const QString &message = "",
                          qreal from = 0.0, qreal to = 1.0, qreal value = 0.0) {
    int index = notifications.size();
    Notification *noti = new Notification(index, this);

    noti->setTitle(title);
    noti->setMessage(message);
    noti->setFrom(from);
    noti->setTo(to);
    noti->setValue(value);

    auto rcnt = rowCount();
    beginInsertRows(QModelIndex(), rcnt, rcnt);
    connect(noti, &Notification::propertiesChanged, this, [this, noti]() {
        QModelIndex topLeft = createIndex(noti->getIndex(), 0);
        QModelIndex bottomRight = createIndex(noti->getIndex(), 0);
        emit NotificationModel::dataChanged(topLeft, bottomRight);
    });
    notifications.emplace_back(noti);
    endInsertRows();
    return noti;
}

across::Notification *NotificationModel::notify(const QString &title,
                                                const QString &message,
                                                double from, double to,
                                                double value, int duration) {
    if (p_popNotify) {
        QVariant result;
        qDebug() << QMetaObject::invokeMethod(
            p_popNotify, "notify", Q_RETURN_ARG(QVariant, result),
            Q_ARG(QVariant, title), Q_ARG(QVariant, message),
            Q_ARG(QVariant, from), Q_ARG(QVariant, to), Q_ARG(QVariant, value),
            Q_ARG(QVariant, duration));
        return qvariant_cast<across::Notification *>(result);
    }
    return nullptr;
}

void NotificationModel::remove(const int index) {
    if (index >= notifications.size() || index < 0)
        return;

    auto rcnt = index;
    beginRemoveRows(QModelIndex(), rcnt, rcnt);

    QModelIndex topLeft = createIndex(rcnt, 0);
    QModelIndex bottomRight = createIndex(rcnt, 0);
    emit NotificationModel::dataChanged(topLeft, bottomRight);
    Notification *noti = notifications[index];
    for (int i = index + 1; i < notifications.size(); i++) {
        notifications[i]->setIndex(notifications[i]->getIndex() - 1);
    }
    notifications.remove(index);
    noti->deleteLater();
    endRemoveRows();
}