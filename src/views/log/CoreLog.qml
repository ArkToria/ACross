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
            anchors.fill: parent

            TextArea.flickable: TextAreaBox {
                Layout.fillHeight: true
                Layout.fillWidth: true

                color: acrossConfig.deepTextColor
                text: acrossLog.coreLog
                font.family: "Mono"
                wrapMode: Text.NoWrap
                placeholderText: "v2ray core logging ..."
            }

            ScrollBar.vertical: ScrollBar {}
            ScrollBar.horizontal: ScrollBar {}
        }
    }
}
