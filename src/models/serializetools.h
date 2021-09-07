#ifndef SERIALIZETOOLS_H
#define SERIALIZETOOLS_H

#include "magic_enum.hpp"
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QUrlQuery>
#include <optional>

#include "../models/dbtools.h"
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
    static std::optional<SIP008> sip008Parser(
      const std::stringstream& data_stream);

    static bool decodeOutboundFromURL(NodeInfo& node, const QString& raw_url);
    static bool setShadowsocksOutboundFromURL(NodeInfo& node, const QUrl& url);
    static bool setVMessOutboundFromBase64(NodeInfo& node,
                                           const std::string& data);
    static bool setTrojanOutboundFromURL(NodeInfo& node, const QUrl& url);
};
}
}


#endif // SERIALIZETOOLS_H
