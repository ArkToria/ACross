import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic as ControlsBasic
//import QtQuick.Dialogs
import Qt.labs.platform

import Arktoria.ACross
import "../typescripts/home.js" as HomeJS

Window {
    id: nodeShareFormWindow
    width: 680
    height: 420
    minimumWidth: 680
    minimumHeight: 420
    title: qsTr("Share Configuration")

    flags: Qt.platform.os === "windows" ? Qt.Window : Qt.WindowStaysOnTopHint
    modality: Qt.ApplicationModal

    property var nodeModel: null
    property int fontSize: 14

    onVisibilityChanged: {
        if (!visible) {
            nodeShareFormWindow.close()
            nodeShareFormWindow.destroy()
        }
    }

    FileDialog {
        id: saveQRCodeDialog
        title: qsTr("Save QRCode to local path")

        defaultSuffix: "png"
        fileMode: FileDialog.SaveFile
        onAccepted: {
            acrossNodes.saveQRCodeToFile(nodeModel.nodeID, file)
        }
    }

    Rectangle {
        id: background
        anchors.fill: parent

        color: acrossConfig.deepColor

        CardBox {
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing

            RowLayout {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing * 4

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    color: "transparent"

                    SVGBox {
                        anchors.centerIn: parent

                        source: HomeJS.getQRCode(acrossNodes, nodeModel,
                                                 acrossConfig)
                        sourceWidth: parent.width / 1.2
                        sourceHeight: sourceWidth
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    GridLayout {
                        anchors.fill: parent
                        columns: 2

                        columnSpacing: acrossConfig.itemSpacing
                        rowSpacing: acrossConfig.itemSpacing

                        Label {
                            Layout.fillWidth: true
                            Layout.columnSpan: 2

                            text: qsTr("Node Info")
                            color: acrossConfig.textColor
                            font.pointSize: fontSize
                        }

                        Label {
                            text: qsTr("Name")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: nodeModel.name
                        }

                        Label {
                            text: qsTr("Address")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: nodeModel.address
                        }

                        Label {
                            text: qsTr("Port")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: nodeModel.port
                        }

                        Label {
                            text: qsTr("Password")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: nodeModel.password
                        }

                        Label {
                            Layout.fillWidth: true
                            Layout.columnSpan: 2

                            text: qsTr("Copy URL")
                            color: acrossConfig.textColor
                            font.pointSize: fontSize
                        }

                        ControlsBasic.ScrollView {
                            Layout.fillWidth: true
                            Layout.columnSpan: 2
                            Layout.fillHeight: true

                            TextAreaBox {
                                width: parent.availableWidth
                                height: parent.availableHeight

                                wrapMode: Text.WrapAnywhere
                                text: nodeModel.url
                                readOnly: true
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Layout.columnSpan: 2

                            Item {
                                Layout.fillWidth: true
                            }

                            ButtonBox {
                                text: qsTr("Save Image")
                                onClicked: {
                                    saveQRCodeDialog.open()
                                }
                            }

                            ButtonBox {
                                text: qsTr("Copy URL")
                                onClicked: {
                                    acrossNodes.copyURLToClipboard(
                                                nodeModel.name, nodeModel.url)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
