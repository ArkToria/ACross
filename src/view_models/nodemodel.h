#ifndef NODEMODEL_H
#define NODEMODEL_H

#include "nodelist.h"
#include "magic_enum.hpp"

#include <QAbstractListModel>

namespace across {
class NodeModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(NodeList* list READ list WRITE setList NOTIFY listChanged)
public:
    explicit NodeModel(QObject* parent = nullptr);

    enum Roles {
        IDRole = Qt::UserRole,
        NameRole,
        GroupRole,
        ProtocolTypeRole,
        AddressRole,
        PortRole,
        PasswordRole,
        RawRole,
        CreatedAtRole,
        ModifiedAtRole,
    };

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index,
        int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    NodeList* list() const;

    void connectItemsReset();

public slots:
    void setList(NodeList* list);

signals:
    void listChanged();

private:
    NodeList* m_list;
    int m_old_rows;
};
}

#endif // NODEMODEL_H
