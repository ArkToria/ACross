import "../typescripts/log.js" as LogJS
import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic as ControlsBasic
import QtQuick.Layouts

ControlsBasic.ScrollView {
    property string itemName: "app"

    function clear() {
        textEdit.clear();
        acrossLogView.clearUndoStacks(textEdit);
    }

    Layout.fillWidth: true
    Layout.fillHeight: true

    Flickable {
        id: flick

        contentWidth: textEdit.paintedWidth
        contentHeight: textEdit.paintedHeight
        clip: true

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
            onCursorRectangleChanged: {
                LogJS.ensureVisible(flick, cursorRectangle);
            }
            onLineCountChanged: {
                LogJS.autoClean(textEdit, acrossConfig.logLines);
            }
            Component.onCompleted: {
                acrossLogView.setLogItem(textEdit, itemName);
            }
        }

    }

}
