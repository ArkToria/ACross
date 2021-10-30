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
    bool capture_groups = true;
    QRegularExpression pattern;
    QVector<QTextCharFormat> formats;
  };

  QVector<HighlightingRule> highlighting_rules;

  QTextCharFormat none_format;
  QTextCharFormat info_format;
  QTextCharFormat warning_format;
  QTextCharFormat highlight_format;
  QTextCharFormat date_format;
  QTextCharFormat time_format;
  QTextCharFormat logger_format;
  QTextCharFormat ip_host_format;
  QTextCharFormat tcp_udp_format;
};
}

#endif // LOGHIGHLIGHTER_H
