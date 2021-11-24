#ifndef DBTOOLS_H
#define DBTOOLS_H

#include "../view_models/logtools.h"

#include <QDateTime>
#include <QMap>
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

    RuntimeValue(const QString &key, const QVariant &value);
    RuntimeValue(const RunTimeValues key, const QVariant &value);
    RuntimeValue(const QString &key, int type, const QVariant &value);
};

class DBWorker : public QObject {
    Q_OBJECT
  public:
    explicit DBWorker(QObject *parent = nullptr);

  public slots:
    void directExec(const QString &sql_str, QSqlDatabase &db);

    void
    stepExec(const QString &sql_str, QVariantList *inputCollection,
                  int outputColumns, QList<QVariantList> *outputCollections
                  , std::shared_ptr<spdlog::logger> p_logger, QSqlDatabase &db);

  signals:
    void directExecReady(QSqlError result);
    void stepExecReady(QPair<QSqlError, qint64> result);
};

class DBTools : public QObject {
    Q_OBJECT

  public:
    explicit DBTools(QObject *parent = nullptr);

    ~DBTools();

    void init(const QString &db_path);
    void reload();
    bool isTableExists(const QString &table_name);
    bool isItemExists(const QString &group_name,
                      const QString &table_name = "groups");

    QSqlError createDefaultTables();
    QSqlError createDefaultValues();
    QSqlError createDefaultGroup();
    QSqlError createDefaultRouting();

    QSqlError insert(NodeInfo &node);
    QSqlError insert(QList<NodeInfo> &nodes);
    QSqlError update(NodeInfo &node);
    QSqlError update(QList<NodeInfo> &nodes);

    QSqlError insert(GroupInfo &group);
    QSqlError update(GroupInfo &group);
    QSqlError update(QList<GroupInfo> &groups);

    QSqlError insert(RoutingInfo &routing);
    QSqlError update(RoutingInfo &routing);

    QSqlError removeNodeFromID(qint64 id);
    QSqlError removeGroupFromID(qint64 id, bool keep_group = false);
    QSqlError reloadAllGroupsInfo();

    QSqlError createRuntimeValue(const RuntimeValue &value);
    std::optional<RuntimeValue> readRuntimeValue(const QString &key);
    std::optional<RuntimeValue> readRuntimeValue(const RunTimeValues &key);
    QSqlError updateRuntimeValue(const RuntimeValue &value);
    QSqlError deleteRuntimeValue(const QString &key);
    QSqlError deleteRuntimeValue(const RunTimeValues &key);

    qint64 getCurrentNodeID();
    qint64 getCurrentGroupID();
    qint64 getDefaultNodeID();
    qint64 getDefaultGroupID();
    qsizetype getSizeFromGroupID(qint64 group_id);
    std::optional<GroupInfo> getGroupFromID(qint64 group_id);
    QList<GroupInfo> getAllGroupsInfo();
    QList<NodeInfo> listAllNodesFromGroupID(qint64 group_id);
    QMap<qint64, QList<qint64>> search(const QString &value);

  public slots:
    void close();

  signals:
    void startDirectExec(const QString &sql_str, QSqlDatabase &db);
    void startStepExec(const QString &sql_str, QVariantList *inputCollection,
                  int outputColumns, QList<QVariantList> *outputCollections
                  , std::shared_ptr<spdlog::logger> p_logger, QSqlDatabase &db);

    void destroy();

  private:
    QSqlError directExec(const QString &sql_str);

    QPair<QSqlError, qint64>
    stepExec(const QString &sql_str, QVariantList *inputCollection = nullptr,
             int outputColumns = 0,
             QList<QVariantList> *outputCollections = nullptr);

  private:
    friend class TransactionWrap;

    QSqlDatabase m_db;
    QList<GroupInfo> m_groups;

    QThread *p_db_thread = nullptr;

    DBWorker *p_worker = nullptr;
    QSqlError tmp_direct_res;
    QPair<QSqlError, qint64> tmp_step_res;

    std::shared_ptr<spdlog::logger> p_logger;
    QString m_db_path = "across.db";
};

class TransactionWrap {
  public:
    inline TransactionWrap(DBTools *db_tools) : p_db(db_tools) {
        mutex.lock();
        p_db->directExec("BEGIN");
    }

    inline ~TransactionWrap() {
        p_db->directExec("END;");
        mutex.unlock();
    }

  private:
    QPointer<DBTools> p_db;
    QMutex mutex;
};
} // namespace across

#endif // DBTOOLS_H
