#ifndef DBTOOLS_H
#define DBTOOLS_H

#include "../view_models/configtools.h"
#include "../view_models/logtools.h"

#include <QDateTime>
#include <QMap>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

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
  QString url = "";
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

  QSqlError stepExec(const QString& sql_str,
                     QVariantList* inputCollection = nullptr,
                     int outputColumns = 0,
                     QVector<QVariantList>* outputCollections = nullptr);

  QSqlError createDefaultGroup();

  QSqlError createGroupsTable();

  QSqlError createNodesTable(const QString& group_name);

  QSqlError createRuntimeTable();

  QSqlError createRuntimeValue(const QString& key, const QString& value);

  QString readRuntimeValue(const QString& key);

  QSqlError updateRuntimeValue(const QString& key, const QString& value);

  QSqlError deleteRuntimeValue(const QString& key);

  bool isTableExists(const QString& table_name);

  bool isGroupExists(const QString& group_name);

  QSqlError insert(NodeInfo& node);

  QSqlError insert(GroupInfo& group);

  int64_t getLastID();

  QSqlError update(GroupInfo& group);

  QSqlError removeItemFromID(const QString& group_name, int64_t id);

  QSqlError removeGroupFromName(const QString& group_name,
                                bool keep_group = false);

  QSqlError dropTable(const QString& table_name);

  std::vector<GroupInfo> listAllGroupsInfo();

  std::vector<GroupInfo> getAllGroupsInfo();

  std::vector<NodeInfo> listAllNodesInfo(const QString& group_name);

  std::map<int, NodesInfo> listAllNodes();

public slots:
  void close();

signals:
  void destroy();

private:
  QSqlError createTable(const QString& create_str);
  std::vector<GroupInfo> m_all_groups_info;

private:
  QSqlDatabase m_db;
  std::shared_ptr<across::utils::LogTools> p_logger;
  QSharedPointer<across::setting::ConfigTools> p_config = nullptr;
};
}

#endif // DBTOOLS_H
