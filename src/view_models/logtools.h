#ifndef LOGTOOLS_H
#define LOGTOOLS_H

#include "logview.h"

#include <QString>

#include "fmt/format.h"
#include "spdlog/logger.h"
#include "spdlog/spdlog.h"

namespace across::utils {
enum LoggerEnum { core, app };

class LogTools : public LogView {
  public:
    explicit LogTools(QSharedPointer<LogView> log_view, const QString &name = "",
             LoggerEnum log_enum = LoggerEnum::app);

    template <typename... Args>
    inline void trace(fmt::format_string<Args...> fmt, Args &&...args) {
        p_logger->trace(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void debug(fmt::format_string<Args...> fmt, Args &&...args) {
        p_logger->debug(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void info(fmt::format_string<Args...> fmt, Args &&...args) {
        p_logger->info(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void warning(fmt::format_string<Args...> fmt, Args &&...args) {
        p_logger->warn(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void error(fmt::format_string<Args...> fmt, Args &&...args) {
        p_logger->error(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void critical(fmt::format_string<Args...> fmt, Args &&...args) {
        p_logger->critical(fmt, std::forward<Args>(args)...);
    }

  private:
    std::shared_ptr<spdlog::logger> p_logger;
};
} // namespace across
#endif // LOGTOOLS_H
