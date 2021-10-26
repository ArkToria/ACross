#ifndef JSONHIGHLIGHTER_H
#define JSONHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>

namespace across {
class jsonHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT

public:
  explicit jsonHighlighter(QTextDocument* parent = nullptr);

protected:
  void highlightBlock(const QString& text) override;

private:
  struct HighlightingRule
  {
    QRegularExpression pattern;
    QVector<QTextCharFormat> formats;
  };
  QVector<HighlightingRule> highlightingRules;

  QTextCharFormat keywordFormat;
  QTextCharFormat keyFormat;
  QTextCharFormat keyquoteFormat;
  QTextCharFormat valFormat;
  QTextCharFormat valquoteFormat;
  QTextCharFormat stringFormat;
  QTextCharFormat colonFormat;
  QTextCharFormat numberFormat;
};
}

#endif // JSONHIGHLIGHTER_H
