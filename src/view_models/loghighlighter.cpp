#include "loghighlighter.h"

namespace across {
LogHighlighter::LogHighlighter(QTextDocument* parent)
  : QSyntaxHighlighter(parent)
{}

void
LogHighlighter::init()
{
  infoFormat.setForeground(QColor("#94cbff"));
  warningFormat.setForeground(QColor("#ff716a"));
  highlightFormat.setForeground(QColor("#63a7d7"));
  loggerFormat.setForeground(Qt::darkGreen);
  dateFormat.setForeground(Qt::cyan);
  timeFormat.setForeground(Qt::cyan);

  HighlightingRule rule;
  // logger
  rule.pattern = QRegularExpression(R"((\[)([^\]]+)(\]))");
  rule.formats =
    QVector<QTextCharFormat>{ noneFormat, loggerFormat, noneFormat };
  highlighting_rules.append(rule);
  // date
  rule.pattern = QRegularExpression(R"((\d\d\d\d-\d\d-\d\d))");
  rule.formats = QVector<QTextCharFormat>{ dateFormat };
  highlighting_rules.append(rule);

  // time
  rule.pattern = QRegularExpression(R"((\d\d:\d\d:\d\d.\d\d\d))");
  rule.formats = QVector<QTextCharFormat>{ timeFormat };
  highlighting_rules.append(rule);

  QList<QPair<QString, QTextCharFormat>> keywords{
    qMakePair("info", infoFormat),
    qMakePair("warning", warningFormat),
    qMakePair("error", warningFormat),
    qMakePair("accepted", highlightFormat)
  };

  for (auto&& keyword : keywords) {
    rule.pattern =
      QRegularExpression(QString(R"((\b%1\b))").arg(keyword.first));
    rule.formats = QVector<QTextCharFormat>{ keyword.second };
    highlighting_rules.append(rule);
  }
}

void
LogHighlighter::highlightBlock(const QString& text)
{
  for (const HighlightingRule& rule : qAsConst(highlighting_rules)) {
    QRegularExpressionMatchIterator matchIterator =
      rule.pattern.globalMatch(text);

    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      for (int index = 1; index <= rule.formats.size(); index++) {
        setFormat(match.capturedStart(index),
                  match.capturedLength(index),
                  rule.formats[index - 1]);
      }
    }
  }

  setCurrentBlockState(0);
}
}
