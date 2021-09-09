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
            text: qsTr("Core Log")

            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        Flickable {
            id: flickable
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea.flickable: TextAreaBox {
                text: acrossLog.coreLog
                placeholderText: "v2ray core logging ..."

                color: acrossConfig.deepTextColor
                selectedTextColor: acrossConfig.highlightTextColor
                selectionColor: acrossConfig.highlightColor

                font.family: "Mono"
                textFormat: TextEdit.RichText
                wrapMode: Text.NoWrap
            }

            ScrollBar.vertical: ScrollBar {}
            ScrollBar.horizontal: ScrollBar {}
        }
    }
}
