#ifndef NODEFORMMODEL_H
#define NODEFORMMODEL_H

#include "sodium.h"
#include <QObject>

#include "../models/dbtools.h"
#include "../models/jsontools.h"
#include "../models/serializetools.h"
#include "nodelist.h"
#include "rawoutboundformmodel.h"
#include "shadowsocksformmodel.h"
#include "trojanformmodel.h"
#include "urlschemeformmodel.h"
#include "vmessformmodel.h"

namespace across {

class NodeFormModel : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(
    QString address READ address WRITE setAddress NOTIFY addressChanged)
  Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
  Q_PROPERTY(
    QString password READ password WRITE setPassword NOTIFY passwordChanged)
  Q_PROPERTY(
    QString protocol READ protocol WRITE setProtocol NOTIFY protocolChanged)
  Q_PROPERTY(
    VMessFormModel* vmess READ vmess WRITE setVmess NOTIFY vmessChanged)
  Q_PROPERTY(
    TrojanFormModel* trojan READ trojan WRITE setTrojan NOTIFY trojanChanged)
  Q_PROPERTY(ShadowsocksFormModel* shadowsocks READ shadowsocks WRITE
               setShadowsocks NOTIFY shadowsocksChanged)
  Q_PROPERTY(RawOutboundFormModel* raw READ raw WRITE setRaw NOTIFY rawChanged)
  Q_PROPERTY(URLSchemeFormModel* urlScheme READ urlScheme WRITE setURLScheme
               NOTIFY urlSchemeChanged)
  Q_PROPERTY(NodeList* list READ list WRITE setList NOTIFY listChanged)
public:
  explicit NodeFormModel(QObject* parent = nullptr);

  Q_INVOKABLE void accept();

  Q_INVOKABLE void cancel();

public:
  const QString& name() const;

  const QString& address() const;

  int port() const;

  const QString& password() const;

  const QString& protocol() const;

  TrojanFormModel* trojan() const;

  NodeList* list() const;

  ShadowsocksFormModel* shadowsocks() const;

  VMessFormModel* vmess() const;

  RawOutboundFormModel* raw() const;

  URLSchemeFormModel* urlScheme() const;

public slots:
  void setName(const QString& newName);

  void setAddress(const QString& newAddress);

  void setPort(int newPort);

  void setPassword(const QString& newPassword);

  void setProtocol(const QString& newProtocol);

  void setVmess(VMessFormModel* newVmess);

  void setTrojan(TrojanFormModel* newTrojan);

  void setShadowsocks(ShadowsocksFormModel* newShadowsocks);

  void setRaw(RawOutboundFormModel* newRaw);

  void setURLScheme(URLSchemeFormModel* newUrlScheme);

  void setList(NodeList* newList);

signals:
  void nameChanged();

  void addressChanged();

  void portChanged();

  void passwordChanged();

  void protocolChanged();

  void trojanChanged();

  void listChanged();

  void shadowsocksChanged();

  void vmessChanged();

  void rawChanged();

  void urlSchemeChanged();

private:
  QString m_name = "";
  QString m_address = "";
  int m_port = 443;
  QString m_password = "";
  QString m_protocol = "vmess";

  NodeList* p_list;
  TrojanFormModel* p_trojan;
  ShadowsocksFormModel* p_shadowsocks;
  VMessFormModel* p_vmess;
  RawOutboundFormModel* p_raw;
  URLSchemeFormModel* p_urlScheme;

  bool setTrojanOutbound(NodeInfo& node);
  bool setShadowsocksOutbound(NodeInfo& node);
  bool setVMessOutboud(NodeInfo& node);
  bool setRawOutbound(NodeInfo& node);
  bool decodeOutboundFromURL(NodeInfo& node);
};
}

#endif // NODEFORMMODEL_H
