#include "groupmodel.h"

using namespace across;

GroupModel::GroupModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_list(nullptr) {
}

int GroupModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid() || !m_list)
        return 0;

    if (m_list != nullptr) {
        return m_list->items().size();
    } else {
        return 0;
    }
}

QVariant GroupModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || !m_list || index.row() >= rowCount()) {
        return QVariant();
    }

    const GroupInfo item = m_list->items().at(index.row());

    switch (role) {
    case GroupIDRole:
        return static_cast<qsizetype>(item.id);
    case NameRole:
        return item.name;
    case IsSubscriptionRole:
        return item.isSubscription;
    case TypeRole:
        return item.type;
    case UrlRole:
        return item.url;
    case CycleTimeRole:
        return item.cycle_time;
    case CreatedAtRole:
        return item.created_time.toString("MM/dd/yyyy HH:mm:ss");
    case ModifiedAtRole:
        return item.modified_time.toString("MM/dd/yyyy HH:mm:ss");
    case ItemsRole:
        return item.items;
    default:
        return QVariant();
    }

    return QVariant();
}

QHash<int, QByteArray> GroupModel::roleNames() const {
    static const QHash<int, QByteArray> roles {
        { GroupIDRole, "group_id" },
        { UrlRole, "url" },
        { NameRole, "name" },
        { ItemsRole, "items" },
        { CreatedAtRole, "createdAt" },
        { ModifiedAtRole, "modifiedAt" },
        { IsSubscriptionRole, "isSubscription" }
    };

    return roles;
}

GroupList *GroupModel::list() const {
    return m_list;
}

void GroupModel::setList(GroupList *list) {
    if (list == nullptr) {
        return;
    }

    beginResetModel();

    if (m_list) {
        m_list->disconnect(this);
    }

    m_list = list;

    if (m_list) {
        connect(m_list, &GroupList::preItemAppended, this, [&] {
            const int index = m_list->items().size();

            beginInsertRows(QModelIndex(), index, index);
        });

        connect(m_list, &GroupList::postItemAppended, this, [&] {
            // TODO: reset current index to the last one
            endInsertRows();
        });

        connect(m_list, &GroupList::preItemRemoved, this, [&](int index) {
          beginRemoveRows(QModelIndex(), index, index);
        });

        connect(m_list, &GroupList::postItemRemoved, this, [&] {
            endRemoveRows();
        });

        connect(m_list, &GroupList::preLastItemRemoved, this, [&]() {
          int index = m_list->items().size();
          beginRemoveRows(QModelIndex(), index, index);
        });

        connect(m_list, &GroupList::postLastItemRemoved, this, [&] {
          endRemoveRows();
        });
    }

    endResetModel();
}

bool GroupModel::setData(const QModelIndex &index, const QVariant &value,
                         int role) {
    if (m_list == nullptr) {
        return false;
    }

    GroupInfo item = m_list->items().at(index.row());

    switch (role) {
    case NameRole:
        item.name = value.toString();
        break;
    case IsSubscriptionRole:
        item.isSubscription = value.toBool();
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
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}
