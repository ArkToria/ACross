import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    implicitWidth: 480
    implicitHeight: 224

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing
        spacing: 0

        GridLayout {
            columns: 2
            rowSpacing: acrossConfig.itemSpacing
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            Label {
                text: qsTr("Name")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                Layout.fillWidth: true

                onEditingFinished: {
                    nodeFormModel.name = text
                }
            }

            Label {
                text: qsTr("Address")
                color: acrossConfig.textColor
            }

            RowLayout {
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

            Label {
                text: qsTr("Password")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                Layout.fillWidth: true

                onEditingFinished: {
                    nodeFormModel.password = text
                }
            }

            Label {
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
                        streamSettingLoader.source = "qrc:/ACross/src/views/home/VMESSSetting.qml"
                        break
                    case "shadowsocks":
                        streamSettingLoader.source = "qrc:/ACross/src/views/home/ShadowsocksSetting.qml"
                        break
                    case "trojan":
                        streamSettingLoader.source = "qrc:/ACross/src/views/home/TrojanSetting.qml"
                        break
                    default:
                        console.log("unknown protocol")
                    }
                }
            }
        }
    }
}
