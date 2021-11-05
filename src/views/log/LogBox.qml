import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic as ControlsBasic

ControlsBasic.ScrollView {
    Layout.fillWidth: true
    Layout.fillHeight: true

    function getComponent() {
        return textEdit
    }

    Flickable {
        id: flick

        contentWidth: textEdit.paintedWidth
        contentHeight: textEdit.paintedHeight
        clip: true


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

            font: fixedFont
            wrapMode: Text.NoWrap
            onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)

            //                onLineCountChanged: {
            //                    if (lineCount > maxLines) {
            //                        clear()
            //                    }
            //                }
        }
    }
}
