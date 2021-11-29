#ifndef ROUTING_MODEL_H
#define ROUTING_MODEL_H

#include "routinglist.h"

#include <QAbstractListModel>
#include <QObject>

namespace across {
class RoutingModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(RoutingList *list READ list WRITE setList NOTIFY listChanged)
  public:
    explicit RoutingModel(QObject *parent = nullptr);

    enum RoutingRoles {
        IDRole = Qt::UserRole,
        NameRole,
        DomainStrategyRole,
        DomainMatcherRole,
        RawRole,
        CreatedAtRole,
        ModifiedAtRole,
    };

    Q_ENUM(RoutingRoles);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    virtual QHash<int, QByteArray> roleNames() const override;

    RoutingList *list() const;

    void connectItems();

  public slots:
    void setList(RoutingList *list);

  signals:
    void listChanged();

  private:
    RoutingList *p_list;
    int m_old_rows;
};

} // namespace across

#endif // ROUTING_MODEL_H
