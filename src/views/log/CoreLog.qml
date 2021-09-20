import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

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

        LogBox {
            Component.onCompleted: {
                acrossLogView.coreLogItem = getComponent()
            }
        }
    }
}
