import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import QtQuick.Dialogs

import ACross

Window {
    id: nodeEditFormPopWindow
    width: 960
    height: 680
    minimumWidth: 680
    minimumHeight: 420
    title: qsTr("Edit Configuration")

    flags: Qt.WindowStaysOnTopHint
    modality: Qt.ApplicationModal

    property var nodeModel: null
    property int fontSize: 14

    onVisibilityChanged: {
        if (!visible) {
            nodeEditFormPopWindow.close()
            nodeEditFormPopWindow.destroy()
        }
    }

    NodeFormModel {
        id: nodeFormModel
        list: acrossNodes
    }

    Rectangle {
        anchors.fill: parent
        color: acrossConfig.deepColor

        CardBox {
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing

            GridLayout {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing

                columns: 2
                rows: 3
                columnSpacing: acrossConfig.itemSpacing
                rowSpacing: acrossConfig.itemSpacing

                TextAreaBox {
                    id: jsonPreview
                    Layout.fillHeight: true
                    Layout.preferredWidth: parent.width / 2
                    Layout.rowSpan: 3

                    text: acrossNodes.jsonFormat(nodeModel.raw)
                    selectByMouse: true
                    readOnly: true

                    Component.onCompleted: {
                        acrossNodes.setDocument(jsonPreview.textDocument)
                    }
                }

                TabBar {
                    id: bar
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop

                    Repeater {
                        model: [qsTr("Decode URL"), qsTr(
                                "Manual Setting"), qsTr("Import Config")]

                        TabButton {
                            id: tabButton

                            contentItem: Text {
                                text: modelData
                                color: bar.currentIndex === model.index ? acrossConfig.textColor : acrossConfig.deepTextColor
                                horizontalAlignment: Text.AlignHCenter
                            }

                            background: Rectangle {
                                implicitHeight: 24
                                color: bar.currentIndex === model.index ? acrossConfig.backgroundColor : acrossConfig.deepColor
                            }

                            MouseArea {
                                anchors.fill: tabButton
                                hoverEnabled: true

                                onEntered: {
                                    cursorShape = Qt.PointingHandCursor
                                }

                                onExited: {
                                    cursorShape = Qt.ArrowCursor
                                }

                                onClicked: {
                                    bar.currentIndex = index
                                }
                            }
                        }
                    }
                }

                StackLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop

                    currentIndex: bar.currentIndex

                    Item {
                        id: urlSetting
                        ColumnLayout {
                            anchors.fill: parent

                            TextFieldBox {
                                id: urlTextField
                                Layout.fillWidth: true
                                wrapMode: Text.WrapAnywhere
                                //                                text: nodeModel.url
                                placeholderText: "support url scheme: ss:// vmess:// trojan://"
                            }

                            Item {
                                Layout.fillHeight: true
                            }
                        }

                        function accept() {
                            return urlTextField.text
                        }
                    }

                    Item {
                        id: manualSetting

                        GridLayout {
                            anchors.fill: parent
                            columns: 4

                            rowSpacing: acrossConfig.itemSpacing
                            columnSpacing: acrossConfig.itemSpacing

                            Label {
                                Layout.fillWidth: true
                                Layout.columnSpan: 4

                                text: qsTr("Outbound Config")
                                color: acrossConfig.textColor
                                font.pointSize: fontSize
                            }

                            Label {
                                text: qsTr("Name")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                id: manualNameText
                                Layout.fillWidth: true
                                Layout.columnSpan: 3

                                //                                placeholderText: nodeModel.name
                            }

                            Label {
                                text: qsTr("Address")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                id: manualAddressText
                                Layout.fillWidth: true

                                //                                placeholderText: nodeModel.address
                            }

                            Label {
                                text: qsTr("Port")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                id: manualPortText
                                Layout.minimumWidth: 72

                                //                                placeholderText: nodeModel.port
                                placeholderText: "443"
                                validator: IntValidator {
                                    bottom: 0
                                    top: 65535
                                }
                            }

                            Label {
                                text: qsTr("Password")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                id: manualPasswordText
                                Layout.fillWidth: true
                                Layout.columnSpan: 3

                                //                                placeholderText: nodeModel.password
                            }

                            Label {
                                text: qsTr("Protocol")
                                color: acrossConfig.textColor
                            }

                            DropDownBox {
                                id: manualProtocolType
                                Layout.fillWidth: true
                                Layout.columnSpan: 3

                                model: ["vmess", "shadowsocks", "trojan"]

                                //                                currentIndex: nodeModel.protocol
                                onEditTextChanged: {
                                    switch (currentIndex) {
                                    case 0:
                                        protocolSettingsLoader.source
                                                = "qrc:/ACross/src/views/home/VMESSSetting.qml"
                                        break
                                    case 1:
                                        protocolSettingsLoader.source = "qrc:/ACross/src/views/home/ShadowsocksSetting.qml"
                                        break
                                    case 2:
                                        protocolSettingsLoader.source
                                                = "qrc:/ACross/src/views/home/TrojanSetting.qml"
                                        break
                                    default:
                                        console.log("unknown protocol")
                                    }
                                }
                            }

                            Loader {
                                id: protocolSettingsLoader
                                Layout.fillWidth: true
                                Layout.columnSpan: 4

                                signal acceptAll
                                property var vmessSetting: null
                                property var shadowsocksSetting: null
                            }

                            Item {
                                Layout.fillHeight: true
                            }
                        }

                        function accept() {
                            protocolSettingsLoader.acceptAll()

                            return {
                                "name": manualNameText.text,
                                "address": manualAddressText.text,
                                "port": manualPortText.text,
                                "password": manualPasswordText.text,
                                "protocol": manualProtocolType.currentIndex,
                                "vmess": protocolSettingsLoader.vmessSetting,
                                "shadowsocks": protocolSettingsLoader.shadowsocksSetting
                            }
                        }
                    }

                    Item {
                        id: importConfigSetting
                        FileDialog {
                            id: importConfigDialog
                            title: qsTr("Import v2ray config from file")
                            fileMode: FileDialog.OpenFile

                            nameFilters: ["v2ray config (*.conf)", "All files (*)"]
                            onAccepted: {
                                importConfigPath.text = file
                            }
                        }

                        GridLayout {
                            anchors.fill: parent
                            columns: 4

                            Label {
                                text: qsTr("Node Name")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                id: importConfigName
                                Layout.fillWidth: true
                                Layout.columnSpan: 3

                                //                                placeholderText: nodeModel.name
                            }

                            Label {
                                text: qsTr("Config File")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                id: importConfigPath
                                Layout.fillWidth: true
                                Layout.columnSpan: 2

                                placeholderText: qsTr("Enter Config Path Here")
                            }

                            ButtonBox {
                                text: qsTr("Open File")
                                onClicked: {
                                    importConfigDialog.open()
                                }
                            }

                            Item {
                                Layout.fillHeight: true
                            }
                        }

                        function accept() {
                            return {
                                "name": importConfigName.text,
                                "filePath": importConfigPath.text
                            }
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignBottom
                    spacing: acrossConfig.itemSpacing

                    Item {
                        Layout.fillWidth: true
                    }

                    ButtonBox {
                        id: acceptFormButton

                        text: qsTr("Accept")
                        onClicked: {
                            let variantMap = {
                                "type": bar.currentIndex,
                                "url": urlSetting.accept(),
                                "manual": manualSetting.accept(),
                                "config": importConfigSetting.accept()
                            }
                            nodeFormModel.accept(variantMap)
                            nodeEditFormPopWindow.close()
                        }
                    }

                    ButtonBox {
                        text: qsTr("Cancel")
                        basicColor: acrossConfig.warnColor
                        basicState: "WarnState"

                        onClicked: {
                            nodeEditFormPopWindow.close()
                        }
                    }
                }
            }
        }
    }
}
