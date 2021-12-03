#include "routingmodel.h"

using namespace across;

RoutingModel::RoutingModel(QObject *parent)
    : QAbstractListModel(parent), p_list(nullptr) {}

int RoutingModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid() || p_list == nullptr)
        return 0;
    else
        return p_list->items().size();
}

QVariant RoutingModel::data(const QModelIndex &index, int role) const {
    const QString dateTimeFormat("mm/dd/yyyy hh:mm:ss");

    if (!index.isValid() || p_list == nullptr || index.row() >= rowCount())
        return QVariant();

    const RoutingInfo item = p_list->items().at(index.row());

    switch (role) {
    case IDRole:
        return static_cast<qsizetype>(item.id);
    case NameRole:
        return item.name;
    case DomainStrategyRole:
        return item.domain_strategy;
    case DomainMatcherRole:
        return item.domain_matcher;
    case RawRole:
        return item.raw;
    case CreatedAtRole:
        return item.created_time.toString(dateTimeFormat);
    case ModifiedAtRole:
        return item.modified_time.toString(dateTimeFormat);
    default:
        break;
    }

    return QVariant();
}

bool RoutingModel::setData(const QModelIndex &index, const QVariant &value,
                           int role) {
    if (p_list == nullptr)
        return false;

    RoutingInfo item = p_list->items().at(index.row());

    switch (role) {
    case NameRole:
        item.name = value.toString();
        break;
    case DomainStrategyRole:
        item.domain_strategy = value.toString();
        break;
    case DomainMatcherRole:
        item.domain_matcher = value.toString();
        break;
    case RawRole:
        item.raw = value.toString();
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

QHash<int, QByteArray> RoutingModel::roleNames() const {
    const QHash<int, QByteArray> roles = {
        {IDRole, "routingID"},
        {NameRole, "name"},
        {DomainStrategyRole, "domainStrategy"},
        {DomainMatcherRole, "domainMatcher"},
        {RawRole, "raw"},
        {CreatedAtRole, "createdAt"},
        {ModifiedAtRole, "modifiedAt"},
    };

    return roles;
}

RoutingList *RoutingModel::list() const { return p_list; }

void RoutingModel::connectItems() {
    connect(p_list, &RoutingList::preItemsReset, this,
            [&]() { m_old_rows = p_list->items().size(); });

    connect(p_list, &RoutingList::postItemsReset, this, [&]() {
        int index = p_list->items().size();

        QModelIndex topLeft = createIndex(0, 0);
        QModelIndex bottomRight = createIndex(index, 0);

        if (m_old_rows > index) {
            beginRemoveRows(QModelIndex(), index, m_old_rows);
            endRemoveRows();
        } else if (m_old_rows < index) {
            beginInsertRows(QModelIndex(), m_old_rows, index - 1);
            endInsertRows();
        }

        emit dataChanged(topLeft, bottomRight);
    });
}

void RoutingModel::setList(RoutingList *list) {
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
