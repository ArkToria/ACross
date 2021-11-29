#ifndef ROUTINGLIST_H
#define ROUTINGLIST_H

#include "../models/dbtools.h"

#include <QObject>

namespace across {
class RoutingList : public QObject {
    Q_OBJECT
  public:
    explicit RoutingList(QObject *parent = nullptr);

    void init(QSharedPointer<across::DBTools> db);

    QList<RoutingInfo> items() const;
    void clearItems(int index = -1);
    void reloadItems();

  signals:
    void preItemsReset();
    void postItemsReset();

  private:
    std::shared_ptr<spdlog::logger> p_logger;
    QSharedPointer<DBTools> p_db;

    QList<RoutingInfo> m_routings;
};
} // namespace across

#endif // ROUTINGLIST_H
