import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Arktoria.ACross

CardBox {
    implicitWidth: 680
    implicitHeight: 300

    property int fontSize: 14

    GridLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 3

        columns: 3
        columnSpacing: acrossConfig.itemSpacing * 2
        rowSpacing: acrossConfig.itemSpacing * 2

        Label {
            text: qsTr("Application Log")

            font.pointSize: Math.round(fontSize * 1.2)
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillWidth: true
        }

        ButtonBox {
            text: qsTr("Clear")

            onClicked: {
                logBox.clear()
            }
        }

        LogBox {
            id: logBox

            Layout.fillWidth: true
            Layout.columnSpan: 3
            Layout.fillHeight: true

            itemName: "app"
        }
    }
}
