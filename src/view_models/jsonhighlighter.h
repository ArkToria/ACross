#ifndef JSONHIGHLIGHTER_H
#define JSONHIGHLIGHTER_H

#include <QColor>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>

#include "across.grpc.pb.h"

namespace across {
class JSONHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

  public:
    explicit JSONHighlighter(QTextDocument *parent = nullptr);

    void init();

    void setTheme(across::config::Theme *p_theme);

  protected:
    void highlightBlock(const QString &text) override;

  private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QVector<QTextCharFormat> formats;
    };

    QVector<HighlightingRule> highlighting_rules;

    QTextCharFormat keyword_format;
    QTextCharFormat key_format;
    QTextCharFormat keyquote_format;
    QTextCharFormat val_format;
    QTextCharFormat valquote_format;
    QTextCharFormat string_format;
    QTextCharFormat colon_format;
    QTextCharFormat number_format;
};
} // namespace across

#endif // JSONHIGHLIGHTER_H
