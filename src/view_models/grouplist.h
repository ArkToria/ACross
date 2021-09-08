#ifndef ACROSS_GROUPLIST_H
#define ACROSS_GROUPLIST_H

#include <memory>

#include "magic_enum.hpp"
#include <QObject>
#include <QPointer>
#include <QVariant>

#include "../models/clipboardtools.h"
#include "../models/dbtools.h"
#include "../models/networktools.h"
#include "../models/notifytools.h"
#include "../models/serializetools.h"
#include "nodelist.h"

namespace across {
class GroupList : public QObject
{
  Q_OBJECT
public:
  explicit GroupList(QObject* parent = nullptr);

  void init(std::shared_ptr<spdlog::details::thread_pool> thread_pool,
            DBTools& db,
            across::network::CURLTools& curl_tools,
            across::NodeList& node_list);

  QVector<GroupInfo> items() const;

  bool insertSIP008(const std::stringstream& data_stream,
                    const across::network::CURLTools::DownloadTask& task);

  bool insertBase64(const std::stringstream& data_stream,
                    const across::network::CURLTools::DownloadTask& task);

public slots:
  void reloadItems(bool reopen_db = false);

  void appendItem(const QString& group_name,
                  const QString& url,
                  int type,
                  int cycle_time);

  void appendItem(const QString& group_name, const QString& node_items);

  void removeItem(int index);

  void upgradeItem(int index);

  void setDisplayGroupID(int id);

  void copyUrlToClipboard(int index);

signals:
  void preItemAppended();
  void postItemAppended();

  void preItemRemoved(int index);
  void postItemRemoved();

  void preLastItemRemoved();
  void postLastItemRemoved();

private:
  across::DBTools* p_db;
  across::NodeList* p_nodes;
  across::network::CURLTools* p_curl;
  std::shared_ptr<spdlog::details::thread_pool> p_thread_pool;

  QVector<GroupInfo> m_items;
  std::shared_ptr<LogTools> p_logger;
  int64_t m_current_group_id;
};
}

#endif // ACROSS_GROUPLIST_H
