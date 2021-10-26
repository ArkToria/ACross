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

    ShadowsocksFormModel {
        id: shadowsocksFormModel
    }

    TrojanFormModel {
        id: trojanFormModel
    }

    VMessFormModel {
        id: vmessFormModel
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
                    Layout.fillWidth: true
                    Layout.rowSpan: 3
                    Layout.maximumWidth: parent.width / 2

                    text: acrossNodes.jsonHighlighting(nodeModel.raw)
                    textFormat: Text.RichText
                    selectByMouse: true
                    readOnly: true
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
                        ColumnLayout {
                            anchors.fill: parent

                            TextFieldBox {
                                Layout.fillWidth: true
                                wrapMode: Text.WrapAnywhere
                                text: nodeModel.url
                                placeholderText: "support url scheme: ss:// vmess:// trojan://"
                            }

                            Item {
                                Layout.fillHeight: true
                            }
                        }
                    }

                    Item {
                        GridLayout {
                            anchors.fill: parent
                            columns: 4

                            rowSpacing: acrossConfig.itemSpacing
                            columnSpacing: acrossConfig.itemSpacing

                            Label {
                                Layout.fillWidth: true
                                Layout.columnSpan: 4

                                text: qsTr("Outbound")
                                color: acrossConfig.textColor
                                font.pointSize: fontSize
                            }

                            Label {
                                text: qsTr("Name")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                Layout.fillWidth: true
                                Layout.columnSpan: 3

                                placeholderText: nodeModel.name
                            }

                            Label {
                                text: qsTr("Address")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                Layout.fillWidth: true

                                placeholderText: nodeModel.address
                            }

                            Label {
                                text: qsTr("Port")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                placeholderText: nodeModel.port

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
                                Layout.fillWidth: true
                                Layout.columnSpan: 3

                                placeholderText: nodeModel.password
                            }

                            Label {
                                text: qsTr("Protocol")
                                color: acrossConfig.textColor
                            }

                            DropDownBox {
                                Layout.fillWidth: true
                                Layout.columnSpan: 3

                                model: ["vmess", "shadowsocks", "trojan"]
                                currentIndex: nodeModel.protocol

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
                            }

                            Item {
                                Layout.fillHeight: true
                            }
                        }
                    }

                    Item {
                        FileDialog {
                            id: importFileDialog
                            title: qsTr("Import v2ray config from file")
                            fileMode: FileDialog.OpenFile

                            nameFilters: ["v2ray config (*.conf)", "All files (*)"]
                            onAccepted: {

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
                                Layout.fillWidth: true
                                Layout.columnSpan: 3

                                placeholderText: nodeModel.name
                            }

                            Label {
                                text: qsTr("Config File")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                Layout.fillWidth: true
                                Layout.columnSpan: 2

                                placeholderText: qsTr("Enter Config Path Here")
                            }

                            ButtonBox {
                                text: qsTr("Open File")
                                onClicked: {

                                }
                            }

                            Item {
                                Layout.fillHeight: true
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
