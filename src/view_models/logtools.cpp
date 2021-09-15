#include "logtools.h"

using namespace across;
using namespace across::utils;

LogTools::LogTools(LogView& view, const QString& name)
  : LogView(&view)
{
  if (p_logger != nullptr) {
    m_logger = p_logger->clone(name.toStdString());
  }
}
