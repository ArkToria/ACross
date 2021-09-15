#include "logtools.h"

using namespace across;
using namespace across::utils;

LogTools::LogTools() {}

LogTools::LogTools(LogView& view, const QString& name)
  : LogView(&view)
{
  m_logger = p_logger->clone(name.toStdString());
}
