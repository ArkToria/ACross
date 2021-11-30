#include "jsonhighlighter.h"

namespace across {
JSONHighlighter::JSONHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {}

void JSONHighlighter::init() {
    HighlightingRule rule;

    // for non-string types and string values (array of strings)
    rule.pattern = QRegularExpression(R"((")([^"]+)(")(\s*:\s*))");
    rule.formats = QVector<QTextCharFormat>{keyquote_format, key_format,
                                            keyquote_format, colon_format};
    highlighting_rules.append(rule);

    rule.pattern = QRegularExpression(R"(\b(true|false|null)\b)");
    rule.formats = QVector<QTextCharFormat>{keyword_format};
    highlighting_rules.append(rule);

    // Number
    rule.pattern = QRegularExpression(R"(\b(0b|0x){0,1}[\d.']+\b)");
    rule.formats = QVector<QTextCharFormat>{number_format};
    highlighting_rules.append(rule);

    // for string types
    rule.pattern =
        QRegularExpression(R"((")([^"]+)(")(\s*:)(\s*")((?:\\"|.)*?)("))");
    rule.formats = QVector<QTextCharFormat>{
        keyquote_format, key_format,    keyquote_format, colon_format,
        valquote_format, string_format, valquote_format};
    highlighting_rules.append(rule);

    // for string primitives
    rule.pattern = QRegularExpression(R"((^")((?:\\"|.)*?)("$))");
    rule.formats = QVector<QTextCharFormat>{valquote_format, string_format,
                                            valquote_format};
    highlighting_rules.append(rule);
}

void JSONHighlighter::setTheme(config::Theme *p_theme) {
    const auto& colors = p_theme->colors();

    val_format.setForeground(QColor(colors.text_color().c_str()));
    string_format.setForeground(QColor(colors.text_color().c_str()));
    colon_format.setForeground(QColor(colors.highlight_color().c_str()));
    keyword_format.setForeground(QColor(colors.highlight_color().c_str()));
    key_format.setForeground(QColor(colors.highlight_color().c_str()));
    keyquote_format.setForeground(QColor(colors.highlight_color().c_str()));
    number_format.setForeground(QColor(colors.style_color().c_str()));
}

void JSONHighlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : qAsConst(highlighting_rules)) {
        QRegularExpressionMatchIterator matchIterator =
            rule.pattern.globalMatch(text);

        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            for (int index = 1; index <= rule.formats.size(); index++) {
                setFormat(match.capturedStart(index),
                          match.capturedLength(index), rule.formats[index - 1]);
            }
        }
    }

    setCurrentBlockState(0);
}
} // namespace across
