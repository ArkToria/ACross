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

    auto db_path = p_config->dbPath().toStdString();
    if (db_path.empty()) {
      p_logger->error("Failed to load database on path");
      break;
    } else {
      p_logger->info("Open database on path: {}", db_path);
    }

    auto result = SQLITE_OK;

    if (result = sqlite3_open(db_path.c_str(), &m_db); result != SQLITE_OK) {
      p_logger->error("SQL open error code: {}", result);
      break;
    } else {
      p_logger->info("SQL opened successfully");
    }

    if (result = setPragma(); result != SQLITE_OK)
      break;

    if (result = createGroupsTable(); result != SQLITE_OK)
      break;

    if (result = createRuntimeTable(); result != SQLITE_OK)
      break;

    if (result = createNodesTable("default_group"); result != SQLITE_OK)
      break;

    if (result = createDefaultGroup(); result != SQLITE_OK)
      break;

  } while (false);
}

int
DBTools::stepExec(const QVariantList& collection,
                  const QString& sql_str,
                  QVector<QVariantList>* collections)
{
  int result = SQLITE_OK;
  sqlite3_stmt* stmt;

  do {
    auto step_str = sql_str.toStdString();
    result = sqlite3_prepare_v2(m_db, step_str.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
      p_logger->error("SQL prepare error code: {}", result);
      break;
    }

    if (collections == nullptr) {
      for (auto i = 0; i < collection.size(); ++i) {
        auto item = collection.at(i);
        auto index = i + 1;

        switch (item.typeId()) {
          case QMetaType::UInt:
          case QMetaType::Bool:
          case QMetaType::Int:
            result = sqlite3_bind_int(stmt, index, item.toInt());
            break;
          case QMetaType::LongLong:
            result = sqlite3_bind_int64(stmt, index, item.toLongLong());
            break;
          case QMetaType::QDateTime:
            result = sqlite3_bind_int64(
              stmt, index, item.toDateTime().toSecsSinceEpoch());
            break;
          case QMetaType::QString:
            result = sqlite3_bind_text(stmt,
                                       index,
                                       item.toString().toStdString().c_str(),
                                       -1,
                                       SQLITE_TRANSIENT);
            break;
          default:
            p_logger->error("Unknown value type: {}", item.typeId());
            break;
        }

        if (result != SQLITE_OK) {
          p_logger->error("SQL bind error code: {}", result);
          break;
        }
      }
    }

    sqlite3_mutex_enter(sqlite3_db_mutex(m_db));
    if (collections == nullptr) {
      result = sqlite3_step(stmt);
    } else {
      auto temp = collection;

      while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        for (auto i = 0; i < temp.size(); ++i) {
          auto& item = temp[i];

          switch (item.typeId()) {
            case QMetaType::Bool:
            case QMetaType::Int:
              item = sqlite3_column_int(stmt, i);
              break;
            case QMetaType::LongLong:
              item = sqlite3_column_int64(stmt, i);
              break;
            case QMetaType::QDateTime:
              item =
                QDateTime().fromSecsSinceEpoch(sqlite3_column_int64(stmt, i));
              break;
            case QMetaType::QString:
              item =
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            default:
              p_logger->error("Unknown value type: {}", item.typeId());
              break;
          }
        }
        collections->append(temp);
      }
    }

    sqlite3_mutex_leave(sqlite3_db_mutex(m_db));
    if (result != SQLITE_DONE) {
      p_logger->error("SQL group step error code: {}", result);
    }

    result = sqlite3_reset(stmt);
    if (result != SQLITE_OK) {
      p_logger->error("SQL stmt reset error code: {}", result);
    }

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
      p_logger->error("SQL stmt finalize error code: {}", result);
    }
  } while (false);

  return result;
}

int
DBTools::setPragma()
{
  char* err_msg;
  auto result = SQLITE_OK;
  std::string init_str{ "PRAGMA encoding='UTF-8';"
                        "PRAGMA synchronous=OFF;"
                        "PRAGMA count_changes=OFF;"
                        "PRAGMA journal_mode=MEMORY;"
                        "PRAGMA temp_store=MEMORY;"
                        "PRAGMA sql_self_updates=1" };

  sqlite3_mutex_enter(sqlite3_db_mutex(m_db));
  result = sqlite3_exec(m_db, init_str.c_str(), NULL, NULL, &err_msg);
  sqlite3_mutex_leave(sqlite3_db_mutex(m_db));
  if (result != SQLITE_OK) {
    p_logger->error("SQL initialization error code: {}", err_msg);
    sqlite3_free(err_msg);
  }

  return result;
}

int
DBTools::createDefaultGroup()
{
  int result = SQLITE_OK;
  const QString default_group_str("default_group");

  if (!isGroupExists(default_group_str)) {
    GroupInfo default_group{ .name = default_group_str };
    result = this->insert(default_group);
  }

  return result;
}

int
DBTools::createTable(const QString& create_str)
{
  char* err_msg;
  auto result = SQLITE_OK;

  sqlite3_mutex_enter(sqlite3_db_mutex(m_db));
  result =
    sqlite3_exec(m_db, create_str.toStdString().c_str(), nullptr, 0, &err_msg);
  sqlite3_mutex_leave(sqlite3_db_mutex(m_db));

  if (result != SQLITE_OK) {
    p_logger->error("SQL create table error: {}", err_msg);
    sqlite3_free(err_msg);
  }
  return result;
}

int
DBTools::createGroupsTable()
{
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

int
DBTools::createNodesTable(const QString& group_name)
{
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
                                     "ModifiedAt INT64 NOT NULL);")
                               .arg(group_name);
  return createTable(create_nodes_str);
}

int
DBTools::createRuntimeTable()
{
  QString create_runtime_str = { "CREATE TABLE IF NOT EXISTS runtime("
                                 "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "Key TEXT UNIQUE NOT NULL,"
                                 "Value TEXT);" };
  return createTable(create_runtime_str);
}

int
DBTools::createRuntimeValue(const QString& key, const QString& value)
{
  auto result = SQLITE_OK;
  QString insert_str("INSERT INTO runtime"
                     "(Key, Value)"
                     "VALUES(?,?)");

  QVariantList collections = { key, value };

  result = stepExec(collections, insert_str);
  return result;
}

QString
DBTools::readRuntimeValue(const QString& key)
{}

int
DBTools::updateRuntimeValue(const QString& key, const QString& value)
{}

int
DBTools::deleteRuntimeValue(const QString& key)
{}

bool
DBTools::isTableExists(const QStringList& table_names)
{
  bool result = true;
  int err = SQLITE_OK;
  std::string err_name;
  sqlite3_stmt* stmt = nullptr;

  for (auto& name : table_names) {
    std::string result_name;
    QString check_str =
      QString("SELECT Name FROM sqlite_master WHERE Type = 'table' AND "
              "Name = '%1';")
        .arg(name);

    err = sqlite3_prepare_v2(
      m_db, check_str.toStdString().c_str(), -1, &stmt, nullptr);
    if (err != SQLITE_OK) {
      p_logger->error("SQL prepare error code: {}", err);
      break;
    }

    while ((err = sqlite3_step(stmt)) == SQLITE_ROW) {
      result_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    if (err != SQLITE_DONE) {
      p_logger->error("SQL table exists step error code: {}", err);
      break;
    }

    err = sqlite3_reset(stmt);
    if (err != SQLITE_OK) {
      p_logger->error("SQL stmt reset error code: {}", err);
      break;
    }

    if (result_name != name.toStdString()) {
      p_logger->warn("SQL check table `{}` doesn't exists", name.toStdString());
      result = false;
      break;
    }
  }

  if (stmt != nullptr) {
    err = sqlite3_finalize(stmt);
    if (err != SQLITE_OK) {
      p_logger->error("SQL stmt finalize error code: {}", err);
    }
  }

  if (err != SQLITE_OK || !err_name.empty()) {
    result = false;
  }

  return result;
}

bool
DBTools::isGroupExists(const QString& group_name)
{
  int result = SQLITE_OK;
  bool is_exists = false;
  sqlite3_stmt* stmt = nullptr;
  QString check_str("SELECT Name FROM groups WHERE Name = ?;");

  do {
    result = sqlite3_prepare_v2(
      m_db, check_str.toStdString().c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
      p_logger->error("SQL prepare error code: {}", result);
      break;
    }

    result = sqlite3_bind_text(
      stmt, 1, group_name.toStdString().c_str(), -1, SQLITE_TRANSIENT);
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind error code: {}", result);
      break;
    }

    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
      is_exists = true;
      break;
    }
  } while (false);

  if (stmt != nullptr) {
    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
      p_logger->error("SQL stmt finalize error code: {}", result);
    }
  }

  return is_exists;
}

int
DBTools::insert(NodeInfo& node)
{
  auto result = SQLITE_OK;
  QString insert_str =
    QString("INSERT INTO \"%1\" "
            "(Name, GroupName, GroupID, Protocol, Address, Port, "
            "Password, Raw, URL, CreatedAt, ModifiedAt) "
            "VALUES(?,?,?,?,?,?,?,?,?,?,?)")
      .arg(node.group);

  if (node.created_time.isNull()) {
    node.created_time = QDateTime::currentDateTime();
  }

  if (node.modified_time.isNull()) {
    node.modified_time = QDateTime::currentDateTime();
  }

  qint64 group_id = node.group_id;
  QVariantList collections = {
    node.name,    node.group,        group_id,           node.protocol,
    node.address, node.port,         node.password,      node.raw,
    node.url,     node.created_time, node.modified_time,
  };

  if (result = stepExec(collections, insert_str); result == SQLITE_OK)
    node.id = getLastID();

  return result;
}

int
DBTools::insert(GroupInfo& group)
{
  auto result = SQLITE_OK;
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

  if (result = stepExec(collections, insert_str); result == SQLITE_OK)
    group.id = getLastID();

  return result;
}

int64_t
DBTools::getLastID()
{
  return sqlite3_last_insert_rowid(m_db);
}

int
DBTools::update(GroupInfo& group)
{
  auto result = SQLITE_OK;
  QString update_str("UPDATE groups SET "
                     "Name = ?, IsSubscription = ?, Type = ?, "
                     "Url = ?, CycleTime = ?, ModifiedAt = ? "
                     "WHERE ID = ?;");

  qint64 group_id = group.id;
  QVariantList collections = {
    group.name,       group.isSubscription, group.type, group.url,
    group.cycle_time, group.modified_time,  group_id,
  };

  result = stepExec(collections, update_str);

  return result;
}

int
DBTools::removeItemFromID(const QString& group_name, int64_t id)
{
  auto result = SQLITE_OK;
  QString remove_str = QString("DELETE FROM '%1' WHERE id = ?").arg(group_name);
  qint64 node_id = id;

  result = stepExec({ node_id }, remove_str);
  return result;
}

int
DBTools::removeGroupFromName(const QString& group_name, bool keep_group)
{
  auto result = SQLITE_OK;
  QString remove_str("DELETE FROM groups WHERE name = ?");
  auto drop_table_str = QString("DROP TABLE '%1'").arg(group_name);

  if (!keep_group) {
    if (result = stepExec({ group_name }, remove_str); result != SQLITE_OK) {
      p_logger->error("Failed to remove {}", group_name.toStdString());

      return result;
    }
  }

  char* err_msg;
  sqlite3_mutex_enter(sqlite3_db_mutex(m_db));
  result = sqlite3_exec(
    m_db, drop_table_str.toStdString().c_str(), NULL, NULL, &err_msg);
  sqlite3_mutex_leave(sqlite3_db_mutex(m_db));

  if (result != SQLITE_OK) {
    p_logger->error("Failed to remove nodes from group {}: {}",
                    group_name.toStdString(),
                    err_msg);
    sqlite3_free(err_msg);
  }

  return result;
}

std::vector<GroupInfo>
DBTools::listAllGroupsInfo()
{
  int result;
  GroupInfo temp;
  qint64 group_id;
  QString select_str("SELECT * FROM groups;");
  std::vector<GroupInfo> groups;

  QVariantList collection = {
    group_id, temp.name,       temp.isSubscription, temp.type,
    temp.url, temp.cycle_time, temp.created_time,   temp.modified_time
  };

  QVector<QVariantList> collections;

  result = stepExec(collection, select_str, &collections);
  if (result != SQLITE_OK) {
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
      group.created_time = item.at(6).toDateTime();
      group.modified_time = item.at(7).toDateTime();
      group.items = listAllNodesInfo(group.name).size();

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
  QString select_str = QString("SELECT * FROM \"%1\";").arg(group_name);

  return listNodesInfo(select_str);
}

std::vector<NodeInfo>
DBTools::listNodesInfo(const QString& select_str)
{
  int result;
  sqlite3_stmt* stmt;
  std::vector<NodeInfo> nodes;

  do {
    result = sqlite3_prepare_v2(
      m_db, select_str.toStdString().c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
      p_logger->error("SQL prepare error code: {}", result);
      break;
    }

    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
      NodeInfo node;

      node.id = sqlite3_column_int(stmt, 0);
      node.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
      node.group = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
      node.group_id = sqlite3_column_int(stmt, 3);
      node.protocol = static_cast<EntryType>(sqlite3_column_int(stmt, 4));
      node.address =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
      node.port = sqlite3_column_int(stmt, 6);
      node.password =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
      node.raw = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
      node.url = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
      node.created_time =
        QDateTime().fromSecsSinceEpoch(sqlite3_column_int64(stmt, 10));
      node.modified_time =
        QDateTime().fromSecsSinceEpoch(sqlite3_column_int64(stmt, 11));

      nodes.emplace_back(node);
    }

    if (result != SQLITE_DONE) {
      p_logger->error("SQL select error code: {}", sqlite3_errmsg(m_db));
    }

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
      p_logger->error("SQL stmt finalize error code: {}", result);
    }
  } while (false);
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

const QString
DBTools::getLibVersion()
{
  return sqlite3_libversion();
}

void
DBTools::close()
{
  if (m_db != nullptr) {
    sqlite3_close(m_db);
    m_db = nullptr;
    emit destroy();
  }
}
