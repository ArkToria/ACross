#include "dbtools.h"

using namespace across;
using namespace across::setting;
using namespace across::utils;

DBTools::DBTools(QObject* parent) {}

DBTools::~DBTools()
{
  close();
}

void
DBTools::init(QSharedPointer<LogView> log_view,
              QSharedPointer<ConfigTools> config)
{
  p_logger = std::make_shared<LogTools>(log_view, "database");
  p_config = config;

  reload();
}

void
DBTools::reload()
{
  close();

  do {
    if (p_config == nullptr)
      break;

    auto db_path = p_config->dbPath();
    if (db_path.isEmpty()) {
      p_logger->error("Failed to load database on path");
      break;
    } else {
      p_logger->info("Open database on path: {}", db_path.toStdString());
    }

    QSqlError result;
    if (QSqlDatabase::isDriverAvailable("QSQLITE")) {
      m_db = QSqlDatabase::addDatabase("QSQLITE");
      m_db.setDatabaseName(db_path);
      if (!m_db.open()) {
        p_logger->error("Failed to open database");
        break;
      } else {
        p_logger->info("Opened successfully");
      }
    } else {
      p_logger->error("Database driver is unavailable: {}",
                      m_db.driverName().toStdString());
      break;
    }

    if (result = createGroupsTable(); result.type() != QSqlError::NoError) {
      p_logger->error("Failed to create groups table: {}",
                      result.text().toStdString());
      break;
    }

    if (result = createRuntimeTable(); result.type() != QSqlError::NoError) {
      p_logger->error("Failed to create runtime table: {}",
                      result.text().toStdString());
      break;
    } else {
      createRuntimeValue("CURRENT_GROUP_ID", "0");
      createRuntimeValue("CURRENT_NODE_ID", "0");
      createRuntimeValue("DEFAULT_GROUP_ID", "0");
      createRuntimeValue("DEFAULT_NODE_ID", "0");
    }

    if (result = createSearchTable(); result.type() != QSqlError::NoError) {
      p_logger->error("Failed to create virtual search table: {}",
                      result.text().toStdString());
    }

    if (result = createNodesTable("default_group");
        result.type() != QSqlError::NoError) {
      p_logger->error("Failed to create default_group table: {}",
                      result.text().toStdString());
      break;
    }

    if (result = createDefaultGroup(); result.type() != QSqlError::NoError) {
      p_logger->error("Failed to insert default_group to groups table: {}",
                      result.text().toStdString());
      break;
    }

  } while (false);
}

QSqlError
DBTools::stepExec(const QString& sql_str,
                  QVariantList* inputCollection,
                  int outputColumns,
                  QVector<QVariantList>* outputCollections)
{
  QSqlError result;
  QSqlQuery query(m_db);

  do {
    if (!query.prepare(sql_str)) {
      p_logger->error("Failed to prepared: {}",
                      query.lastError().text().toStdString());
      break;
    }

    if (inputCollection != nullptr) {
      for (auto& item : *inputCollection) {
        query.addBindValue(item);

        if (result = query.lastError(); result.type() != QSqlError::NoError) {
          p_logger->error("Failed to bind: {}", result.text().toStdString());
          break;
        }
      }
    }

    if (!query.exec()) {
      if (result = query.lastError(); result.type() != QSqlError::NoError) {
        p_logger->error("Failed to step: {}", result.text().toStdString());
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

  return result;
}

QSqlError
DBTools::createDefaultGroup()
{
  QSqlError result;
  const QString default_group("default_group");

  if (!isGroupExists(PREFIX + default_group)) {
    GroupInfo group = { .name = default_group };
    result = this->insert(group);
  }

  return result;
}

QSqlError
DBTools::directExec(const QString& sql_str)
{
  QSqlQuery query(m_db);
  query.exec(sql_str);
  return query.lastError();
}

QSqlError
DBTools::createGroupsTable()
{
  if (isTableExists("groups")) {
    return QSqlError();
  }

  QString create_groups_str("CREATE TABLE IF NOT EXISTS groups("
                            "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "Name TEXT UNIQUE,"
                            "IsSubscription BOOLEAN,"
                            "Type INTEGER,"
                            "Url TEXT,"
                            "CycleTime INTEGER,"
                            "CreatedAt INT64 NOT NULL,"
                            "ModifiedAt INT64 NOT NULL);");
  return directExec(create_groups_str);
}

QSqlError
DBTools::createNodesTable(const QString& group_name)
{
  if (isTableExists(PREFIX + group_name)) {
    return QSqlError();
  }

  QString create_nodes_str = QString("CREATE TABLE IF NOT EXISTS '%1'("
                                     "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                     "Name TEXT NOT NULL,"
                                     "GroupName TEXT NOT NULL,"
                                     "GroupID INTEGER NOT NULL,"
                                     "Protocol INTEGER NOT NULL,"
                                     "Address CHAR(255) NOT NULL,"
                                     "Port INTEGER NOT NULL,"
                                     "Password TEXT NOT NULL,"
                                     "Raw TEXT NOT NULL,"
                                     "URL TEXT,"
                                     "CreatedAt INT64 NOT NULL,"
                                     "ModifiedAt INT64 NOT NULL) ;")
                               .arg(PREFIX + group_name.toHtmlEscaped());

  return directExec(create_nodes_str);
}

QSqlError
DBTools::createRuntimeTable()
{
  if (isTableExists("runtime")) {
    return QSqlError();
  }

  QString create_runtime_str = { "CREATE TABLE IF NOT EXISTS runtime ("
                                 "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "Key TEXT UNIQUE NOT NULL,"
                                 "Value TEXT);" };
  return directExec(create_runtime_str);
}

QSqlError
DBTools::createSearchTable()
{
  if (isTableExists("search")) {
    return QSqlError();
  }

  QString create_runtime_str = { "CREATE VIRTUAL TABLE IF NOT EXISTS search ("
                                 "ID INTEGER PRIMARY KEY NOT NULL,"
                                 "GroupID TEXT NOT NULL,"
                                 "Name TEXT,"
                                 "GroupName TEXT NOT NULL,"
                                 "Address TEXT);" };

  return directExec(create_runtime_str);
}

QSqlError
DBTools::createRuntimeValue(const QString& key, const QString& value)
{
  QString insert_str("INSERT OR IGNORE INTO runtime"
                     "(Key, Value)"
                     "VALUES(?,?)");

  QVariantList input_collection = { key, value };

  return stepExec(insert_str, &input_collection);
}

QString
DBTools::readRuntimeValue(const QString& key)
{
  QString select_str("SELECT Value FROM runtime WHERE Key = ?");

  QVector<QVariantList> output_collections;
  QVariantList input_collection = { key };
  if (auto result =
        stepExec(select_str, &input_collection, 1, &output_collections);
      result.type() == QSqlError::NoError) {
    return output_collections.first().first().toString();
  }
  return QString();
}

QSqlError
DBTools::updateRuntimeValue(const QString& key, const QString& value)
{
  QString update_str("UPDATE runtime SET Value = ? WHERE Key = ?");
  QVariantList input_collection = { value, key };

  return stepExec(update_str, &input_collection);
}

QSqlError
DBTools::deleteRuntimeValue(const QString& key)
{
  QString delete_str("DELETE FROM runtime WHERE Key = ?");
  QVariantList input_collection = { key };

  return stepExec(delete_str, &input_collection);
}

QSqlError
DBTools::createTrigger(const QString& group_name)
{
  QVector<QString> triggers;
  QSqlError result;

  triggers.append(
    QString("CREATE TRIGGER %1 AFTER INSERT ON %2"
            "FROM EACH ROW"
            "BEGIN"
            "INSERT INTO search VALUES (new.ID, new.GroupID, new.Name, "
            "new.GroupName, new.Address);"
            "END;")
      .arg("trigger_after_insert_" + PREFIX + group_name.toHtmlEscaped(),
           PREFIX + group_name.toHtmlEscaped()));

  triggers.append(
    QString("CREATE TRIGGER %1 AFTER DELETE ON %2"
            "FROM EACH ROW"
            "BEGIN"
            "DROP FROM search WHERE ID = old.ID;"
            "END;")
      .arg("trigger_after_delete_" + PREFIX + group_name.toHtmlEscaped(),
           PREFIX + group_name.toHtmlEscaped()));

  triggers.append(
    QString("CREATE TRIGGER %1 AFTER UPDATE ON %2"
            "FROM EACH ROW"
            "BEGIN"
            "UPDATE OR REPLACE search SET GroupID = old.GroupID, "
            "Name = old.Name, GroupName = old.GroupName, Address = old.Address "
            "WHERE ID = old.ID;"
            "END;")
      .arg("trigger_after_update_" + PREFIX + group_name.toHtmlEscaped(),
           PREFIX + group_name.toHtmlEscaped()));

  for (auto& trigger_str : triggers) {
    if (result = directExec(trigger_str); result.type() != QSqlError::NoError) {
      return result;
    }
  }

  return result;
}

int64_t
DBTools::getCurrentNodeID()
{
  return readRuntimeValue("CURRENT_NODE_ID").toLongLong();
}

int64_t
DBTools::getCurrentGroupID()
{
  return readRuntimeValue("CURRENT_GROUP_ID").toLongLong();
}

int64_t
DBTools::getDefaultNodeID()
{
  return readRuntimeValue("DEFAULT_NOED_ID").toLongLong();
}

int64_t
DBTools::getDefaultGroupID()
{
  return readRuntimeValue("DEFAULT_GROUP_ID").toLongLong();
}

bool
DBTools::isTableExists(const QString& table_name)
{
  for (auto& table : m_db.tables()) {
    if (table == table_name)
      return true;
  }

  return false;
}

bool
DBTools::isGroupExists(const QString& group_name)
{
  QSqlQuery query(m_db);

  if (query.prepare("SELECT * FROM groups WHERE Name = ?")) {
    query.addBindValue(group_name);

    if (query.exec()) {
      if (query.next()) {
        return true;
      }
    }
  }

  return false;
}

QSqlError
DBTools::insert(NodeInfo& node)
{
  QSqlError result;
  QString insert_str =
    QString("INSERT INTO \"%1\" "
            "(Name, GroupName, GroupID, Protocol, Address, Port, "
            "Password, Raw, URL, CreatedAt, ModifiedAt) "
            "VALUES(?,?,?,?,?,?,?,?,?,?,?)")
      .arg(PREFIX + node.group.toHtmlEscaped());

  if (node.created_time.isNull()) {
    node.created_time = QDateTime::currentDateTime();
  }

  if (node.modified_time.isNull()) {
    node.modified_time = QDateTime::currentDateTime();
  }

  QVariantList input_collection = {
    node.name,
    node.group,
    node.group_id,
    node.protocol,
    node.address,
    node.port,
    node.password,
    node.raw,
    node.url,
    node.created_time.toSecsSinceEpoch(),
    node.modified_time.toSecsSinceEpoch(),
  };

  if (result = stepExec(insert_str, &input_collection);
      result.type() == QSqlError::NoError)
    node.id = getLastID();

  return result;
}

QSqlError
DBTools::insert(GroupInfo& group)
{
  QSqlError result;

  QString insert_str(
    "INSERT INTO groups"
    "(Name, IsSubscription, Type, Url, CycleTime, CreatedAt, ModifiedAt)"
    "VALUES(?,?,?,?,?,?,?)");

  if (group.created_time.isNull()) {
    group.created_time = QDateTime::currentDateTime();
  }

  if (group.modified_time.isNull()) {
    group.modified_time = QDateTime::currentDateTime();
  }

  QVariantList input_collection = {
    PREFIX + group.name,
    group.isSubscription,
    group.type,
    group.url,
    group.cycle_time,
    group.created_time.toSecsSinceEpoch(),
    group.modified_time.toSecsSinceEpoch(),
  };

  if (result = stepExec(insert_str, &input_collection);
      result.type() == QSqlError::NoError) {
    group.id = getLastID();

    result = createTrigger(group.name);
  }

  return result;
}

int64_t
DBTools::getLastID()
{
  QSqlQuery query(m_db);

  if (auto id = query.lastInsertId(); id.typeId() == QMetaType::LongLong) {
    return id.toLongLong();
  }

  return 0;
}

QSqlError
DBTools::update(GroupInfo& group)
{
  QString update_str("UPDATE groups SET "
                     "Name = ?, IsSubscription = ?, Type = ?, "
                     "Url = ?, CycleTime = ?, ModifiedAt = ? "
                     "WHERE ID = ?;");

  QVariantList input_collection = {
    PREFIX + group.name,
    group.isSubscription,
    group.type,
    group.url,
    group.cycle_time,
    group.modified_time.toSecsSinceEpoch(),
    group.id,
  };

  return stepExec(update_str, &input_collection);
}

QSqlError
DBTools::removeItemFromID(const QString& group_name, qint64 id)
{
  QString remove_str = QString("DELETE FROM '%1' WHERE id = ?").arg(group_name);
  QVariantList input_collection = { id };

  return stepExec(remove_str, &input_collection);
}

QSqlError
DBTools::removeGroupFromName(const QString& group_name, bool keep_group)
{
  QString remove_str("DELETE FROM groups WHERE name = ?");
  QVariantList input_collection = { group_name };

  if (!keep_group) {
    if (auto result = stepExec(remove_str, &input_collection);
        result.type() != QSqlError::NoError) {
      p_logger->error("Failed to remove {}", group_name.toStdString());

      return result;
    }
  }

  return dropTable(group_name);
}

QSqlError
DBTools::dropTable(const QString& table_name)
{
  QString drop_str = QString("DROP TABLE \"%1\"").arg(PREFIX + table_name);

  return stepExec(drop_str);
}

std::vector<GroupInfo>
DBTools::listAllGroupsInfo()
{
  GroupInfo temp;
  QString select_str("SELECT * FROM groups");
  std::vector<GroupInfo> groups;
  QVector<QVariantList> collections;

  if (auto result = stepExec(select_str, nullptr, 8, &collections);
      result.type() != QSqlError::NoError) {
    p_logger->error("Failed to list all groups");
  } else {
    for (auto& item : collections) {
      GroupInfo group;
      group.id = item.at(0).toLongLong();
      group.name = item.at(1).toString().remove(PREFIX);
      group.isSubscription = item.at(2).toBool();
      group.type = magic_enum::enum_value<SubscriptionType>(item.at(3).toInt());
      group.url = item.at(4).toString();
      group.cycle_time = item.at(5).toInt();
      group.created_time =
        QDateTime::fromSecsSinceEpoch(item.at(6).toLongLong());
      group.modified_time =
        QDateTime::fromSecsSinceEpoch(item.at(7).toLongLong());
      group.items = listAllNodesFromGroup(group.name).size();

      if (group.id > 0)
        groups.emplace_back(group);
    }
  }

  m_all_groups_info = groups;
  return groups;
}

std::vector<GroupInfo>
DBTools::getAllGroupsInfo()
{
  return m_all_groups_info;
}

QVector<NodeInfo>
DBTools::listAllNodesFromGroup(const QString& group_name)
{
  NodeInfo temp;
  QVector<NodeInfo> nodes;
  QVector<QVariantList> collections;
  QString select_str =
    QString("SELECT * FROM \"%1\"").arg(PREFIX + group_name.toHtmlEscaped());

  if (auto result = stepExec(select_str, nullptr, 12, &collections);
      result.type() != QSqlError::NoError) {
    p_logger->error("Failed to list all nodes");
  } else {
    for (auto& item : collections) {
      NodeInfo node;

      node.id = item.at(0).toLongLong();
      node.name = item.at(1).toString();
      node.group = item.at(2).toString();
      node.group_id = item.at(3).toLongLong();
      node.protocol = magic_enum::enum_value<EntryType>(item.at(4).toInt());
      node.address = item.at(5).toString();
      node.port = item.at(6).toInt();
      node.password = item.at(7).toString();
      node.raw = item.at(8).toString();
      node.url = item.at(9).toString();
      node.created_time =
        QDateTime::fromSecsSinceEpoch(item.at(10).toLongLong());
      node.modified_time =
        QDateTime::fromSecsSinceEpoch(item.at(11).toLongLong());

      nodes.emplace_back(node);
    }
  }

  return nodes;
}

QMap<qint64, QVector<NodeInfo>>
DBTools::listAllNodes()
{
  QMap<qint64, QVector<NodeInfo>> all_nodes;
  for (auto& item : m_all_groups_info) {
    auto nodes = listAllNodesFromGroup(item.name);

    // group id, { current node id, current index, nodes }
    all_nodes.insert(item.id, nodes);
  }
  return all_nodes;
}

void
DBTools::close()
{
  if (m_db.isOpen()) {
    m_db.close();
    emit destroy();
  }
}
