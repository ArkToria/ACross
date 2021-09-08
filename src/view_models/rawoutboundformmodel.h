#ifndef RAWOUTBOUNDFORMMODEL_H
#define RAWOUTBOUNDFORMMODEL_H

#include <QObject>
namespace across {
class RawOutboundFormModel : public QObject
{
  Q_OBJECT
  Q_PROPERTY(
    QString rawText READ rawText WRITE setRawText NOTIFY rawTextChanged)
  QString m_rawText;

public:
  explicit RawOutboundFormModel(QObject* parent = nullptr);

  const QString& rawText() const;
  void setRawText(const QString& newRawText);

signals:

  void rawTextChanged();
};
}

#endif // RAWOUTBOUNDFORMMODEL_H
