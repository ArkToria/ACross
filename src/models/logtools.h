#ifndef LOGTOOLS_H
#define LOGTOOLS_H

#include <memory>
#include <vector>

#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace across {

enum LogMode {
    STDOUT,
    FILE,
    STDOUT_AND_FILE,
};

class LogTools {
public:
    explicit LogTools();

    explicit LogTools(std::shared_ptr<spdlog::details::thread_pool> thread_pool, const std::string& name, LogMode mode = LogMode::STDOUT);

    void init(std::shared_ptr<spdlog::details::thread_pool> thread_pool);

    void setLoggerName(const std::string& name);

    void setFileName(const std::string& name);

    void setPath(const std::string& path);

    void setLogMode(LogMode mode);

    void setMaxKeepFile(int num);

    void setThreadNums(int num);

    void setMaxLogSize(int size);

    void setQueueSize(int size);

    std::shared_ptr<spdlog::async_logger> getLogger();

    template <typename... Args>
    inline void trace(fmt::format_string<Args...> fmt, Args&&... args)
    {
        p_logger->trace(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void debug(fmt::format_string<Args...> fmt, Args&&... args)
    {
        p_logger->debug(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void info(fmt::format_string<Args...> fmt, Args&&... args)
    {
        p_logger->info(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void warn(fmt::format_string<Args...> fmt, Args&&... args)
    {
        p_logger->warn(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void error(fmt::format_string<Args...> fmt, Args&&... args)
    {
        p_logger->error(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void critical(fmt::format_string<Args...> fmt, Args&&... args)
    {
        p_logger->critical(fmt, std::forward<Args>(args)...);
    }

private:
    std::shared_ptr<spdlog::async_logger> p_logger;

    std::string m_name = "main";
    std::string m_filename = "across_log.txt";
    std::string m_path = "./logs/";
    LogMode m_mode;

    int m_max_keep_file = 3;
    int m_max_log_size = 1024 * 1024 * 4;
};
};

#endif // LOGTOOLS_H
