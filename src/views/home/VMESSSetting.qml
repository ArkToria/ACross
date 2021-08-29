import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

Item {
    implicitWidth: 680
    implicitHeight: 300

    property int textBoxWidth: 72

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing
        spacing: 0

        Label {
            Layout.margins: acrossConfig.itemSpacing

            text: qsTr("VMESS Setting")
            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("AlterID")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                id: alterIDText
                Layout.fillWidth: true

                placeholderText: "[0-65535] 0: auto enable VMessAEAD"
                validator: IntValidator {
                    bottom: 0
                    top: 65535
                }
            }

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Enable TLS")
                color: acrossConfig.textColor
            }

            SwitchBox {
                id: tlsEnableSelect
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Security")
                color: acrossConfig.textColor
            }

            DropDownBox {
                id: securitySelect
                Layout.fillWidth: true

                model: ["auto", "aes-128-gcm", "chacha20-poly1305", "none", "zero"]
            }

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Network")
                horizontalAlignment: Text.AlignHCenter
                color: acrossConfig.textColor
            }

            DropDownBox {
                id: networkSelect
                Layout.fillWidth: true

                //              model: [ "ws", "quic", "tcp", "kcp", "http" ]
                model: ["ws"]
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Host")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                id: hostText
                Layout.fillWidth: true

                color: acrossConfig.textColor
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Path")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                id: pathText
                Layout.fillWidth: true

                color: acrossConfig.textColor
            }
        }
    }

    Connections {
        target: streamSettingLoader

        function onAcceptAll() {
            vmessFormModel.alterID = alterIDText.text
            vmessFormModel.tlsEnable = tlsEnableSelect.checked
            vmessFormModel.security = securitySelect.editText
            vmessFormModel.network = networkSelect.editText
            vmessFormModel.host = hostText.text
            vmessFormModel.path = pathText.text
        }
    }
}
