#include "app.h"

using namespace across;
using namespace across::core;
using namespace across::utils;
using namespace across::network;
using namespace across::setting;

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

    p_db = QSharedPointer<DBTools>::create();
    p_core = QSharedPointer<CoreTools>::create();
    p_curl = QSharedPointer<CURLTools>::create();
    p_nodes = QSharedPointer<NodeList>::create();
    p_groups = QSharedPointer<GroupList>::create();
    p_tray = QSharedPointer<SystemTray>::create();
    p_notifications = QSharedPointer<NotificationModel>::create();
    p_image_provider = new ImageProvider; // free by qml engine
    p_config->init(p_curl);

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

    return true;
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
    m_engine.rootContext()->setContextProperty(QStringLiteral("acrossNotifications"),
                                               p_notifications.get());
    m_engine.rootContext()->setContextProperty(
        QStringLiteral("fixedFont"),
        QFontDatabase::systemFont(QFontDatabase::FixedFont));
    m_engine.rootContext()->setContextProperty(
        QStringLiteral("mainWindowVisible"),
        !p_config->enableStartFromMinimized());
    m_engine.addImageProvider(QStringLiteral("acrossImageProvider"),
                              p_image_provider);
    m_engine.load(url);

    p_config->setLogMode();
    p_db->init(p_config->dbPath());
    p_core->init(p_config);
    p_tray->init(p_config, p_core, p_nodes);
#if !defined(Q_CC_MINGW) && !defined(Q_OS_MACOS)
    p_nodes->init(p_config, p_core, p_db);
    p_groups->init(p_config, p_db, p_nodes, p_curl, p_notifications);
#else
    p_nodes->init(p_config, p_core, p_db, p_tray->getTrayIcon());
    p_groups->init(p_config, p_db, p_nodes, p_curl, p_notifications, p_tray->getTrayIcon());
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
