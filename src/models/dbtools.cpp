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
DBTools::insertStepExec(const QVariantList& collections, const QString& sql_str)
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

    for (auto i = 0; i < collections.size(); ++i) {
      auto item = collections.at(i);
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

    sqlite3_mutex_enter(sqlite3_db_mutex(m_db));
    {
      result = sqlite3_step(stmt);
    }
    sqlite3_mutex_leave(sqlite3_db_mutex(m_db));
    if (result != SQLITE_DONE) {
      p_logger->error("SQL group insert step error code: {}", result);
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
  GroupInfo default_group{ 0, "default_group" };
  result = this->insert(default_group);
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

  result = insertStepExec(collections, insert_str);
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
  QString check_str =
    QString("SELECT Name FROM groups WHERE Name = '%1';").arg(group_name);

  int err = SQLITE_OK;
  bool result = false;
  sqlite3_stmt* stmt = nullptr;

  do {
    err = sqlite3_prepare_v2(
      m_db, check_str.toStdString().c_str(), -1, &stmt, nullptr);
    if (err != SQLITE_OK) {
      p_logger->error("SQL prepare error code: {}", err);
      break;
    }

    err = sqlite3_step(stmt);
    if (err == SQLITE_ROW) {
      result = true;
      break;
    }
  } while (false);

  if (stmt != nullptr) {
    err = sqlite3_finalize(stmt);
    if (err != SQLITE_OK) {
      p_logger->error("SQL stmt finalize error code: {}", err);
    }
  }

  return result;
}

int
DBTools::insert(NodeInfo& node)
{
  auto result = SQLITE_OK;
  QString insert_str =
    QString("INSERT INTO '%1' "
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

  if (result = insertStepExec(collections, insert_str); result == SQLITE_OK)
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

  if (result = insertStepExec(collections, insert_str); result == SQLITE_OK)
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
  char* err_msg;
  QString update_str =
    QString("UPDATE groups SET "
            "Name = '%1', IsSubscription = '%2', Type = '%3', "
            "Url = '%4', CycleTime = '%5', ModifiedAt = '%6' "
            "WHERE ID = '%7';")
      .arg(group.name)
      .arg(group.isSubscription)
      .arg(group.type)
      .arg(group.url)
      .arg(group.cycle_time)
      .arg(group.modified_time.toSecsSinceEpoch())
      .arg(group.id);
  std::string update = update_str.toStdString();

  sqlite3_mutex_enter(sqlite3_db_mutex(m_db));
  auto result = sqlite3_exec(m_db, update.c_str(), NULL, NULL, &err_msg);
  sqlite3_mutex_leave(sqlite3_db_mutex(m_db));

  if (result != SQLITE_OK) {
    p_logger->error("Update: {}", err_msg);
    sqlite3_free(err_msg);
  }

  return result;
}

int
DBTools::removeItemFromID(const QString& group_name, int64_t id)
{
  char* err_msg;
  QString remove_str =
    QString("DELETE FROM '%1' WHERE id = '%2'").arg(group_name).arg(id);
  std::string remove = remove_str.toStdString();

  sqlite3_mutex_enter(sqlite3_db_mutex(m_db));
  auto result = sqlite3_exec(m_db, remove.c_str(), NULL, NULL, &err_msg);
  sqlite3_mutex_leave(sqlite3_db_mutex(m_db));

  if (result != SQLITE_OK) {
    p_logger->error(
      "Failed to remove {}[{}]: {}", group_name.toStdString(), id, err_msg);
    sqlite3_free(err_msg);
  }

  return result;
}

int
DBTools::removeGroupFromName(const QString& group_name, bool keep_group)
{
  char* err_msg;
  auto result = SQLITE_OK;
  auto remove_str =
    QString("DELETE FROM groups WHERE name = '%1'").arg(group_name);
  auto drop_table_str = QString("DROP TABLE '%1'").arg(group_name);

  if (!keep_group) {
    sqlite3_mutex_enter(sqlite3_db_mutex(m_db));
    result = sqlite3_exec(
      m_db, remove_str.toStdString().c_str(), NULL, NULL, &err_msg);
    sqlite3_mutex_leave(sqlite3_db_mutex(m_db));

    if (result != SQLITE_OK) {
      p_logger->error(
        "Failed to remove {}: {}", group_name.toStdString(), err_msg);
      sqlite3_free(err_msg);

      return result;
    }
  }

  result = sqlite3_exec(
    m_db, drop_table_str.toStdString().c_str(), NULL, NULL, &err_msg);
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
  return listGroupsInfo(QString("SELECT * FROM groups;"));
}

std::vector<GroupInfo>
DBTools::listGroupsInfo(const QString& select_str)
{
  int result;
  sqlite3_stmt* stmt;
  std::vector<GroupInfo> groups;

  do {
    result = sqlite3_prepare_v2(
      m_db, select_str.toStdString().c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
      p_logger->error("SQL prepare error code: {}", result);
      break;
    }

    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
      GroupInfo group;

      group.id = sqlite3_column_int64(stmt, 0);
      group.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
      group.isSubscription = sqlite3_column_int(stmt, 2);
      group.type =
        magic_enum::enum_value<SubscriptionType>(sqlite3_column_int(stmt, 3));
      group.url = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
      group.cycle_time = sqlite3_column_int(stmt, 5);
      group.created_time =
        QDateTime().fromSecsSinceEpoch(sqlite3_column_int64(stmt, 6));
      group.modified_time =
        QDateTime().fromSecsSinceEpoch(sqlite3_column_int64(stmt, 7));
      group.items = listAllNodesInfo(group.name).size();

      groups.emplace_back(group);
    }

    if (result != SQLITE_DONE) {
      p_logger->error("SQL select error code: {}", sqlite3_errmsg(m_db));
    }

    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK) {
      p_logger->error("SQL stmt finalize error code: {}", result);
    }
  } while (false);
  return groups;
}

std::vector<NodeInfo>
DBTools::listAllNodesInfo(const QString& group_name)
{
  QString select_str = QString("SELECT * FROM '%1';").arg(group_name);

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
  for (auto& item : listAllGroupsInfo()) {
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
