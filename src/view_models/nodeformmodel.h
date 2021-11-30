#ifndef NODEFORMMODEL_H
#define NODEFORMMODEL_H

#include <QObject>

#include "../models/dbtools.h"
#include "../models/serializetools.h"
#include "nodelist.h"

namespace across {
class NodeFormModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(NodeList *list READ list WRITE setList NOTIFY listChanged)
  public:
    explicit NodeFormModel(QObject *parent = nullptr);

    Q_INVOKABLE void accept(const QVariantMap &values);
    Q_INVOKABLE QString refreshPreview(const QVariantMap &values);

  public:
    NodeList *list() const;

  public slots:
    void setList(across::NodeList *newList);

  signals:
    void listChanged();

  private:
    v2ray::config::V2rayConfig m_config;
    NodeList *p_list;

    bool manualSetting(NodeInfo &node, const QVariantMap &values);
    bool setTrojanOutbound(NodeInfo &node, const QVariantMap &values);
    bool setShadowsocksOutbound(NodeInfo &node, const QVariantMap &values);
    bool setVMessOutboud(NodeInfo &node, const QVariantMap &values);
    bool setRawOutbound(NodeInfo &node, const QVariantMap &values);
    bool decodeOutboundFromURL(NodeInfo &node, const QString &url_str);
};
} // namespace across

#endif // NODEFORMMODEL_H
