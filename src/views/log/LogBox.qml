import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Flickable {
    id: flick
    Layout.fillWidth: true
    Layout.fillHeight: true

    contentWidth: textEdit.paintedWidth
    contentHeight: textEdit.paintedHeight
    clip: true

    function getComponent() {
        return textEdit
    }

    function ensureVisible(r) {
        if (contentX >= r.x)
            contentX = r.x
        else if (contentX + width <= r.x + r.width)
            contentX = r.x + r.width - width
        if (contentY >= r.y)
            contentY = r.y
        else if (contentY + height <= r.y + r.height)
            contentY = r.y + r.height - height
    }

    TextEdit {
        id: textEdit
        width: flick.width

        property int maxLines: 100

        focus: true
        readOnly: true
        selectByMouse: true

        color: acrossConfig.deepTextColor
        selectedTextColor: acrossConfig.highlightTextColor
        selectionColor: acrossConfig.highlightColor

        font.family: "Mono"
        wrapMode: Text.NoWrap
        onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)

        //                onLineCountChanged: {
        //                    if (lineCount > maxLines) {
        //                        clear()
        //                    }
        //                }
    }
}
