#ifndef LOGLIST_H
#define LOGLIST_H

#include "configtools.h"

#include <QObject>
#include <QPair>

namespace across {
class LogList : public QObject
{
  Q_OBJECT
public:
  explicit LogList(QObject* parent = nullptr);

  ~LogList();

  void init(across::setting::ConfigTools& config);

  void clean();

  void append(const QString& msg);

  void remove();

  QPair<int, QString> getLastItem();

  QPair<int, QString> getFirstItem();

  QString getItem(int index);

  size_t getSize();

  void styleFomatter(QString& msg);

signals:
  void preItemsReset();
  void postItemsReset();

  void preItemAppended();
  void postItemAppended();

  void preItemRemoved(int index);
  void postItemRemoved();

  void preFirstItemRemoved();
  void postFirstItemRemoved();

private:
  QSharedPointer<QContiguousCache<QString>> p_core_cache;
  QMap<QString, QString> colors_map;

  static void doDeleteLater(QContiguousCache<QString>* obj);
};
}

#endif // LOGLIST_H
