#include "groupmodel.h"

using namespace across;

GroupModel::GroupModel(QObject* parent)
  : QAbstractListModel(parent)
  , p_list(nullptr)
{}

int
GroupModel::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid() || !p_list)
    return 0;

  if (p_list != nullptr) {
    return p_list->items().size();
  } else {
    return 0;
  }
}

QVariant
GroupModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid() || !p_list || index.row() >= rowCount()) {
    return QVariant();
  }

  const GroupInfo item = p_list->items().at(index.row());

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

QHash<int, QByteArray>
GroupModel::roleNames() const
{
  static const QHash<int, QByteArray> roles{ { GroupIDRole, "group_id" },
                                             { UrlRole, "url" },
                                             { NameRole, "name" },
                                             { ItemsRole, "items" },
                                             { CreatedAtRole, "createdAt" },
                                             { ModifiedAtRole, "modifiedAt" },
                                             { IsSubscriptionRole,
                                               "isSubscription" } };

  return roles;
}

GroupList*
GroupModel::list() const
{
  return p_list;
}

void
GroupModel::setList(GroupList* list)
{
  if (list == nullptr) {
    return;
  }

  beginResetModel();

  if (p_list) {
    p_list->disconnect(this);
  }

  p_list = list;

  if (p_list) {
    connect(p_list, &GroupList::preItemAppended, this, [&] {
      const int index = p_list->items().size();

      beginInsertRows(QModelIndex(), index, index);
    });

    connect(p_list, &GroupList::postItemAppended, this, [&] {
      // TODO: reset current index to the last one
      endInsertRows();
    });

    connect(p_list, &GroupList::preItemRemoved, this, [&](int index) {
      beginRemoveRows(QModelIndex(), index, index);
    });

    connect(
      p_list, &GroupList::postItemRemoved, this, [&] { endRemoveRows(); });

    connect(p_list, &GroupList::preLastItemRemoved, this, [&]() {
      int index = p_list->items().size();
      beginRemoveRows(QModelIndex(), index, index);
    });

    connect(
      p_list, &GroupList::postLastItemRemoved, this, [&] { endRemoveRows(); });

    connect(p_list,
            &GroupList::itemsSizeChanged,
            this,
            [&](int64_t group_id, int size) {
              for (int index = 0; index != p_list->items().size(); ++index) {
                if (group_id == p_list->items().at(index).id) {
                  QModelIndex topLeft = createIndex(index, 0);
                  QModelIndex bottomRight = createIndex(index + 1, 0);
                  emit dataChanged(topLeft, bottomRight);
                }
              }
            });
  }

  endResetModel();
}

bool
GroupModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (p_list == nullptr) {
    return false;
  }

  GroupInfo item = p_list->items().at(index.row());

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
