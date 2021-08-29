#ifndef SERIALIZETOOLS_H
#define SERIALIZETOOLS_H

#include <optional>
#include <QUrl>
#include <QUrlQuery>
#include <QString>
#include <QStringList>

#include "jsontools.h"

namespace across {
namespace config {
class SerializeTools
{
public:
    SerializeTools();

    static std::optional<SIP008::Server> sip002Decode(const QUrl& url);
    static std::optional<URLMetaObject> trojanDecode(const QUrl& url);
    static std::optional<URLMetaObject> vmessBase64Decode(const std::string &data);
    static std::optional<SIP008> sip008Parser(const std::stringstream &data_stream);
};
}
}


#endif // SERIALIZETOOLS_H
