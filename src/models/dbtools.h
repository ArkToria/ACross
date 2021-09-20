#ifndef DBTOOLS_H
#define DBTOOLS_H

#include "fmt/format.h"
#include "sqlite3.h"
#include <QDateTime>
#include <QMap>
#include <chrono>

#include "../view_models/configtools.h"
#include "../view_models/logtools.h"

#include "magic_enum.hpp"

namespace across {
enum SubscriptionType : int
{
  sip008 = 0,
  base64,
  jsonraw,
  none
};

enum EntryType
{
  vmess,
  shadowsocks,
  trojan,
  raw,
  scheme,
  unknown,
};

struct NodeInfo
{
  int64_t id = 0;
  QString name = "";
  QString group = "";
  int64_t group_id = 0;
  EntryType protocol;
  QString address = "";
  uint port = 0;
  QString password = "";
  QString raw = "";
  QDateTime created_time;
  QDateTime modified_time;
};

struct NodesInfo
{
  int64_t current_id = 0;
  int current_index = 0;
  std::vector<NodeInfo> nodes;
};

struct GroupInfo
{
  int64_t id = 0;
  QString name = "";
  bool isSubscription = false;
  SubscriptionType type = none;
  QString url = "";
  int cycle_time = 0;
  QDateTime created_time = QDateTime::currentDateTime();
  QDateTime modified_time = QDateTime::currentDateTime();
  int items = 0;
};

class DBTools : public QObject
{
  Q_OBJECT
public:
  explicit DBTools(QObject* parent = nullptr);

  ~DBTools();

  void init(QSharedPointer<LogView> log_view,
            QSharedPointer<across::setting::ConfigTools> config);

  void reload();

  int createDefaultGroup();

  int createGroupsTable();

  int createNodesTable(const std::string& group_name);

  bool isTableExists(const std::vector<std::string>& table_names);

  bool isGroupExists(const std::string& group_name);

  int insert(NodeInfo& node);

  int insert(GroupInfo& group);

  int64_t getLastID();

  int removeItemFromID(const std::string& group_name, int64_t id);

  int removeGroupFromName(const std::string& group_name);

  std::vector<GroupInfo> listAllGroupsInfo();

  std::vector<NodeInfo> listAllNodesInfo(const std::string& group_name);

  std::map<int, NodesInfo> listAllNodes();

  const std::string getLibVersion();

public slots:
  void close();

signals:
  void destroy();

private:
  int createTable(const std::string& create_str);
  std::vector<GroupInfo> listGroupsInfo(const std::string& select_str);
  std::vector<NodeInfo> listNodesInfo(const std::string& select_str);

private:
  std::shared_ptr<across::utils::LogTools> p_logger;
  QSharedPointer<across::setting::ConfigTools> p_config = nullptr;
  sqlite3* m_db = nullptr;
};
}

#endif // DBTOOLS_H
