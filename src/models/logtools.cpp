#include "logtools.h"

using namespace across;

LogTools::LogTools()
{
}

LogTools::LogTools(std::shared_ptr<spdlog::details::thread_pool> thread_pool,
                   const std::string& name, LogMode mode)
    : m_name(name)
    , m_mode(mode)
{
    init(thread_pool);
}

void LogTools::init(std::shared_ptr<spdlog::details::thread_pool> thread_pool)
{
    std::vector<spdlog::sink_ptr> sinks;

    std::string log_file = m_path.append(m_filename);

    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        log_file, m_max_log_size, m_max_keep_file);

    switch (m_mode) {
    case LogMode::STDOUT:
        sinks.emplace_back(stdout_sink);
        break;
    case LogMode::FILE:
        sinks.emplace_back(rotating_sink);
        break;
    case LogMode::STDOUT_AND_FILE:
    default:
        sinks.emplace_back(stdout_sink);
        sinks.emplace_back(rotating_sink);
        break;
    }

    p_logger = std::make_shared<spdlog::async_logger>(
        m_name, sinks.begin(), sinks.end(),
        thread_pool, spdlog::async_overflow_policy::block);

    spdlog::register_logger(p_logger);
}

std::shared_ptr<spdlog::async_logger> LogTools::getLogger()
{
    return p_logger;
}

void LogTools::setLoggerName(const std::string& name)
{
    m_name = name;
}

void LogTools::setFileName(const std::string& name)
{
    m_filename = name;
}

void LogTools::setPath(const std::string& path)
{
    m_path = path;
}

void LogTools::setLogMode(LogMode mode)
{
    m_mode = mode;
}

void LogTools::setMaxKeepFile(int num)
{
    m_max_keep_file = num;
}

void LogTools::setMaxLogSize(int size)
{
    m_max_log_size = size;
}

