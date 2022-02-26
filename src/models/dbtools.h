#ifndef DBTOOLS_H
#define DBTOOLS_H

#include "../view_models/logtools.h"

#include <QDateTime>
#include <QMap>
#include <QMutex>
#include <QThread>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "magic_enum.hpp"

namespace across {
enum SubscriptionType : int {
    base64 = 0,
    sip008 = 1,
    jsonraw = 2,
};

enum EntryType : int {
    vmess,
    shadowsocks,
    trojan,
    raw,
    scheme,
    unknown,
};

struct NodeInfo {
    qint64 id = 0;
    QString name = "";
    qint64 group_id = 0;
    QString group_name = "";
    qint64 routing_id = 0;
    QString routing_name;
    EntryType protocol;
    QString address = "";
    uint port = 0;
    QString password = "";
    QString raw = "";
    QString url = "";
    qint64 latency = -1;
    qint64 upload = 0;
    qint64 download = 0;
    QDateTime created_time;
    QDateTime modified_time;

    QVariantMap toVariantMap();
};

struct GroupInfo {
    qint64 id = 0;
    QString name = "";
    bool is_subscription = false;
    SubscriptionType type = base64;
    QString url = "";
    int cycle_time = 0;
    QDateTime created_time;
    QDateTime modified_time;
    int items = 0;

    QVariantMap toVariantMap();
};

struct RoutingInfo {
    qint64 id = 0;
    QString name = "";
    QString domain_strategy = "";
    QString domain_matcher = "";
    QString raw = "";
    QDateTime created_time;
    QDateTime modified_time;
};

enum RunTimeValues : int {
    CURRENT_NODE_ID = 0,
    CURRENT_GROUP_ID,
    DEFAULT_NODE_ID,
    DEFAULT_GROUP_ID,

    OTHER_INFO = 100,
};

struct RuntimeValue {
    const QString key = "";
    const int type = QMetaType::QString;
    QVariant value;

    RuntimeValue(QString key, const QVariant &value);
    RuntimeValue(RunTimeValues key, const QVariant &value);
    RuntimeValue(QString key, int type, const QVariant &value);
};
}

#endif // DBTOOLS_H
