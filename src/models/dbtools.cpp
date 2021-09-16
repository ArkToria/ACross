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

  if (p_config == nullptr) {
    return;
  }

  auto db_config = p_config->getDBConfig();

  if (db_config.path.isEmpty()) {
    p_logger->error("Failed to load database on path");
    return;
  } else {
    p_logger->info("Open database on path: {}", db_config.path.toStdString());
  }

  int result = sqlite3_open(db_config.path.toStdString().c_str(), &m_db);
  if (result != SQLITE_OK) {
    p_logger->error("SQL open error code: {}", result);
    return;
  } else {
    p_logger->info("SQL opened successfully");
  }

  char* err_msg;
  sqlite3_mutex_enter(sqlite3_db_mutex(m_db));

  std::string init_str{ "PRAGMA encoding='UTF-8';"
                        "PRAGMA synchronous=OFF;"
                        "PRAGMA count_changes=OFF;"
                        "PRAGMA journal_mode=MEMORY;"
                        "PRAGMA temp_store=MEMORY;" };

  result = sqlite3_exec(m_db, init_str.c_str(), NULL, NULL, &err_msg);
  if (result != SQLITE_OK) {
    p_logger->error("SQL initialization error code: {}", err_msg);
    sqlite3_free(err_msg);
  }

  if (!isTableExists({ "groups" })) {
    result = createGroupsTable();

    if (result == SQLITE_OK) {
      p_logger->info("SQL create tables successfully");
    } else {
      p_logger->error("SQL failed to create tables");
    }
  }

  if (!isGroupExists("default_group")) {
    result = createDefaultGroup();

    if (result == SQLITE_OK) {
      p_logger->info("SQL create default group successfully");

      result = createNodesTable("default_group");
      if (result != SQLITE_OK) {
        p_logger->error("Failed to create default nodes table: {}", result);
      }
    }
  }

  sqlite3_mutex_leave(sqlite3_db_mutex(m_db));
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
DBTools::createTable(const std::string& create_str)
{
  char* err_msg;

  int result = sqlite3_exec(m_db, create_str.c_str(), nullptr, 0, &err_msg);

  if (result != SQLITE_OK) {
    p_logger->error("SQL create table error: {}", err_msg);
    sqlite3_free(err_msg);
  }

  return result;
}

int
DBTools::createGroupsTable()
{

  std::string create_groups_str{ "CREATE TABLE IF NOT EXISTS groups("
                                 "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "Name TEXT UNIQUE,"
                                 "IsSubscription BOOLEAN,"
                                 "Type INTEGER,"
                                 "Url TEXT,"
                                 "CycleTime INTEGER,"
                                 "CreatedAt INT64 NOT NULL,"
                                 "ModifiedAt INT64 NOT NULL);" };

  auto result = createTable(create_groups_str);

  return result;
}

int
DBTools::createNodesTable(const std::string& group_name)
{
  std::string create_nodes_str =
    fmt::format({ "CREATE TABLE IF NOT EXISTS '{}'("
                  "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "Name TEXT NOT NULL,"
                  "GroupName TEXT NOT NULL,"
                  "GroupID INTEGER NOT NULL,"
                  "Protocol INTEGER NOT NULL,"
                  "Address CHAR(255) NOT NULL,"
                  "Port INTEGER NOT NULL,"
                  "Password TEXT NOT NULL,"
                  "Raw TEXT NOT NULL,"
                  "CreatedAt INT64 NOT NULL,"
                  "ModifiedAt INT64 NOT NULL);" },
                group_name);

  auto result = createTable(create_nodes_str);

  return result;
}

bool
DBTools::isTableExists(const std::vector<std::string>& table_names)
{
  bool result = true;
  int err = SQLITE_OK;
  std::string err_name;
  sqlite3_stmt* stmt = nullptr;

  for (auto name : table_names) {
    std::string result_name;
    std::string check_str =
      fmt::format("SELECT Name FROM sqlite_master WHERE Type = 'table' AND "
                  "Name = '{}';",
                  name);

    err = sqlite3_prepare_v2(m_db, check_str.c_str(), -1, &stmt, nullptr);
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

    if (result_name != name) {
      err_name = name;
      p_logger->warn("SQL check table `{}` doesn't exists", name);
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
DBTools::isGroupExists(const std::string& group_name)
{
  std::string check_str =
    fmt::format("SELECT Name FROM groups WHERE Name = '{}';", group_name);
  std::string result_name;
  int err = SQLITE_OK;
  bool result = false;
  sqlite3_stmt* stmt = nullptr;

  do {
    err = sqlite3_prepare_v2(m_db, check_str.c_str(), -1, &stmt, nullptr);
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
  int result;
  sqlite3_stmt* stmt;

  if (node.created_time.isNull()) {
    node.created_time = QDateTime::currentDateTime();
  }

  if (node.modified_time.isNull()) {
    node.modified_time = QDateTime::currentDateTime();
  }

  std::string insert_str =
    fmt::format({ "INSERT INTO '{}' "
                  "(Name, GroupName, GroupID, Protocol, Address, Port, "
                  "Password, Raw, CreatedAt, ModifiedAt) "
                  "VALUES(?,?,?,?,?,?,?,?,?,?)" },
                node.group.toStdString());

  do {
    result = sqlite3_prepare_v2(m_db, insert_str.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
      p_logger->error("SQL prepare error code: {}", result);
      break;
    }

    std::string name = node.name.toStdString();
    result = sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind name error code: {}", result);
      break;
    }

    std::string group = node.group.toStdString();
    result = sqlite3_bind_text(stmt, 2, group.c_str(), -1, SQLITE_STATIC);
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind group name error code: {}", result);
      break;
    }

    result = sqlite3_bind_int(stmt, 3, node.group_id);
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind group id error code: {}", result);
      break;
    }

    result = sqlite3_bind_int(stmt, 4, node.protocol);
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind protocol error code: {}", result);
      break;
    }

    std::string address = node.address.toStdString();
    result = sqlite3_bind_text(stmt, 5, address.c_str(), -1, SQLITE_STATIC);
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind address error code: {}", result);
      break;
    }

    result = sqlite3_bind_int(stmt, 6, node.port);
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind port error code: {}", result);
      break;
    }

    std::string password = node.password.toStdString();
    result = sqlite3_bind_text(stmt, 7, password.c_str(), -1, SQLITE_STATIC);
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind password error code: {}", result);
      break;
    }

    std::string raw = node.raw.toStdString();
    result = sqlite3_bind_text(stmt, 8, raw.c_str(), -1, SQLITE_STATIC);
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind raw error code: {}", result);
      break;
    }

    result = sqlite3_bind_int64(stmt, 9, node.created_time.toTime_t());
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind created time error code: {}", result);
      break;
    }

    result = sqlite3_bind_int64(stmt, 10, node.modified_time.toTime_t());
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind modified time error code: {}", result);
      break;
    }

    sqlite3_mutex_enter(sqlite3_db_mutex(m_db));
    {
      result = sqlite3_step(stmt);
      if (result != SQLITE_DONE) {
        p_logger->error("SQL insert node step error code: {}", result);
      }
    }
    sqlite3_mutex_leave(sqlite3_db_mutex(m_db));

    result = sqlite3_reset(stmt);
    if (result != SQLITE_OK) {
      p_logger->error("SQL stmt reset error code: {}", result);
    }
  } while (false);

  result = sqlite3_finalize(stmt);
  if (result != SQLITE_OK) {
    p_logger->error("SQL stmt finalize error code: {}", result);
  }

  return result;
}

int
DBTools::insert(GroupInfo& group)
{
  int result;
  sqlite3_stmt* stmt;

  std::string insert_str{
    "INSERT INTO groups"
    "(Name, IsSubscription, Type, Url, CycleTime, CreatedAt, ModifiedAt)"
    "VALUES(?,?,?,?,?,?,?)"
  };

  do {
    result = sqlite3_prepare_v2(m_db, insert_str.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
      p_logger->error("SQL prepare error code: {}", result);
      break;
    }

    std::string name = group.name.toStdString();
    result = sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind group name error code: {}", result);
      break;
    }

    result = sqlite3_bind_int(stmt, 2, group.isSubscription);
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind group isSubscription error code: {}", result);
      break;
    }

    result = sqlite3_bind_int(stmt, 3, group.type);
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind group isSubscription error code: {}", result);
      break;
    }

    std::string url = group.url.toStdString();
    result = sqlite3_bind_text(stmt, 4, url.c_str(), -1, SQLITE_STATIC);
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind group url error code: {}", result);
      break;
    }

    result = sqlite3_bind_int(stmt, 5, group.cycle_time);
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind group isSubscription error code: {}", result);
      break;
    }

    result = sqlite3_bind_int64(stmt, 6, group.created_time.toTime_t());
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind group created time error code: {}", result);
      break;
    }

    result = sqlite3_bind_int64(stmt, 7, group.modified_time.toTime_t());
    if (result != SQLITE_OK) {
      p_logger->error("SQL bind group modified time error code: {}", result);
      break;
    }

    sqlite3_mutex_enter(sqlite3_db_mutex(m_db));
    {
      result = sqlite3_step(stmt);
      if (result != SQLITE_DONE) {
        p_logger->error("SQL group insert step error code: {}", result);
      }
    }
    sqlite3_mutex_leave(sqlite3_db_mutex(m_db));

    result = sqlite3_reset(stmt);
    if (result != SQLITE_OK) {
      p_logger->error("SQL stmt reset error code: {}", result);
    }
  } while (false);

  result = sqlite3_finalize(stmt);
  if (result != SQLITE_OK) {
    p_logger->error("SQL stmt finalize error code: {}", result);
  }

  return result;
}

int64_t
DBTools::getLastID()
{
  return sqlite3_last_insert_rowid(m_db);
}

int
DBTools::removeItemFromID(const std::string& group_name, int64_t id)
{
  char* err_msg;
  std::string remove_str =
    fmt::format("DELETE FROM {} WHERE id = '{}'", group_name, id);

  auto result = sqlite3_exec(m_db, remove_str.c_str(), NULL, NULL, &err_msg);
  if (result != SQLITE_OK) {
    p_logger->error("Failed to remove {}[{}]: {}", group_name, id, err_msg);
  }

  return result;
}

int
DBTools::removeGroupFromName(const std::string& group_name)
{
  char* err_msg;
  std::string remove_str =
    fmt::format("DELETE FROM groups WHERE name = '{}'", group_name);

  auto result = sqlite3_exec(m_db, remove_str.c_str(), NULL, NULL, &err_msg);
  if (result != SQLITE_OK) {
    p_logger->error("Failed to remove {}: {}", group_name, err_msg);
  } else {
    std::string drop_table_str = fmt::format("DROP TABLE '{}'", group_name);

    result = sqlite3_exec(m_db, drop_table_str.c_str(), NULL, NULL, &err_msg);
    if (result != SQLITE_OK) {
      p_logger->error(
        "Failed to remove nodes from group {}: {}", group_name, err_msg);
    }
  }

  return result;
}

std::vector<GroupInfo>
DBTools::listAllGroupsInfo()
{
  std::string select_str{ "SELECT * FROM groups;" };

  return listGroupsInfo(select_str);
}

std::vector<GroupInfo>
DBTools::listGroupsInfo(const std::string& select_str)
{
  int result;
  sqlite3_stmt* stmt;
  std::vector<GroupInfo> groups;

  do {
    result = sqlite3_prepare_v2(m_db, select_str.c_str(), -1, &stmt, nullptr);
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
        QDateTime().fromTime_t(sqlite3_column_int64(stmt, 6));
      group.modified_time =
        QDateTime().fromTime_t(sqlite3_column_int64(stmt, 7));
      group.items = listAllNodesInfo(group.name.toStdString()).size();

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
DBTools::listAllNodesInfo(const std::string& group_name)
{
  std::string select_str = fmt::format({ "SELECT * FROM '{}';" }, group_name);

  return listNodesInfo(select_str);
}

std::vector<NodeInfo>
DBTools::listNodesInfo(const std::string& select_str)
{
  int result;
  sqlite3_stmt* stmt;
  std::vector<NodeInfo> nodes;

  do {
    result = sqlite3_prepare_v2(m_db, select_str.c_str(), -1, &stmt, nullptr);
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
      node.created_time = QDateTime().fromTime_t(sqlite3_column_int64(stmt, 9));
      node.modified_time =
        QDateTime().fromTime_t(sqlite3_column_int64(stmt, 10));

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

  for (auto& iter : listAllGroupsInfo()) {
    auto nodes = listAllNodesInfo(iter.name.toStdString());

    all_nodes.insert(
      { iter.id,
        { 0,
          0,
          nodes } }); // group id, { current node id, current index, nodes }
  }

  return all_nodes;
}

const std::string
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
