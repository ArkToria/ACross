import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    implicitWidth: 680
    implicitHeight: 260

    GridLayout {
        anchors.fill: parent

        columns: 4
        rowSpacing: acrossConfig.itemSpacing
        columnSpacing: acrossConfig.itemSpacing

        Label {
            Layout.fillWidth: true
            Layout.columnSpan: 4

            text: qsTr("Trojan Setting")
            font.pointSize: fontSize
            color: acrossConfig.textColor
        }

        Label {
            text: qsTr("Host")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: serverNameText
            Layout.fillWidth: true
            Layout.columnSpan: 3
        }

        Label {
            text: qsTr("Network")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: networkSelect
            Layout.fillWidth: true
            Layout.columnSpan: 3

            model: ["tcp"]
        }

        Label {
            text: qsTr("Security")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: securitySelect
            Layout.fillWidth: true
            Layout.columnSpan: 3

            model: ["tls", "none"]
        }

        Label {
            text: qsTr("ALPN")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: alpnSelect
            Layout.fillWidth: true
            Layout.columnSpan: 3

            model: ["h2+http/1.1", "h2", "http/1.1"]
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
