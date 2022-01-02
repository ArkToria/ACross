#include "groupmodel.h"

using namespace across;

GroupModel::GroupModel(QObject *parent)
    : QAbstractListModel(parent), p_list(nullptr) {}

int GroupModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid() || p_list == nullptr)
        return 0;
    else
        return p_list->items().size();
}

QVariant GroupModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || p_list == nullptr || index.row() >= rowCount())
        return {};

    const GroupInfo item = p_list->items().at(index.row());

    switch (role) {
    case GroupIDRole:
        return static_cast<qsizetype>(item.id);
    case NameRole:
        return item.name;
    case IsSubscriptionRole:
        return item.is_subscription;
    case TypeRole:
        return item.type;
    case UrlRole:
        return item.url;
    case CycleTimeRole:
        return item.cycle_time;
    case CreatedAtRole:
        return item.created_time.toString(DATE_TIME_FORMAT());
    case ModifiedAtRole:
        return item.modified_time.toString(DATE_TIME_FORMAT());
    case ItemsRole:
        return item.items;
    default:
        return {};
    }
}

QHash<int, QByteArray> GroupModel::roleNames() const {
    static const QHash<int, QByteArray> roles{
        {GroupIDRole, "group_id"},
        {NameRole, "name"},
        {IsSubscriptionRole, "isSubscription"},
        {TypeRole, "type"},
        {UrlRole, "url"},
        {CycleTimeRole, "cycleTime"},
        {CreatedAtRole, "createdAt"},
        {ModifiedAtRole, "modifiedAt"},
        {ItemsRole, "items"},
    };

    return roles;
}

GroupList *GroupModel::list() const { return p_list; }

void GroupModel::connectItems() {
    connect(p_list, &GroupList::preItemsReset, this,
            [&]() { m_old_rows = p_list->items().size(); });

    connect(p_list, &GroupList::postItemsReset, this, [&]() {
        int index = p_list->items().size();

        QModelIndex topLeft = createIndex(0, 0);
        QModelIndex bottomRight = createIndex(index, 0);

        if (m_old_rows > index) {
            qInfo() << index << m_old_rows << rowCount();
            beginRemoveRows(QModelIndex(), index, m_old_rows);
            endRemoveRows();
        } else if (m_old_rows < index) {
            beginInsertRows(QModelIndex(), m_old_rows, index - 1);
            endInsertRows();
        }

        emit dataChanged(topLeft, bottomRight);
    });

    connect(p_list, &GroupList::itemInfoChanged, this, [&](int index) {
        emit dataChanged(createIndex(index, 0), createIndex(index + 1, 0));
    });
}

void GroupModel::setList(GroupList *list) {
    if (list == nullptr)
        return;

    beginResetModel();

    if (p_list != nullptr)
        p_list->disconnect(this);

    p_list = list;

    connectItems();

    emit listChanged();

    endResetModel();
}

bool GroupModel::setData(const QModelIndex &index, const QVariant &value,
                         int role) {
    if (p_list == nullptr)
        return false;

    GroupInfo item = p_list->items().at(index.row());

    switch (role) {
    case NameRole:
        item.name = value.toString();
        break;
    case IsSubscriptionRole:
        item.is_subscription = value.toBool();
        break;
    case UrlRole:
        item.url = value.toString();
        break;
    case CreatedAtRole:
        item.created_time.setTime(value.toTime());
        break;
    case ModifiedAtRole:
        item.modified_time.setTime(value.toTime());
        break;
    }

    if (data(index, role) != value) {
        emit dataChanged(index, index, QList<int>() << role);
        return true;
    }
    return false;
}
