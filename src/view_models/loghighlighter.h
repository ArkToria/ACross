#ifndef LOGHIGHLIGHTER_H
#define LOGHIGHLIGHTER_H

#include <QColor>
#include <QMap>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>

#include "across.grpc.pb.h"

namespace across {
class LogHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT

public:
  explicit LogHighlighter(QTextDocument* parent = nullptr);

  void init();

protected:
  void highlightBlock(const QString& text) override;

private:
  struct HighlightingRule
  {
    QRegularExpression pattern;
    QVector<QTextCharFormat> formats;
  };

  QVector<HighlightingRule> highlighting_rules;

  QTextCharFormat noneFormat;
  QTextCharFormat infoFormat;
  QTextCharFormat warningFormat;
  QTextCharFormat highlightFormat;
  QTextCharFormat dateFormat;
  QTextCharFormat timeFormat;
  QTextCharFormat loggerFormat;
};
}

#endif // LOGHIGHLIGHTER_H
