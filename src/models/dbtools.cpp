#include "dbtools.h"

using namespace across;
using namespace across::utils;

DBTools::DBTools(QObject *parent) {}

DBTools::~DBTools() { close(); }

void DBTools::init(const QString &db_path) {
    if (auto app_logger = spdlog::get("app"); app_logger != nullptr) {
        p_logger = app_logger->clone("database");
    } else {
        qCritical("Failed to start logger");
        return;
    }

    m_db_path = db_path;

    reload();
}

void DBTools::reload() {
    close();

    do {
        if (m_db_path.isEmpty()) {
            p_logger->error("Failed to load database on path");
            break;
        } else {
            p_logger->info("Open database on path: {}",
                           m_db_path.toStdString());
        }

        QSqlError result;
        if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
            p_logger->error("Database driver is unavailable: {}",
                            m_db.driverName().toStdString());
            break;
        }

        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(m_db_path);
        if (!m_db.open()) {
            p_logger->error("Database open failed");
            break;
        }

        p_logger->info("Successfully opened the database");

        if (result = createDefaultTables();
            result.type() != QSqlError::NoError) {
            p_logger->error("Failed to create tables: {}",
                            result.text().toStdString());
            break;
        }

        if (result = createDefaultValues();
            result.type() != QSqlError::NoError) {
            p_logger->error("Failed to create values: {}",
                            result.text().toStdString());
            break;
        }

        if (result = createDefaultGroup();
            result.type() != QSqlError::NoError) {
            p_logger->error(
                "Failed to insert default_group to groups table: {}",
                result.text().toStdString());
            break;
        }

        if (result = createDefaultRouting();
            result.type() != QSqlError::NoError) {
            p_logger->error(
                "Failed to insert default_routing to routings table: {}",
                result.text().toStdString());
            break;
        }

    } while (false);
}

QSqlError DBTools::createDefaultTables() {
    QSqlError result;
    const QStringList tables = {
        {"CREATE TABLE IF NOT EXISTS groups("
         "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
         "Name TEXT UNIQUE NOT NULL,"
         "IsSubscription BOOLEAN NOT NULL,"
         "Type INTEGER NOT NULL,"
         "Url TEXT,"
         "CycleTime INTEGER,"
         "CreatedAt INT64 NOT NULL,"
         "ModifiedAt INT64 NOT NULL);"},
        {"CREATE TABLE IF NOT EXISTS runtime("
         "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
         "Name TEXT UNIQUE NOT NULL,"
         "Type INTEGER NOT NULL,"
         "Value TEXT);"},
        {"CREATE TABLE IF NOT EXISTS nodes("
         "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
         "Name TEXT NOT NULL,"
         "GroupID INTEGER NOT NULL,"
         "GroupName TEXT NOT NULL,"
         "RoutingID INTEGER NOT NULL,"
         "RoutingName TEXT NOT NULL,"
         "Protocol INTEGER NOT NULL,"
         "Address TEXT NOT NULL,"
         "Port INTEGER NOT NULL,"
         "Password TEXT,"
         "Raw TEXT NOT NULL,"
         "URL TEXT NOT NULL,"
         "Latency INTEGER,"
         "Upload INT64,"
         "Download INT64,"
         "CreatedAt INT64 NOT NULL,"
         "ModifiedAt INT64 NOT NULL);"},
        {"CREATE TABLE IF NOT EXISTS routings("
         "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
         "Name TEXT NOT NULL,"
         "DomainStrategy TEXT,"
         "DomainMatcher TEXT,"
         "Raw TEXT NOT NULL,"
         "CreatedAt INT64 NOT NULL,"
         "ModifiedAt INT64 NOT NULL);"},
        {"CREATE VIRTUAL TABLE IF NOT EXISTS search "
         "USING fts5(ID, Name, GroupID, GroupName, Address);"},
        {"CREATE TRIGGER IF NOT EXISTS search_a_i AFTER INSERT ON nodes BEGIN "
         "INSERT INTO search VALUES (new.ID, new.Name, new.GroupID, "
         "new.GroupName, new.Address); "
         "END;"},
        {"CREATE TRIGGER IF NOT EXISTS search_a_d AFTER DELETE ON nodes BEGIN "
         "DELETE FROM search WHERE ID = old.ID; "
         "END;"},
        {"CREATE TRIGGER IF NOT EXISTS search_a_u AFTER UPDATE ON nodes BEGIN "
         "UPDATE OR REPLACE search SET Name = new.Name,GroupID = "
         "new.GroupID,GroupName = new.GroupName, Address = new.Address WHERE "
         "ID = old.ID; "
         "END;"},
    };

    TransactionWrap(this);
    for (auto &table : tables) {
        if (result = directExec(table); result.type() != QSqlError::NoError)
            break;
    }

    return result;
}

QSqlError DBTools::createDefaultValues() {
    QSqlError result;
    const QList<RuntimeValue> values = {
        RuntimeValue(CURRENT_NODE_ID, 0LL),
        RuntimeValue(CURRENT_GROUP_ID, 0LL),
        RuntimeValue(DEFAULT_NODE_ID, 0LL),
        RuntimeValue(DEFAULT_GROUP_ID, 0LL),
    };

    for (auto &value : values)
        if (result = createRuntimeValue(value);
            result.type() != QSqlError::NoError)
            break;

    return result;
}

QPair<QSqlError, qint64>
DBTools::stepExec(const QString &sql_str, QVariantList *inputCollection,
                  int outputColumns, QList<QVariantList> *outputCollections) {
    QSqlError result;
    QSqlQuery query(m_db);

    do {
        if (!query.prepare(sql_str)) {
            p_logger->error("Failed to prepared: {}",
                            query.lastError().text().toStdString());
            break;
        }

        if (inputCollection != nullptr) {
            for (auto &item : *inputCollection) {
                query.addBindValue(item);

                if (result = query.lastError();
                    result.type() != QSqlError::NoError) {
                    p_logger->error("Failed to bind: {}",
                                    result.text().toStdString());
                    break;
                }
            }
        }

        if (!query.exec()) {
            if (result = query.lastError();
                result.type() != QSqlError::NoError) {
                p_logger->error("Failed to step: {}",
                                result.text().toStdString());
                break;
            }
        }

        if (outputCollections != nullptr) {
            while (query.next()) {
                QVariantList temp;
                for (auto i = 0; i < outputColumns; ++i) {
                    temp.append(query.value(i));
                }
                outputCollections->append(temp);
            }
        }
    } while (false);

    return {result, query.lastInsertId().toLongLong()};
}

QSqlError DBTools::createDefaultGroup() {
    QSqlError result;

    if (const QString name("default_group"); !isItemExists(name)) {
        GroupInfo group = {.name = name};
        result = this->insert(group);
    }

    return result;
}

QSqlError DBTools::createDefaultRouting() {
    QSqlError result;

    if (const QString name("default_routing");
        !isItemExists(name, "routings")) {
        RoutingInfo routing = {
            .name = name,
            .domain_strategy = "AsIs",
            .domain_matcher = "mph",
        };

        result = this->insert(routing);
    }

    return result;
}

QSqlError DBTools::directExec(const QString &sql_str) {
    QSqlQuery query(m_db);
    query.exec(sql_str);
    return query.lastError();
}

QSqlError DBTools::createRuntimeValue(const RuntimeValue &value) {
    QString insert_str("INSERT OR IGNORE INTO runtime"
                       "(Name, Type, Value)"
                       "VALUES(?,?,?)");

    QVariantList input_collection = {value.key, value.type, value.value};

    return stepExec(insert_str, &input_collection).first;
}

std::optional<RuntimeValue> DBTools::readRuntimeValue(const QString &key) {
    QVariantList input_collection = {key};
    QList<QVariantList> output_collections;
    QString select_str("SELECT Type,Value FROM runtime WHERE Name = ?");

    if (auto result =
            stepExec(select_str, &input_collection, 2, &output_collections)
                .first;
        result.type() == QSqlError::NoError && !output_collections.isEmpty()) {

        auto item = output_collections.first();
        return RuntimeValue(key, item.first().toInt(), item.last());
    }

    return {};
}

std::optional<RuntimeValue>
DBTools::readRuntimeValue(const RunTimeValues &key) {
    return readRuntimeValue(magic_enum::enum_name(key).data());
}

QSqlError DBTools::updateRuntimeValue(const RuntimeValue &value) {
    QString update_str("UPDATE runtime SET Type = ?, Value = ? WHERE Name = ?");
    QVariantList input_collection = {value.type, value.value, value.key};

    return stepExec(update_str, &input_collection).first;
}

QSqlError DBTools::deleteRuntimeValue(const QString &key) {
    QString delete_str("DELETE FROM runtime WHERE Name = ?");
    QVariantList input_collection = {key};

    return stepExec(delete_str, &input_collection).first;
}

QSqlError DBTools::deleteRuntimeValue(const RunTimeValues &key) {
    return deleteRuntimeValue(magic_enum::enum_name(key).data());
}

qint64 DBTools::getCurrentNodeID() {
    return readRuntimeValue(RunTimeValues::CURRENT_NODE_ID)
        .value()
        .value.toLongLong();
}

qint64 DBTools::getCurrentGroupID() {
    return readRuntimeValue(RunTimeValues::CURRENT_GROUP_ID)
        .value()
        .value.toLongLong();
}

qint64 DBTools::getDefaultNodeID() {
    return readRuntimeValue(RunTimeValues::DEFAULT_NODE_ID)
        .value()
        .value.toLongLong();
}

qint64 DBTools::getDefaultGroupID() {
    return readRuntimeValue(RunTimeValues::DEFAULT_GROUP_ID)
        .value()
        .value.toLongLong();
}

bool DBTools::isTableExists(const QString &table_name) {
    if (m_db.tables().contains(table_name))
        return true;
    return false;
}

bool DBTools::isItemExists(const QString &group_name,
                           const QString &table_name) {
    QSqlQuery query(m_db);
    auto query_str = QString("SELECT * FROM %1 WHERE Name = ?").arg(table_name);

    if (query.prepare(query_str.toStdString().c_str())) {
        query.addBindValue(group_name);

        if (query.exec())
            if (query.next())
                return true;
    }

    return false;
}

QSqlError DBTools::insert(NodeInfo &node) {
    const QString insert_str(
        "INSERT INTO nodes "
        "(Name, GroupID, GroupName, RoutingID, RoutingName, "
        "Protocol, Address, Port, Password, Raw, URL, Latency, "
        "Upload, Download, CreatedAt, ModifiedAt) "
        "VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

    if (node.created_time.isNull()) {
        node.created_time = QDateTime::currentDateTime();
    }

    if (node.modified_time.isNull()) {
        node.modified_time = QDateTime::currentDateTime();
    }

    QVariantList input_collection = {
        node.name,
        node.group_id,
        node.group_name,
        node.routing_id,
        node.routing_name,
        node.protocol,
        node.address,
        node.port,
        node.password,
        node.raw,
        node.url,
        node.latency,
        node.upload,
        node.download,
        node.created_time.toSecsSinceEpoch(),
        node.modified_time.toSecsSinceEpoch(),
    };

    auto [result, id] = stepExec(insert_str, &input_collection);
    node.id = id;

    return result;
}

QSqlError DBTools::insert(QList<NodeInfo> &nodes) {
    QSqlError result;
    TransactionWrap(this);
    for (auto &node : nodes)
        if (result = insert(node); result.type() != QSqlError::NoError)
            break;
    return result;
}

QSqlError DBTools::update(NodeInfo &node) {
    QSqlError result;
    const QString update_str(
        "UPDATE nodes SET "
        "Name = ?, GroupID = ?, GroupName = ?, RoutingID = ?, RoutingName = ?, "
        "Protocol = ?, Address = ?, Port = ?, Password = ?, "
        "Raw = ?, URL = ?, Latency = ?, Upload = ?, "
        "Download = ?, ModifiedAt = ? "
        "WHERE ID = ?;");

    node.modified_time = QDateTime::currentDateTime();
    QVariantList input_collection = {
        node.name,
        node.group_id,
        node.group_name,
        node.routing_id,
        node.routing_name,
        node.protocol,
        node.address,
        node.port,
        node.password,
        node.raw,
        node.url,
        node.latency,
        node.upload,
        node.download,
        node.modified_time.toSecsSinceEpoch(),
        node.id,
    };

    if (result = stepExec(update_str, &input_collection).first;
        result.type() != QSqlError::NoError) {
        p_logger->error("Failed to update node: {}", node.id);
    }

    return result;
}

QSqlError DBTools::update(QList<NodeInfo> &nodes) {
    QSqlError result;
    TransactionWrap(this);
    for (auto &node : nodes)
        if (result = update(node); result.type() != QSqlError::NoError)
            break;
    return result;
}

QSqlError DBTools::insert(GroupInfo &group) {
    const QString insert_str(
        "INSERT INTO groups "
        "(Name, IsSubscription, Type, Url, CycleTime, CreatedAt, ModifiedAt)"
        "VALUES(?,?,?,?,?,?,?)");

    if (group.created_time.isNull()) {
        group.created_time = QDateTime::currentDateTime();
    }

    if (group.modified_time.isNull()) {
        group.modified_time = QDateTime::currentDateTime();
    }

    QVariantList input_collection = {
        group.name,
        group.is_subscription,
        group.type,
        group.url,
        group.cycle_time,
        group.created_time.toSecsSinceEpoch(),
        group.modified_time.toSecsSinceEpoch(),
    };

    auto [result, id] = stepExec(insert_str, &input_collection);
    group.id = id;

    return result;
}

QSqlError DBTools::update(GroupInfo &group) {
    QSqlError result;
    const QString update_str("UPDATE groups SET "
                             "Name = ?, IsSubscription = ?, Type = ?, "
                             "Url = ?, CycleTime = ?, ModifiedAt = ? "
                             "WHERE ID = ?;");
    group.modified_time = QDateTime::currentDateTime();

    QVariantList input_collection = {
        group.name,       group.is_subscription,
        group.type,       group.url,
        group.cycle_time, group.modified_time.toSecsSinceEpoch(),
        group.id,
    };

    if (result = stepExec(update_str, &input_collection).first;
        result.type() != QSqlError::NoError) {
        p_logger->error("Failed to update group: {}", group.id);
    }

    return result;
}

QSqlError DBTools::update(QList<GroupInfo> &groups) {
    QSqlError result;
    TransactionWrap(this);
    for (auto &group : groups)
        if (result = update(group); result.type() != QSqlError::NoError)
            break;
    return result;
}

QSqlError DBTools::insert(RoutingInfo &routing) {
    const QString insert_str("INSERT INTO routings "
                             "(Name, DomainStrategy, DomainMatcher, "
                             "Raw, CreatedAt, ModifiedAt)"
                             "VALUES(?,?,?,?,?,?)");

    if (routing.created_time.isNull()) {
        routing.created_time = QDateTime::currentDateTime();
    }

    if (routing.modified_time.isNull()) {
        routing.modified_time = QDateTime::currentDateTime();
    }

    QVariantList input_collection = {
        routing.name,
        routing.domain_strategy,
        routing.domain_matcher,
        routing.raw,
        routing.created_time.toSecsSinceEpoch(),
        routing.modified_time.toSecsSinceEpoch(),
    };

    auto [result, id] = stepExec(insert_str, &input_collection);
    routing.id = id;

    return result;
}

QSqlError DBTools::update(RoutingInfo &routing) {
    QSqlError result;
    const QString update_str("UPDATE routings SET "
                             "Name = ?, DomainStrategy = ?, DomainMatcher = ?, "
                             "Raw = ?, ModifiedAt = ? "
                             "WHERE ID = ?;");

    routing.modified_time = QDateTime::currentDateTime();

    QVariantList input_collection = {
        routing.name,
        routing.domain_strategy,
        routing.domain_matcher,
        routing.raw,
        routing.modified_time.toSecsSinceEpoch(),
        routing.id,
    };

    if (result = stepExec(update_str, &input_collection).first;
        result.type() != QSqlError::NoError) {
        p_logger->error("Failed to update routing: {}", routing.id);
    }

    return result;
}

QSqlError DBTools::removeNodeFromID(qint64 id) {
    const QString remove_str("DELETE FROM nodes WHERE ID = ?");
    QVariantList input_collection = {id};

    return stepExec(remove_str, &input_collection).first;
}

QSqlError DBTools::removeGroupFromID(qint64 id, bool keep_group) {
    QSqlError result;
    QVariantList input_collection = {id};

    if (!keep_group) {
        const QString remove_str("DELETE FROM groups WHERE ID = ?");

        if (result = stepExec(remove_str, &input_collection).first;
            result.type() != QSqlError::NoError) {
            p_logger->error("Failed to remove group {}: {}", id,
                            result.text().toStdString());

            return result;
        }
    }

    const QString remove_str("DELETE FROM nodes WHERE GroupID = ?");
    if (result = stepExec(remove_str, &input_collection).first;
        result.type() != QSqlError::NoError) {
        p_logger->error("Failed to remove nodes: {}",
                        result.text().toStdString());
    }

    return result;
}

qsizetype DBTools::getSizeFromGroupID(qint64 group_id) {
    QList<QVariantList> collections;
    QVariantList input_collection = {group_id};
    const QString select_str("SELECT ID FROM nodes WHERE GroupID = ?");

    if (auto result =
            stepExec(select_str, &input_collection, 1, &collections).first;
        result.type() != QSqlError::NoError) {
        p_logger->error("Failed to list all nodes");
    } else {
        return collections.size();
    }

    return 0;
}

std::optional<GroupInfo> DBTools::getGroupFromID(qint64 group_id) {
    for (auto &group : m_groups) {
        if (group_id == group.id) {
            return group;
        }
    }

    return {};
}

QSqlError DBTools::reloadAllGroupsInfo() {
    QSqlError result;
    GroupInfo temp;
    const QString select_str("SELECT * FROM groups");
    QList<QVariantList> collections;

    if (result = stepExec(select_str, nullptr, 8, &collections).first;
        result.type() != QSqlError::NoError) {
        p_logger->error("Failed to list all groups");
        return result;
    }

    m_groups.clear();

    for (auto &item : collections) {
        GroupInfo group = {
            .id = item.at(0).toLongLong(),
            .name = item.at(1).toString(),
            .is_subscription = item.at(2).toBool(),
            .type =
                magic_enum::enum_value<SubscriptionType>(item.at(3).toInt()),
            .url = item.at(4).toString(),
            .cycle_time = item.at(5).toInt(),
            .created_time =
                QDateTime::fromSecsSinceEpoch(item.at(6).toLongLong()),
            .modified_time =
                QDateTime::fromSecsSinceEpoch(item.at(7).toLongLong()),
        };

        group.items = getSizeFromGroupID(group.id);

        m_groups.emplace_back(group);
    }

    return result;
}

QList<GroupInfo> DBTools::getAllGroupsInfo() { return m_groups; }

QList<NodeInfo> DBTools::listAllNodesFromGroupID(qint64 group_id) {
    QList<NodeInfo> nodes;
    QList<QVariantList> collections;
    QVariantList input_collection = {group_id};
    const QString select_str("SELECT * FROM nodes WHERE GroupID = ?");

    if (auto result =
            stepExec(select_str, &input_collection, 17, &collections).first;
        result.type() != QSqlError::NoError) {

        p_logger->error("Failed to list all nodes");

        return nodes;
    }

    for (auto &item : collections) {
        NodeInfo node = {
            .id = item.at(0).toLongLong(),
            .name = item.at(1).toString(),
            .group_id = item.at(2).toLongLong(),
            .group_name = item.at(3).toString(),
            .routing_id = item.at(4).toLongLong(),
            .routing_name = item.at(5).toString(),
            .protocol = magic_enum::enum_value<EntryType>(item.at(6).toInt()),
            .address = item.at(7).toString(),
            .port = item.at(8).toUInt(),
            .password = item.at(9).toString(),
            .raw = item.at(10).toString(),
            .url = item.at(11).toString(),
            .latency = item.at(12).toLongLong(),
            .upload = item.at(13).toLongLong(),
            .download = item.at(14).toLongLong(),
            .created_time =
                QDateTime::fromSecsSinceEpoch(item.at(15).toLongLong()),
            .modified_time =
                QDateTime::fromSecsSinceEpoch(item.at(16).toLongLong()),
        };

        nodes.emplace_back(node);
    }

    return nodes;
}

QMap<qint64, QList<qint64>> DBTools::search(const QString &value) {
    QList<QVariantList> collections;
    QMap<qint64, QList<qint64>> search_results;
    QVariantList input_collection = {value.toHtmlEscaped().append("*")};
    const QString search_str(
        "SELECT GroupID,GROUP_CONCAT(ID) FROM search WHERE "
        "search = ? GROUP BY GroupID;");

    if (auto result =
            stepExec(search_str, &input_collection, 2, &collections).first;
        result.type() != QSqlError::NoError) {
        p_logger->error("Failed to list all nodes");

        return search_results;
    }

    for (auto &item : collections) {
        QList<qint64> nodes_id;

        for (auto &node_id : item.at(1).toString().split(","))
            nodes_id.append(node_id.toLongLong());

        search_results.insert(item.at(0).toLongLong(), nodes_id);
    }

    return search_results;
}

void DBTools::close() {
    if (m_db.isOpen()) {
        m_db.close();
        emit destroy();
    }
}

RuntimeValue::RuntimeValue(const QString &key, const QVariant &value)
    : key(key), type(value.typeId()), value(value) {}

RuntimeValue::RuntimeValue(const RunTimeValues key, const QVariant &value)
    : key(magic_enum::enum_name(key).data()), type(value.typeId()),
      value(value) {}

RuntimeValue::RuntimeValue(const QString &key, int type, const QVariant &value)
    : key(key), type(type), value(value) {}

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
