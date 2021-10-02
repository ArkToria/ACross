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
  readRuntimeValue("CURRENT_GROUP_ID");
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
DBTools::stepExec(const QVariantList& collection,
                  const QString& sql_str,
                  int columns,
                  QVector<QVariantList>* collections)
{
  QSqlError result;
  QSqlQuery query(m_db);

  do {
    if (!query.prepare(sql_str)) {
      p_logger->error("Failed to prepared: {}",
                      query.lastError().text().toStdString());
      break;
    }

    for (auto& item : collection) {
      query.addBindValue(item);

      if (result = query.lastError(); result.type() != QSqlError::NoError) {
        p_logger->error("Failed to bind: {}", result.text().toStdString());
        break;
      }
    }

    if (!query.exec()) {
      if (result = query.lastError(); result.type() != QSqlError::NoError) {
        p_logger->error("Failed to step: {}", result.text().toStdString());
        break;
      }
    }

    if (collections != nullptr) {
      while (query.next()) {
        QVariantList temp;
        for (auto i = 0; i < columns; ++i) {
          temp.append(query.value(i));
        }
        collections->append(temp);
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

  if (!isGroupExists(default_group)) {
    GroupInfo group = { .name = default_group };
    result = this->insert(group);
  }

  return result;
}

QSqlError
DBTools::createTable(const QString& create_str)
{
  QSqlQuery query(m_db);
  query.exec(create_str);
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
  return createTable(create_groups_str);
}

QSqlError
DBTools::createNodesTable(const QString& group_name)
{
  if (isTableExists(group_name)) {
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
                               .arg(group_name.toHtmlEscaped());

  return createTable(create_nodes_str);
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
  return createTable(create_runtime_str);
}

QSqlError
DBTools::createRuntimeValue(const QString& key, const QString& value)
{
  QString insert_str("INSERT INTO runtime"
                     "(Key, Value)"
                     "VALUES(?,?)");

  QVariantList collection = { key, value };

  return stepExec(collection, insert_str);
}

QString
DBTools::readRuntimeValue(const QString& key)
{
  QString select_str("SELECT Value FROM runtime WHERE Key = ?");

  QVector<QVariantList> collections;
  if (auto result = stepExec({ key }, select_str, 1, &collections);
      result.type() == QSqlError::NoError) {
    return collections.first().first().toString();
  }
  return QString();
}

QSqlError
DBTools::updateRuntimeValue(const QString& key, const QString& value)
{
  return QSqlError();
}

QSqlError
DBTools::deleteRuntimeValue(const QString& key)
{
  return QSqlError();
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
      .arg(node.group.toHtmlEscaped());

  if (node.created_time.isNull()) {
    node.created_time = QDateTime::currentDateTime();
  }

  if (node.modified_time.isNull()) {
    node.modified_time = QDateTime::currentDateTime();
  }

  qint64 group_id = node.group_id;
  QVariantList collection = {
    node.name,
    node.group,
    group_id,
    node.protocol,
    node.address,
    node.port,
    node.password,
    node.raw,
    node.url,
    node.created_time.toSecsSinceEpoch(),
    node.modified_time.toSecsSinceEpoch(),
  };

  if (result = stepExec(collection, insert_str);
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

  QVariantList collections = {
    group.name,       group.isSubscription, group.type,          group.url,
    group.cycle_time, group.created_time,   group.modified_time,
  };

  if (result = stepExec(collections, insert_str);
      result.type() == QSqlError::NoError) {
    group.id = getLastID();
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

  qint64 group_id = group.id;
  QVariantList collection = {
    group.name,       group.isSubscription, group.type, group.url,
    group.cycle_time, group.modified_time,  group_id,
  };

  return stepExec(collection, update_str);
}

QSqlError
DBTools::removeItemFromID(const QString& group_name, int64_t id)
{
  QString remove_str = QString("DELETE FROM '%1' WHERE id = ?").arg(group_name);
  qint64 node_id = id;

  return stepExec({ node_id }, remove_str);
}

QSqlError
DBTools::removeGroupFromName(const QString& group_name, bool keep_group)
{
  QString remove_str("DELETE FROM groups WHERE name = ?");

  if (!keep_group) {
    if (auto result = stepExec({ group_name }, remove_str);
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
  QSqlQuery query(m_db);
  query.prepare("DROP TABLE ?;");
  query.addBindValue(table_name);
  query.exec();
  return query.lastError();
}

std::vector<GroupInfo>
DBTools::listAllGroupsInfo()
{
  GroupInfo temp;
  qint64 group_id = -1;
  QString select_str("SELECT * FROM groups");
  std::vector<GroupInfo> groups;
  QVector<QVariantList> collections;

  if (auto result = stepExec(QVariantList(), select_str, 8, &collections);
      result.type() != QSqlError::NoError) {
    p_logger->error("Failed to list all groups");
  } else {
    for (auto& item : collections) {
      GroupInfo group;
      group.id = item.at(0).toLongLong();
      group.name = item.at(1).toString();
      group.isSubscription = item.at(2).toBool();
      group.type = magic_enum::enum_value<SubscriptionType>(item.at(3).toInt());
      group.url = item.at(4).toString();
      group.cycle_time = item.at(5).toInt();
      group.created_time =
        QDateTime::fromSecsSinceEpoch(item.at(6).toLongLong());
      group.modified_time =
        QDateTime::fromSecsSinceEpoch(item.at(7).toLongLong());
      group.items = listAllNodesInfo(group.name).size();

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

std::vector<NodeInfo>
DBTools::listAllNodesInfo(const QString& group_name)
{
  NodeInfo temp;
  qint64 node_id;
  std::vector<NodeInfo> nodes;
  QVector<QVariantList> collections;
  QString select_str =
    QString("SELECT * FROM '%1'").arg(group_name.toHtmlEscaped());

  if (auto result = stepExec(QVariantList(), select_str, 12, &collections);
      result.type() != QSqlError::NoError) {
    p_logger->error("Failed to list all nodes");
  } else {
    for (auto& item : collections) {
      NodeInfo node;

      node.id = item.at(0).toLongLong();
      node.name = item.at(1).toString();
      node.group = item.at(2).toString();
      node.group_id = item.at(3).toInt();
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


std::map<int, NodesInfo>
DBTools::listAllNodes()
{
  std::map<int, NodesInfo> all_nodes;
  for (auto& item : m_all_groups_info) {
    auto nodes = listAllNodesInfo(item.name);

    // group id, { current node id, current index, nodes }
    all_nodes.insert({ item.id, { 0, 0, nodes } });
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
