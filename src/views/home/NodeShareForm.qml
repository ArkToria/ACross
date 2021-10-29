import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
//import QtQuick.Dialogs
import Qt.labs.platform

import ACross

Window {
    id: nodeShareFormWindow
    width: 680
    height: 420
    minimumWidth: 680
    minimumHeight: 420
    title: qsTr("Share Configuration")

    flags: Qt.WindowStaysOnTopHint
    modality: Qt.ApplicationModal

    property var nodeModel: null

    onVisibilityChanged: {
        if (!visible) {
            nodeShareFormWindow.close()
            nodeShareFormWindow.destroy()
        }
    }

    function getQRCode() {
        let name = acrossNodes.getQRCode(nodeModel.nodeID)
        if (name === "") {
            return "qrc:/misc/icons/" + acrossConfig.iconStyle + "/repo_qr_code.svg"
        } else {
            return "image://acrossImageProvider/" + name
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
                anchors.margins: acrossConfig.itemSpacing

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    color: "transparent"

                    SVGBox {
                        anchors.centerIn: parent

                        source: getQRCode()
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
                            font.pointSize: 14
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
                            font.pointSize: 14
                        }

                        TextAreaBox {
                            Layout.fillWidth: true
                            Layout.columnSpan: 2
                            Layout.fillHeight: true

                            wrapMode: Text.WrapAnywhere
                            text: nodeModel.url
                            readOnly: true
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
