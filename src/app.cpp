#include "app.h"

using namespace across;
using namespace across::core;

static QApplication*
createApplication(int& argc, char** argv)
{
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  QApplication::setOrganizationName("qv2ray");
  QApplication::setApplicationName(APP_NAME);
  QApplication::setOrganizationDomain("qv2ray.org");
  QApplication::setApplicationDisplayName("ACross - V2ray Client");
  QApplication::setWindowIcon(QIcon::fromTheme("org.qv2ray.across"));
  QApplication* app = new QApplication(argc, argv);

  return app;
}

Application::Application(int& argc, char** argv)
  : p_app(createApplication(argc, argv))
{
  // initial global logger thread pool
  spdlog::init_thread_pool(m_queue_size, m_thread_nums);
  m_thread_pool = spdlog::thread_pool();

  // create self logger for Application
  p_logger = std::make_shared<LogTools>(m_thread_pool, "app");

  // setup font family
  p_app->setFont(QFont("Noto Sans", 10));

  setupQmlRegisterTypes();

  setupQmlContextProperties();

  // initial translator and read interface language from config
  setupTranslator(acrossConfig.currentLanguage());

  // dynamic change display language
  connect(&acrossConfig,
          &across::setting::ConfigTools::currentLanguageChanged,
          this,
          [&](QString lang) { setupTranslator(lang); });
}

Application::~Application()
{
  // TODO: find out the reason cause gdb SIG32 nostop
  if (p_app != nullptr) {
    p_app->deleteLater();
  }
}

int
Application::run()
{
  p_logger->info("Start Application");

  return p_app->exec();
}

void
Application::setupTranslator(const QString& lang)
{
  p_app->removeTranslator(&m_translator);

  const QString baseName =
    m_application_name.toLower() + "_" + QLocale(lang).name();

  auto file_path = ":/" + baseName + ".qm";

  if (QFile(file_path).exists()) {
    if (m_translator.load(file_path)) {
      p_app->installTranslator(&m_translator);
    }
  }
}

void
Application::setupQmlRegisterTypes()
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

void
Application::setupQmlContextProperties()
{
  acrossConfig.init(m_thread_pool);
  acrossDB.init(m_thread_pool, acrossConfig);

  auto core_config = acrossConfig.getCore();
  acrossCore.init(core_config, acrossLog);

  acrossNodes.init(m_thread_pool, acrossDB, acrossConfig, acrossCore);
  acrossGroups.init(m_thread_pool, acrossDB, acrossCurl, acrossNodes);

  const QUrl url(QStringLiteral("qrc:/src/views/main.qml"));

  QObject::connect(
    &m_engine,
    &QQmlApplicationEngine::objectCreated,
    p_app,
    [url](QObject* obj, const QUrl& objUrl) {
      if (!obj && url == objUrl)
        QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);

  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossEnvs"),
                                             &acrossEnvs);
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossConfig"),
                                             &acrossConfig);
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossCore"),
                                             &acrossCore);
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossNodes"),
                                             &acrossNodes);
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossGroups"),
                                             &acrossGroups);
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossLog"),
                                             &acrossLog);
  m_engine.load(url);
}
