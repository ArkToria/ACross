#include "app.h"
#include <fstream>
#include <sstream>

using namespace across;
using namespace across::core;
using namespace across::utils;
using namespace across::network;
using namespace across::setting;

Application::Application(int& argc, char** argv)
  : QApplication(argc, argv)
{
  p_logview = QSharedPointer<LogView>(new LogView());
  p_config = QSharedPointer<ConfigTools>(new ConfigTools(this));
  p_db = QSharedPointer<DBTools>(new DBTools());
  p_core = QSharedPointer<CoreTools>(new CoreTools());
  p_curl = QSharedPointer<CURLTools>(new CURLTools);
  p_nodes = QSharedPointer<NodeList>(new NodeList());
  p_groups = QSharedPointer<GroupList>(new GroupList);
  p_tray = QSharedPointer<SystemTray>(new SystemTray());
  p_image_provider = new ImageProvider();
  p_config->init(p_curl);

  registerModels();
  setRootContext();
  setTranslator(p_config->currentLanguage());

  // dynamic change display language
  connect(p_config.get(),
          &across::setting::ConfigTools::currentLanguageChanged,
          this,
          [&](QString lang) { setTranslator(lang); });

  // dynamic load qrcode image content
  connect(p_nodes.get(),
          &across::NodeList::updateQRCode,
          p_image_provider,
          &across::ImageProvider::setContent);
}

Application::~Application()
{
  m_engine.removeImageProvider("acrossImageProvider");
}

int
Application::run()
{
  return this->exec();
}

void
Application::setRootContext()
{
  const QUrl url(QStringLiteral("qrc:/ACross/src/views/main.qml"));
  QObject::connect(
    &m_engine,
    &QQmlApplicationEngine::objectCreated,
    this,
    [url](QObject* obj, const QUrl& objUrl) {
      if (!obj && url == objUrl) {
        QCoreApplication::exit(-1);
      }
    },
    Qt::QueuedConnection);

  m_engine.addImportPath(u"qrc:/"_qs);
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossLogView"),
                                             p_logview.get());
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossConfig"),
                                             p_config.get());
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossCore"),
                                             p_core.get());
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossNodes"),
                                             p_nodes.get());
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossGroups"),
                                             p_groups.get());
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossTray"),
                                             p_tray.get());
  m_engine.addImageProvider(QStringLiteral("acrossImageProvider"),
                            p_image_provider);
  m_engine.load(url);

  p_db->init(p_logview, p_config);
  p_core->init(p_logview, p_config);
  p_nodes->init(p_logview, p_config, p_core, p_db);
  p_groups->init(p_logview, p_config, p_db, p_nodes, p_curl);
  p_tray->init(p_logview, p_config, p_core, p_nodes);
}

void
Application::setTranslator(const QString& lang)
{
  if (!m_translator.isEmpty()) {
    this->removeTranslator(&m_translator);
  }

  auto file_path =
    u":/i18n/%1_%2.qm"_qs.arg(m_app_name.toLower(), QLocale(lang).name());

  if (QFile(file_path).exists()) {
    if (m_translator.load(file_path)) {
      this->installTranslator(&m_translator);
    }
  }

  m_engine.retranslate();
  p_tray->retranslate();
}

void
Application::registerModels()
{
  qmlRegisterType<across::GroupModel>(APP_NAME, 1, 0, "GroupModel");
  qmlRegisterType<across::NodeModel>(APP_NAME, 1, 0, "NodeModel");
  qmlRegisterType<across::NodeFormModel>(APP_NAME, 1, 0, "NodeFormModel");
  qmlRegisterType<across::VMessFormModel>(APP_NAME, 1, 0, "VMessFormModel");
  qmlRegisterType<across::TrojanFormModel>(APP_NAME, 1, 0, "TrojanFormModel");
  qmlRegisterType<across::ShadowsocksFormModel>(
    APP_NAME, 1, 0, "ShadowsocksFormModel");
  qmlRegisterType<across::RawOutboundFormModel>(
    APP_NAME, 1, 0, "RawOutboundFormModel");
  qmlRegisterType<across::URLSchemeFormModel>(
    APP_NAME, 1, 0, "URLSchemeFormModel");
}
