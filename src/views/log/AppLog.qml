import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

CardBox {
    implicitWidth: 680
    implicitHeight: 300

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 2

        spacing: acrossConfig.itemSpacing

        Label {
            text: qsTr("Application Log")

            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        Flickable {
            id: flick
            Layout.fillWidth: true
            Layout.fillHeight: true

            contentWidth: appLogTextEdit.paintedWidth
            contentHeight: appLogTextEdit.paintedHeight
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
                id: appLogTextEdit
                width: flick.width

                property int maxLines: 100

                focus: true
                readOnly: true
                selectByMouse: true

                color: acrossConfig.deepTextColor
                selectedTextColor: acrossConfig.highlightTextColor
                selectionColor: acrossConfig.highlightColor

                font.family: "Mono"
                textFormat: TextEdit.RichText
                wrapMode: Text.NoWrap
                onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)

                //                onLineCountChanged: {
                //                    if (lineCount > maxLines) {
                //                        clear()
                //                    }
                //                }
            }

            Component.onCompleted: {
                acrossLogView.appLogItem = appLogTextEdit
            }
        }
    }
}
