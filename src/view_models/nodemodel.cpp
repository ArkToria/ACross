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

  if (index.model() != this) {
    return {};
  }

  const NodeInfo item = p_list->items().at(index.row());

  switch (role) {
    case IDRole:
      return QVariant::fromValue(item.id);
    case NameRole:
      return item.name;
    case GroupRole:
      return item.group_name;
    case ProtocolTypeRole:
      return magic_enum::enum_name(item.protocol).data();
    case AddressRole:
      return item.address;
    case PortRole:
      return item.port;
    case PasswordRole:
      return item.password;
    case URLRole:
      return item.url;
    case LatencyRole:
      return item.latency;
    case UploadRole:
      return item.upload;
    case DownloadRole:
      return item.download;
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
  static const QHash<int, QByteArray> roles = {
    { IDRole, "nodeID" },
    { NameRole, "name" },
    { GroupRole, "group" },
    { AddressRole, "address" },
    { PortRole, "port" },
    { PasswordRole, "password" },
    { URLRole, "url" },
    { ProtocolTypeRole, "protocol" },
    { LatencyRole, "latency"},
    { CreatedAtRole, "createdAt" },
    { ModifiedAtRole, "modifiedAt" },
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
  connect(p_list, &NodeList::itemReset, this, [&](int index) {
    QModelIndex topLeft = createIndex(index, 0);
    QModelIndex bottomRight = createIndex(index + 1, 0);
    emit dataChanged(topLeft, bottomRight);
  });

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
  if (list == nullptr)
    return;

  beginResetModel();

  if (p_list)
    p_list->disconnect(this);

  p_list = list;
  connectItems();
  emit listChanged();
  endResetModel();
}
