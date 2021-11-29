#include "routinglist.h"

using namespace across;

RoutingList::RoutingList(QObject *parent) : QObject(parent) {}

void RoutingList::init(QSharedPointer<across::DBTools> db) {
    if (auto app_logger = spdlog::get("app"); app_logger != nullptr) {
        p_logger = app_logger->clone("routings");
    } else {
        qCritical("Failed to start logger");
        return;
    }

    p_db = db;

    reloadItems();
}

QList<RoutingInfo> RoutingList::items() const { return m_routings; }

void RoutingList::clearItems(int index) {
    if (index < 0) {
        m_routings.clear();
    } else if (index < m_routings.size()) {
        m_routings.remove(index);
    }
}

void RoutingList::reloadItems() {
    if (auto result = p_db->reloadAllRoutingsInfo();
        result.type() == QSqlError::NoError) {
        emit preItemsReset();
        m_routings = p_db->getAllRoutingsInfo();
        emit postItemsReset();

        if (m_routings.isEmpty()) {
            p_logger->warn("No routing items from database");
            return;
        }
    }
}
