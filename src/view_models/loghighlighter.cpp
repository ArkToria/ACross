#include "loghighlighter.h"

namespace across {
LogHighlighter::LogHighlighter(QTextDocument* parent)
  : QSyntaxHighlighter(parent)
{}

void
LogHighlighter::init()
{
  HighlightingRule rule;
  // logger
  rule.pattern = QRegularExpression(R"((\[)([^\]]+)(\]))");
  rule.formats =
    QVector<QTextCharFormat>{ none_format, logger_format, none_format };
  highlighting_rules.append(rule);
  // date
  rule.pattern = QRegularExpression(R"((\d\d\d\d-\d\d-\d\d))");
  rule.formats = QVector<QTextCharFormat>{ date_format };
  highlighting_rules.append(rule);

  // time
  rule.pattern = QRegularExpression(R"((\d\d:\d\d:\d\d.\d\d\d))");
  rule.formats = QVector<QTextCharFormat>{ time_format };
  highlighting_rules.append(rule);

  // Modified From Qv2ray
  // https://github.com/Shadowsocks-NET/Qv2ray/blob/main/src/components/LogHighlighter/LogHighlighter.cpp
#define REGEX_IPV6_ADDR                                                        \
  R"(\[\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:)))(%.+)?\s*\])"
#define REGEX_IPV4_ADDR                                                        \
  R"((\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5]))"
#define REGEX_PORT_NUMBER                                                      \
  R"(([0-9]|[1-9]\d{1,3}|[1-5]\d{4}|6[0-5]{2}[0-3][0-5])*)"
  {
    rule.capture_groups = false;
    // IP IPv6 Host;
    rule.pattern = QRegularExpression(REGEX_IPV4_ADDR ":" REGEX_PORT_NUMBER);
    rule.pattern.setPatternOptions(
      QRegularExpression::ExtendedPatternSyntaxOption);
    rule.formats = QVector<QTextCharFormat>{ ip_host_format };
    highlighting_rules.append(rule);
    //
    rule.pattern = QRegularExpression(REGEX_IPV6_ADDR ":" REGEX_PORT_NUMBER);
    rule.pattern.setPatternOptions(
      QRegularExpression::ExtendedPatternSyntaxOption);
    rule.formats = QVector<QTextCharFormat>{ ip_host_format };
    highlighting_rules.append(rule);
    //
    rule.pattern =
      QRegularExpression("([a-zA-Z0-9]([a-zA-Z0-9\\-]{0,61}[a-zA-Z0-9])?\\.)+["
                         "a-zA-Z]{2,6}(/|):" REGEX_PORT_NUMBER);
    rule.pattern.setPatternOptions(
      QRegularExpression::PatternOption::ExtendedPatternSyntaxOption);
    rule.formats = QVector<QTextCharFormat>{ ip_host_format };
    highlighting_rules.append(rule);

    rule.capture_groups = true;
  }

  QList<QPair<QString, QTextCharFormat>> keywords{
    qMakePair("info", info_format),     qMakePair("warning", warning_format),
    qMakePair("error", warning_format), qMakePair("accepted", highlight_format),
    qMakePair("tcp", tcp_udp_format),   qMakePair("udp", tcp_udp_format)
  };

  for (auto&& keyword : keywords) {
    rule.pattern =
      QRegularExpression(QString(R"((\b%1\b))").arg(keyword.first));
    rule.formats = QVector<QTextCharFormat>{ keyword.second };
    highlighting_rules.append(rule);
  }
}

void
LogHighlighter::setTheme(const config::Theme& theme)
{
  auto colors = theme.colors();

  info_format.setForeground(QColor(colors.style_color().c_str()));
  warning_format.setForeground(QColor(colors.warn_color().c_str()));
  highlight_format.setForeground(QColor(colors.highlight_color().c_str()));
  ip_host_format.setForeground(QColor(colors.text_color().c_str()));
  logger_format.setForeground(QColor(colors.text_color().c_str()));
  date_format.setForeground(QColor(colors.text_color().c_str()));
  time_format.setForeground(QColor(colors.text_color().c_str()));
  tcp_udp_format.setForeground(QColor(colors.text_color().c_str()));
  tcp_udp_format.setFontWeight(QFont::Bold);
}

void
LogHighlighter::highlightBlock(const QString& text)
{
  for (const HighlightingRule& rule : qAsConst(highlighting_rules)) {
    QRegularExpressionMatchIterator matchIterator =
      rule.pattern.globalMatch(text);

    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      if (rule.capture_groups) {
        for (int index = 1; index <= rule.formats.size(); index++) {
          setFormat(match.capturedStart(index),
                    match.capturedLength(index),
                    rule.formats[index - 1]);
        }
      } else {
        setFormat(
          match.capturedStart(), match.capturedLength(), rule.formats[0]);
      }
    }
  }

  setCurrentBlockState(0);
}
}
