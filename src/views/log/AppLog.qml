import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

CardBox {
    implicitWidth: 680
    implicitHeight: 300

    property int fontSize: 14

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 3

        spacing: acrossConfig.itemSpacing * 2

        Label {
            text: qsTr("Application Log")

            font.pointSize: Math.round(fontSize * 1.2)
            color: acrossConfig.textColor
        }

        LogBox {
            itemName: "app"
        }
    }
}
