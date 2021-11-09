#include "logview.h"

using namespace across;

LogView::LogView(QObject *parent) : QObject(parent) {
    spdlog::init_thread_pool(QUEUE_SIZE, THREAD_NUMS);
    spdlog::flush_every(std::chrono::seconds(AUTO_FLUSH_TIME));

    std::vector<spdlog::sink_ptr> sinks;

    auto p_logger = std::make_shared<spdlog::async_logger>(
        "app", sinks.begin(), sinks.end(), spdlog::thread_pool(),
        spdlog::async_overflow_policy::block);

    if (spdlog::get("app") == nullptr) {
        auto core_logger = p_logger->clone("core");

        spdlog::register_logger(p_logger);
        spdlog::register_logger(core_logger);
    }
}

LogView::~LogView() {
    spdlog::drop_all();
    spdlog::shutdown();
}

void LogView::setTheme(const across::config::Theme &theme) {
    m_theme.Clear();
    m_theme.CopyFrom(theme);
}

void LogView::setLogItem(QQuickItem *item, const QString &name) {
    if (item == nullptr)
        return;

    if (auto logger = spdlog::get(name.toStdString()); logger != nullptr) {
        spdlog::sink_ptr qt_sink =
            std::make_shared<spdlog::sinks::qt_sink_mt>(item, "append");

        logger->sinks().emplace_back(qt_sink);
        logger->info("Adding qt sinks to logger: {}, Sinks size: {}",
                     logger->name(), logger->sinks().size());

        const auto p_item =
            qvariant_cast<QQuickTextDocument *>(item->property("textDocument"))
                ->textDocument();

        if (logger->name() == "core") {
            if (m_theme.IsInitialized()) {
                m_core_highlighter.setTheme(m_theme);
            }

            m_core_highlighter.init();
            m_core_highlighter.setDocument(p_item);
            return;
        } else if (logger->name() == "app") {
            if (m_theme.IsInitialized()) {
                m_app_highlighter.setTheme(m_theme);
            }

            m_app_highlighter.init();
            m_app_highlighter.setDocument(p_item);
            return;
        }
    }
}
