import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic as ControlsBasic

ControlsBasic.ScrollView {
    Layout.fillWidth: true
    Layout.fillHeight: true

    property string itemName: "app"

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

            focus: true
            readOnly: true
            selectByMouse: true

            color: acrossConfig.deepTextColor
            selectedTextColor: acrossConfig.highlightTextColor
            selectionColor: acrossConfig.highlightColor

            font: fixedFont
            wrapMode: Text.WordWrap
            onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)

            onLineCountChanged: {
                let maxLines = acrossConfig.logLines
                if (maxLines <= 0) {

                    // unlimit
                } else if (lineCount >= maxLines) {
                    clear()
                }
            }

            Component.onCompleted: {
                acrossLogView.setLogItem(textEdit, itemName)
            }
        }
    }
}
