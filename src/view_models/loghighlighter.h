#ifndef LOGHIGHLIGHTER_H
#define LOGHIGHLIGHTER_H

#include <QColor>
#include <QMap>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QSharedPointer>

#include "across.grpc.pb.h"

namespace across {
class LogHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

  public:
    explicit LogHighlighter(QTextDocument *parent = nullptr);

    void init();

    void setTheme(const across::config::Theme &theme);

  protected:
    void highlightBlock(const QString &text) override;

  private:
    struct HighlightingRule {
        bool capture_groups = true;
        QRegularExpression pattern;
        QVector<QSharedPointer<QTextCharFormat>> formats;
    };

    QVector<HighlightingRule> highlighting_rules;

    QSharedPointer<QTextCharFormat> none_format;
    QSharedPointer<QTextCharFormat> info_format;
    QSharedPointer<QTextCharFormat> warning_format;
    QSharedPointer<QTextCharFormat> highlight_format;
    QSharedPointer<QTextCharFormat> date_format;
    QSharedPointer<QTextCharFormat> time_format;
    QSharedPointer<QTextCharFormat> logger_format;
    QSharedPointer<QTextCharFormat> ip_host_format;
    QSharedPointer<QTextCharFormat> tcp_udp_format;
};
} // namespace across

#endif // LOGHIGHLIGHTER_H
