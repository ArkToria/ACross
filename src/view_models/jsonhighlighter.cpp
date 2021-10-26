#include "jsonhighlighter.h"

namespace across {
jsonHighlighter::jsonHighlighter(QTextDocument* parent)
  : QSyntaxHighlighter(parent)
{
  HighlightingRule rule;

  stringFormat.setForeground(Qt::red);
  keywordFormat.setForeground(Qt::blue);
  numberFormat.setForeground(Qt::blue);
  keyFormat.setBackground(Qt::black);
  keyquoteFormat.setBackground(Qt::black);
  valquoteFormat.setForeground(Qt::red);
  colonFormat.setForeground(Qt::cyan);

  // for string types
  rule.pattern =
    QRegularExpression(R"((")([^"]+)(")(\s*:)(\s*")((?:\\"|.)*?)("))");
  rule.formats =
    QVector<QTextCharFormat>{ keyquoteFormat, keyFormat,      keyquoteFormat,
                              colonFormat,    valquoteFormat, stringFormat,
                              valquoteFormat };
  highlightingRules.append(rule);

  // for string primitives
  rule.pattern = QRegularExpression(R"((^")((?:\\"|.)*?)("$))");
  rule.formats =
    QVector<QTextCharFormat>{ valquoteFormat, stringFormat, valquoteFormat };
  highlightingRules.append(rule);

  // for non-string types and string values (array of strings)
  rule.pattern = QRegularExpression(R"((")([^"]+)(")(\s*:\s*))");
  rule.formats = QVector<QTextCharFormat>{
    keyquoteFormat, keyFormat, keyquoteFormat, colonFormat
  };
  highlightingRules.append(rule);

  rule.pattern = QRegularExpression(R"(\b(true|false|null)\b)");
  rule.formats = QVector<QTextCharFormat>{ keywordFormat };
  highlightingRules.append(rule);

  // Number
  rule.pattern = QRegularExpression(R"(\b(0b|0x){0,1}[\d.']+\b)");
  rule.formats = QVector<QTextCharFormat>{ numberFormat };
  highlightingRules.append(rule);
}
void
jsonHighlighter::highlightBlock(const QString& text)
{
  for (const HighlightingRule& rule : qAsConst(highlightingRules)) {
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
