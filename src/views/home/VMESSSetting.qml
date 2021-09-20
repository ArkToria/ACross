import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "../components"

Item {
    implicitWidth: 680
    implicitHeight: 300

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

        GridLayout {
            columns: 2
            rowSpacing: acrossConfig.itemSpacing
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            Label {

                text: qsTr("AlterID")
                color: acrossConfig.textColor
            }

            RowLayout {
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

                    text: qsTr("Enable TLS")
                    color: acrossConfig.textColor
                }

                SwitchBox {
                    id: tlsEnableSelect
                }
            }

            Label {
                text: qsTr("Security")
                color: acrossConfig.textColor
            }

            RowLayout {
                DropDownBox {
                    id: securitySelect
                    Layout.fillWidth: true

                    model: ["auto", "aes-128-gcm", "chacha20-poly1305", "none", "zero"]
                }

                Label {

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

            Label {

                text: qsTr("Host")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                id: hostText
                Layout.fillWidth: true

                color: acrossConfig.textColor
            }

            Label {

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
