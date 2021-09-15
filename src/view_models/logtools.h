#ifndef LOGTOOLS_H
#define LOGTOOLS_H

#include "logview.h"

#include <QString>

#include "fmt/format.h"
#include "spdlog/logger.h"
#include "spdlog/spdlog.h"

namespace across {
namespace utils {
class LogTools : public LogView
{
public:
  LogTools(LogView& view, const QString& name = "");

  template<typename... Args>
  inline void trace(fmt::format_string<Args...> fmt, Args&&... args)
  {
    m_logger->trace(fmt, std::forward<Args>(args)...);
  }

  template<typename... Args>
  inline void debug(fmt::format_string<Args...> fmt, Args&&... args)
  {
    m_logger->debug(fmt, std::forward<Args>(args)...);
  }

  template<typename... Args>
  inline void info(fmt::format_string<Args...> fmt, Args&&... args)
  {
    m_logger->info(fmt, std::forward<Args>(args)...);
  }

  template<typename... Args>
  inline void warn(fmt::format_string<Args...> fmt, Args&&... args)
  {
    m_logger->warn(fmt, std::forward<Args>(args)...);
  }

  template<typename... Args>
  inline void error(fmt::format_string<Args...> fmt, Args&&... args)
  {
    m_logger->error(fmt, std::forward<Args>(args)...);
  }

  template<typename... Args>
  inline void critical(fmt::format_string<Args...> fmt, Args&&... args)
  {
    m_logger->critical(fmt, std::forward<Args>(args)...);
  }

private:
  std::shared_ptr<spdlog::logger> m_logger;
};
}
}
#endif // LOGTOOLS_H
