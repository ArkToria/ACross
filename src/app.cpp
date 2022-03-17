#include "app.h"

using namespace across;
using namespace across::core;
using namespace across::utils;
using namespace across::network;
using namespace across::setting;
using namespace across::acolorsapi;

Application::Application(int &argc, char **argv)
    : SingleApplication(argc, argv, true,
                        User | ExcludeAppPath | ExcludeAppVersion) {
    setWindowIcon(QIcon(":misc/design/logo.svg"));
    setDesktopFileName("org.arktoria.across");

#ifdef Q_OS_MAC
    setStyle("Basic"); // fix styleFont not set
#endif

    connect(this, &SingleApplication::receivedMessage, this,
            &Application::onMessageReceived, Qt::QueuedConnection);
}

bool Application::initialize() {
    if (isSecondary()) {
        sendMessage("Display mainWindow");
        exitReason = EXIT_SECONDARY_INSTANCE;
        return false;
    }

    p_config = QSharedPointer<ConfigTools>::create(this);

    // dynamic change display theme
    connect(p_config.get(), &ConfigTools::currentThemeChanged, &m_log,
            &LogView::setTheme);

    // p_db = QSharedPointer<DBTools>::create();
    p_acolors = QSharedPointer<AColoRSAPITools>::create("127.0.0.1:19198");
    p_core = QSharedPointer<CoreTools>::create(p_acolors);
    p_curl = QSharedPointer<CURLTools>::create();
    p_nodes = QSharedPointer<NodeList>::create();
    p_groups = QSharedPointer<GroupList>::create();
    p_tray = QSharedPointer<SystemTray>::create();
    p_notifications = QSharedPointer<NotificationModel>::create();
    p_image_provider = new ImageProvider; // free by qml engine

    p_config->init(p_curl, p_acolors);

    registerModels();
    setRootContext();
    setTranslator(p_config->currentLanguage());

    // dynamic change display language
    connect(p_config.get(),
            &across::setting::ConfigTools::currentLanguageChanged, this,
            [&](QString lang) { setTranslator(lang); });

    // dynamic load qrcode image content
    connect(p_nodes.get(), &across::NodeList::updateQRCode, p_image_provider,
            &across::ImageProvider::setContent);

    connect(p_config.get(), &across::setting::ConfigTools::corePathChanged,
            this, &Application::restartAColoRS);
    connect(p_config.get(), &across::setting::ConfigTools::acolorsPathChanged,
            this, &Application::restartAColoRS);
    connect(p_config.get(),
            &across::setting::ConfigTools::acolorsConfigPathChanged, this,
            &Application::restartAColoRS);
    connect(p_config.get(), &across::setting::ConfigTools::acolorsDbPathChanged,
            this, &Application::restartAColoRS);
    connect(p_acolors->notifications(),
            &across::acolorsapi::AColoRSNotifications::shutdown, this,
            &Application::handleShutdown);

    wait(200);
    checkAndReconnect();

    return true;
}

void Application::restartAColoRS() {
    this->acolors_restarting = true;
    this->p_acolors->shutdown();
    wait(200);
    Application::checkAndReconnect();
}

void Application::handleShutdown() {
    if (this->acolors_restarting) {
        this->acolors_restarting = false;
        return;
    }
    this->exit(0);
}

void Application::wait(int msec) {
    QTimer timer;
    QEventLoop loop;
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(msec);
    loop.exec();
}

void Application::checkAndReconnect() {
    if (this->p_acolors->isConnected())
        return;
    this->p_acolors->startProcess(
        p_config->acolorsPath(), p_config->acolorsAPIPort().toInt(),
        p_config->corePath(), p_config->acolorsConfigPath(),
        p_config->acolorsDbPath());
    wait(500);
    this->p_acolors->reconnect();
}

ACrossExitReason Application::getExitReason() { return exitReason; }

Application::~Application() {
    m_engine.removeImageProvider("acrossImageProvider");

    spdlog::shutdown();
}

int Application::run() { return this->exec(); }

void Application::setRootContext() {
    const QUrl url(QStringLiteral("qrc:/Arktoria/ACross/src/views/main.qml"));

    QObject::connect(
        &m_engine, &QQmlApplicationEngine::objectCreated, this,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);

    m_engine.addImportPath(u"qrc:/"_qs);
    m_engine.rootContext()->setContextProperty(QStringLiteral("acrossLogView"),
                                               &m_log);
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
    m_engine.rootContext()->setContextProperty(
        QStringLiteral("acrossNotifications"), p_notifications.get());
    m_engine.rootContext()->setContextProperty(
        QStringLiteral("acolorsNotifications"), p_acolors->notifications());
    m_engine.rootContext()->setContextProperty(QStringLiteral("acolors"),
                                               p_acolors.get());
    m_engine.rootContext()->setContextProperty(
        QStringLiteral("fixedFont"),
        QFontDatabase::systemFont(QFontDatabase::FixedFont));
    m_engine.rootContext()->setContextProperty(
        QStringLiteral("mainWindowVisible"),
        !p_config->enableStartFromMinimized());
    m_engine.addImageProvider(QStringLiteral("acrossImageProvider"),
                              p_image_provider);
    m_engine.load(url);

    QObject *popNotify = nullptr;
    for (auto object : m_engine.rootObjects()) {
        if (object->objectName() == "mainWindow") {
            popNotify = object->findChild<QObject *>("popNotify");
            break;
        }
    }

    p_notifications->init(popNotify);
    p_config->setLogMode();
    // p_db->init(p_config->dbPath());
    p_core->init(p_config, p_notifications);
    p_tray->init(p_config, p_core, p_nodes);
#if !defined(Q_CC_MINGW) && !defined(Q_OS_MACOS)
    p_nodes->init(p_config, p_core, p_acolors, p_notifications);
    p_groups->init(p_config, p_acolors, p_nodes, p_curl, p_notifications);
#else
    p_nodes->init(p_config, p_core, p_acolors, p_notifications,
                  p_tray->getTrayIcon());
    p_groups->init(p_config, p_acolors, p_nodes, p_curl, p_notifications,
                   p_tray->getTrayIcon());
#endif
}

void Application::setTranslator(const QString &lang) {
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

void Application::registerModels() {
    const auto qml_model_name =
        QString("%1.%2").arg(ORG_NAME, APP_NAME).toStdString();
    qmlRegisterType<across::GroupModel>(qml_model_name.c_str(), 1, 0,
                                        "GroupModel");
    qmlRegisterType<across::NodeModel>(qml_model_name.c_str(), 1, 0,
                                       "NodeModel");
    qmlRegisterType<across::NodeFormModel>(qml_model_name.c_str(), 1, 0,
                                           "NodeFormModel");
    qmlRegisterType<across::Notification>(qml_model_name.c_str(), 1, 0,
                                          "Notification");
}

void Application::onMessageReceived(quint32 clientId, const QByteArray &msg) {
    if (clientId == instanceId())
        return;

    m_engine.rootContext()->setContextProperty(
        QStringLiteral("mainWindowVisible"), true);
}
