#ifndef NODEMODEL_H
#define NODEMODEL_H

#include "buildinfo.h"
#include "nodelist.h"

#include "magic_enum.hpp"
#include <QAbstractListModel>
#include <QProcess>

namespace across {
class NodeModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(NodeList *list READ list WRITE setList NOTIFY listChanged)
  public:
    explicit NodeModel(QObject *parent = nullptr);

    enum Roles {
        IDRole = Qt::UserRole,
        NameRole,
        GroupRole,
        GroupIDRole,
        RoutingRole,
        RoutingIDRole,
        ProtocolTypeRole,
        AddressRole,
        PortRole,
        PasswordRole,
        RawRole,
        URLRole,
        LatencyRole,
        UploadRole,
        DownloadRole,
        CreatedAtRole,
        ModifiedAtRole,
    };

    [[nodiscard]] int
    rowCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index,
                                int role = Qt::DisplayRole) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    [[nodiscard]] NodeList *list() const;

    void connectItems();

  public slots:
    void setList(across::NodeList *list);

  signals:
    void listChanged();

  private:
    NodeList *p_list;
    int m_old_rows = -1;
};
} // namespace across

#endif // NODEMODEL_H
