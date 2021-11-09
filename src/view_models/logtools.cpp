#include "logtools.h"

using namespace across;
using namespace across::utils;

LogTools::LogTools(QSharedPointer<LogView> log_view, const QString &name,
                   LoggerEnum l_enum)
    : LogView(log_view.get()) {
    switch (l_enum) {
    case LoggerEnum::app:
        if (p_app_logger != nullptr)
            p_logger = p_app_logger->clone(name.toStdString());
        break;
    case LoggerEnum::core:
        if (p_core_logger != nullptr) {
            p_logger = p_core_logger->clone(name.toStdString());
        }
        break;
    default:
        break;
    }
}
