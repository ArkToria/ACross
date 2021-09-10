#ifndef APPLICATION_H
#define APPLICATION_H

#include "spdlog/async.h"
#include "spdlog/spdlog.h"
#include <QApplication>
#include <QFont>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStringLiteral>
#include <QTranslator>
#include <QUrl>

#include "models/dbtools.h"
#include "models/envtools.h"
#include "view_models/configtools.h"
#include "view_models/grouplist.h"
#include "view_models/groupmodel.h"
#include "view_models/logview.h"
#include "view_models/nodeformmodel.h"
#include "view_models/nodelist.h"
#include "view_models/nodemodel.h"
#include "view_models/rawoutboundformmodel.h"
#include "view_models/shadowsocksformmodel.h"
#include "view_models/trojanformmodel.h"
#include "view_models/urlschemeformmodel.h"
#include "view_models/vmessformmodel.h"
#include "view_models/systemtray.h"

namespace across {
#define APP_NAME "ACross"

class Application : public QApplication
{
  Q_OBJECT
public:
  explicit Application(int& argc, char** argv);

  ~Application();

  int run();

  void setRootContext();

  void setTranslator(const QString& lang = "current");

  void registerModels();

private:
  int m_thread_nums = 1;
  int m_queue_size = 8192;
  std::shared_ptr<spdlog::details::thread_pool> p_thread_pool;

  across::LogView acrossLog;
  across::setting::ConfigTools acrossConfig;
  across::DBTools acrossDB;
  across::core::CoreTools acrossCore;
  across::network::CURLTools acrossCurl;
  across::NodeList acrossNodes;
  across::GroupList acrossGroups;
  across::SystemTray acrossTray;

  const QString m_app_name = APP_NAME;
  QTranslator m_translator;
  QQmlApplicationEngine m_engine;
};
}

#endif // APPLICATION_H
