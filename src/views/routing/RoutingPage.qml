import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    implicitWidth: 840
    implicitHeight: 720

    property int fontSize: 14

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing

        clip: true
        spacing: acrossConfig.itemSpacing

        CardBox {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Label {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing * 4

                text: qsTr("Routing")

                font.pointSize: Math.round(fontSize * 1.2)
                color: acrossConfig.textColor
            }
        }
    }
}
