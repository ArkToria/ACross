#include "logmodel.h"

using namespace across;

LogModel::LogModel(QObject* parent)
  : QAbstractListModel(parent)
  , p_list(nullptr)
{
}

int
LogModel::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid() || !p_list)
    return 0;

  return p_list->getSize();
}

QVariant
LogModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid() || !p_list) {
    return QVariant();
  }

  Q_ASSERT(index.model() == this);

  const QString item = p_list->getItem(index.row());

  switch (role) {
    case LogRole:
      return item;
    default:
      return QVariant();
  }

  return item;
}

QHash<int, QByteArray>
LogModel::roleNames() const
{
  static const QHash<int, QByteArray> roles{ { LogRole, "log" } };

  return roles;
}

LogList*
LogModel::list() const
{
  return p_list;
}

void
LogModel::connectItems()
{
  connect(p_list, &LogList::preItemsReset, this, [&]() {
    m_old_rows = p_list->getSize();
  });

  connect(p_list, &LogList::postItemsReset, this, [&]() {
    int index = p_list->getSize();

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

  connect(p_list, &LogList::preItemAppended, this, [&] {
    const int index = p_list->getSize();

    beginInsertRows(QModelIndex(), index, index);
  });

  connect(p_list, &LogList::postItemAppended, this, [&] { endInsertRows(); });

  connect(p_list, &LogList::preFirstItemRemoved, this, [&] {
    int index = 0;

    beginRemoveRows(QModelIndex(), index, index);
  });

  connect(
    p_list, &LogList::postFirstItemRemoved, this, [&] { endRemoveRows(); });
}

void
LogModel::setList(LogList* list)
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
