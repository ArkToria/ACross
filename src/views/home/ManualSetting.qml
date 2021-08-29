import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import ACross 1.0
import "../components"

Item {
    implicitWidth: 480
    implicitHeight: 224

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing
        spacing: 0

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Name")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                Layout.fillWidth: true

                onEditingFinished: {
                    nodeFormModel.name = text
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Address")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                Layout.fillWidth: true

                onEditingFinished: {
                    nodeFormModel.address = text
                }
            }

            Label {
                text: qsTr("Port")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                Layout.preferredWidth: textBoxWidth

                placeholderText: nodeFormModel.port
                validator: IntValidator {
                    bottom: 0
                    top: 65535
                }
                focus: true

                onEditingFinished: {
                    nodeFormModel.port = text
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Password")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                Layout.fillWidth: true

                onEditingFinished: {
                    nodeFormModel.password = text
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Protocol")
                color: acrossConfig.textColor
            }

            DropDownBox {
                id: protocolText
                Layout.fillWidth: true

                model: ["vmess", "shadowsocks", "trojan"]

                onEditTextChanged: {
                    nodeFormModel.protocol = editText

                    switch (editText) {
                    case "vmess":
                        streamSettingLoader.source = "qrc:/src/views/home/VMESSSetting.qml"
                        break
                    case "shadowsocks":
                        streamSettingLoader.source = "qrc:/src/views/home/ShadowsocksSetting.qml"
                        break
                    case "trojan":
                        streamSettingLoader.source = "qrc:/src/views/home/TrojanSetting.qml"
                        break
                    default:
                        console.log("unknown protocol")
                    }
                }
            }
        }
    }
}
