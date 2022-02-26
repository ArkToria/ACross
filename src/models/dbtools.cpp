#include "dbtools.h"

#include <utility>

using namespace across;
using namespace across::utils;


RuntimeValue::RuntimeValue(QString key, const QVariant &value)
    : key(std::move(key)), type(value.typeId()), value(value) {}

RuntimeValue::RuntimeValue(const RunTimeValues key, const QVariant &value)
    : key(magic_enum::enum_name(key).data()), type(value.typeId()),
      value(value) {}

RuntimeValue::RuntimeValue(QString key, int type, const QVariant &value)
    : key(std::move(key)), type(type), value(value) {}

QVariantMap NodeInfo::toVariantMap() {
    return QVariantMap{
        {"nodeID", this->id},
        {"name", this->name},
        {"group", this->group_name},
        {"groupID", this->group_id},
        {"routing", this->routing_name},
        {"routingID", this->routing_id},
        {"address", this->address},
        {"port", this->port},
        {"password", this->password},
        {"raw", this->raw},
        {"url", this->url},
        {"protocol", this->protocol},
        {"latency", this->latency},
        {"upload", this->upload},
        {"download", this->download},
        {"createdAt", this->created_time.toSecsSinceEpoch()},
        {"modifiedAt", this->modified_time.toSecsSinceEpoch()},
    };
}

QVariantMap GroupInfo::toVariantMap() {
    auto type_name = magic_enum::enum_name(this->type);

    return QVariantMap{
        {"groupID", this->id},
        {"name", this->name},
        {"isSubscription", this->is_subscription},
        {"type", type_name.data()},
        {"url", this->url},
        {"cycleTime", this->cycle_time},
        {"createdAt", this->created_time.toSecsSinceEpoch()},
        {"modifiedAt", this->modified_time.toSecsSinceEpoch()},
        {"items", this->items},
    };
}
