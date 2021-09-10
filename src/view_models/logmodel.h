#ifndef LOGMODEL_H
#define LOGMODEL_H

#include "loglist.h"

#include <QAbstractListModel>

namespace across {
class LogModel : public QAbstractListModel
{
  Q_OBJECT
  Q_PROPERTY(LogList* list READ list WRITE setList NOTIFY listChanged)
public:
  LogModel(QObject* parent = nullptr);

  enum Roles
  {
    LogRole = Qt::UserRole,
  };

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

  virtual QHash<int, QByteArray> roleNames() const override;

  LogList* list() const;

  void connectItems();

public slots:
  void setList(LogList* list);

signals:
  void listChanged();

private:
  LogList* p_list;
  int m_old_rows;
};
}

#endif // LOGMODEL_H
