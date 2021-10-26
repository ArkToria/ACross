import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    implicitWidth: 680
    implicitHeight: 300

    property int fontSize: 14

    GridLayout {
        anchors.fill: parent

        columns: 4
        rowSpacing: acrossConfig.itemSpacing
        columnSpacing: acrossConfig.itemSpacing

        Label {
            Layout.fillWidth: true
            Layout.columnSpan: 4

            text: qsTr("VMESS Setting")
            font.pointSize: fontSize
            color: acrossConfig.textColor
        }

        Label {
            text: qsTr("Enable TLS")
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }

        SwitchBox {
            id: tlsEnableSelect
        }

        Label {
            text: qsTr("AlterID")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: alterIDText
            Layout.fillWidth: true
            Layout.columnSpan: 3

            placeholderText: "[0-65535] 0: auto enable VMessAEAD"
            validator: IntValidator {
                bottom: 0
                top: 65535
            }
        }

        Label {
            text: qsTr("Security")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: securitySelect
            Layout.fillWidth: true
            Layout.columnSpan: 3

            model: ["auto", "aes-128-gcm", "chacha20-poly1305", "none", "zero"]
        }

        Label {
            text: qsTr("Network")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: networkSelect
            Layout.fillWidth: true
            Layout.columnSpan: 3

            model: ["tcp", "kcp", "ws", "http", "domainsocket", "quic", "grpc"]
        }

        Label {
            text: qsTr("Host")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: hostText
            Layout.fillWidth: true
            Layout.columnSpan: 3

            color: acrossConfig.textColor
        }

        Label {
            text: qsTr("Path")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: pathText
            Layout.fillWidth: true
            Layout.columnSpan: 3

            color: acrossConfig.textColor
        }

        Item {
            Layout.fillHeight: true
        }
    }

    Connections {
        target: protocolSettingsLoader

        function onAcceptAll() {
            vmessSetting = {
                "enableTLS": tlsEnableSelect.checked,
                "alterID": alterIDText.text,
                "security": securitySelect.currentText,
                "network": networkSelect.currentText,
                "host": hostText.text,
                "path": pathText.text
            }
        }
    }
}
