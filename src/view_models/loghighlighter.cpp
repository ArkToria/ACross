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
  ipHostFormat.setForeground(Qt::yellow);
  loggerFormat.setForeground(Qt::darkGreen);
  dateFormat.setForeground(Qt::cyan);
  timeFormat.setForeground(Qt::cyan);

  tcpudpFormat.setForeground(QColor(0, 52, 130));
  tcpudpFormat.setFontWeight(QFont::Bold);

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

  // Modified From Qv2ray https://github.com/Shadowsocks-NET/Qv2ray/blob/main/src/components/LogHighlighter/LogHighlighter.cpp
#define REGEX_IPV6_ADDR                                                                                                                                                  \
    R"(\[\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:)))(%.+)?\s*\])"
#define REGEX_IPV4_ADDR R"((\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5]))"
#define REGEX_PORT_NUMBER R"(([0-9]|[1-9]\d{1,3}|[1-5]\d{4}|6[0-5]{2}[0-3][0-5])*)"
  {
      rule.capture_groups = false;
      // IP IPv6 Host;
      rule.pattern = QRegularExpression(REGEX_IPV4_ADDR ":" REGEX_PORT_NUMBER);
      rule.pattern.setPatternOptions(QRegularExpression::ExtendedPatternSyntaxOption);
      rule.formats = QVector<QTextCharFormat>{ ipHostFormat };
      highlighting_rules.append(rule);
      //
      rule.pattern = QRegularExpression(REGEX_IPV6_ADDR ":" REGEX_PORT_NUMBER);
      rule.pattern.setPatternOptions(QRegularExpression::ExtendedPatternSyntaxOption);
      rule.formats = QVector<QTextCharFormat>{ ipHostFormat };
      highlighting_rules.append(rule);
      //
      rule.pattern = QRegularExpression("([a-zA-Z0-9]([a-zA-Z0-9\\-]{0,61}[a-zA-Z0-9])?\\.)+[a-zA-Z]{2,6}(/|):" REGEX_PORT_NUMBER);
      rule.pattern.setPatternOptions(QRegularExpression::PatternOption::ExtendedPatternSyntaxOption);
      rule.formats = QVector<QTextCharFormat>{ ipHostFormat };
      highlighting_rules.append(rule);

      rule.capture_groups = true;
  }

  QList<QPair<QString, QTextCharFormat>> keywords{
    qMakePair("info", infoFormat),
    qMakePair("warning", warningFormat),
    qMakePair("error", warningFormat),
    qMakePair("accepted", highlightFormat),
    qMakePair("tcp", tcpudpFormat),
    qMakePair("udp", tcpudpFormat)
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
      if(rule.capture_groups){
        for (int index = 1; index <= rule.formats.size(); index++) {
          setFormat(match.capturedStart(index),
                    match.capturedLength(index),
                    rule.formats[index - 1]);
        }
      } else {
        setFormat(match.capturedStart(), match.capturedLength(), rule.formats[0]);
      }
    }
  }

  setCurrentBlockState(0);
}
}
