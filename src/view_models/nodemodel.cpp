#include "nodemodel.h"

using namespace across;

NodeModel::NodeModel(QObject* parent)
  : QAbstractListModel(parent)
  , p_list(nullptr)
{}

int
NodeModel::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid() || !p_list)
    return 0;

  return p_list->items().size();
}

QVariant
NodeModel::data(const QModelIndex& index, int role) const
{
  const QString dateTimeFormat("mm/dd/yyyy hh:mm:ss");

  if (!index.isValid() || !p_list) {
    return QVariant();
  }

  Q_ASSERT(index.model() == this);

  const NodeInfo item = p_list->items().at(index.row());

  switch (role) {
    case IDRole:
      return QVariant().fromValue(item.id);
    case NameRole:
      // qt5 display emoji string error
      return item.name;
    case GroupRole:
      return item.group;
    case ProtocolTypeRole:
      return magic_enum::enum_name(item.protocol).data();
    case AddressRole:
      return item.address;
    case PortRole:
      return item.port;
    case PasswordRole:
      return item.password;
    case CreatedAtRole:
      return item.created_time.toString(dateTimeFormat);
    case ModifiedAtRole:
      return item.modified_time.toString(dateTimeFormat);
  }

  return QVariant();
}

QHash<int, QByteArray>
NodeModel::roleNames() const
{
  static const QHash<int, QByteArray> roles{
    { IDRole, "id" },
    { NameRole, "name" },
    { GroupRole, "group" },
    { AddressRole, "address" },
    { PortRole, "port" },
    { PasswordRole, "password" },
    { CreatedAtRole, "createdAt" },
    { ModifiedAtRole, "modifiedAt" },
    { ProtocolTypeRole, "protocol" },
  };

  return roles;
}

NodeList*
NodeModel::list() const
{
  return p_list;
}

void
NodeModel::connectItems()
{
  connect(p_list, &NodeList::preItemsReset, this, [&]() {
    m_old_rows = p_list->items().size();
  });

  connect(p_list, &NodeList::postItemsReset, this, [&]() {
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

void
NodeModel::setList(NodeList* list)
{
  beginResetModel();

  if (p_list) {
    p_list->disconnect(this);
  }

  p_list = list;

  if (p_list) {
    connectItems();
  }

  emit listChanged();

  endResetModel();
}
