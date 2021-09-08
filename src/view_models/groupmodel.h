#ifndef GROUPINFOMODEL_H
#define GROUPINFOMODEL_H

#include "grouplist.h"
#include <QAbstractListModel>
#include <QDateTime>

namespace across {
class GroupModel : public QAbstractListModel
{
  Q_OBJECT
  Q_PROPERTY(GroupList* list READ list WRITE setList NOTIFY listChanged)
public:
  explicit GroupModel(QObject* parent = nullptr);

  enum GroupRoles
  {
    GroupIDRole = Qt::UserRole,
    NameRole,
    IsSubscriptionRole,
    TypeRole,
    UrlRole,
    CycleTimeRole,
    CreatedAtRole,
    ModifiedAtRole,
    ItemsRole,
  };

  Q_ENUM(GroupRoles);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

  bool setData(const QModelIndex& index,
               const QVariant& value,
               int role = Qt::EditRole) override;

  virtual QHash<int, QByteArray> roleNames() const override;

  GroupList* list() const;

public slots:
  void setList(GroupList* list);

signals:
  void listChanged();

  void nodeListChanged();

private:
  GroupList* m_list;
};
}

#endif // GROUPINFOMODEL_H
