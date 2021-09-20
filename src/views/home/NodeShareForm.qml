import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls

import ACross

import "../components/"

Window {
    id: nodeShareFormWindow
    width: 680
    height: 420
    minimumWidth: 680
    minimumHeight: 420
    title: qsTr("Share Configuration")

    flags: Qt.WindowStaysOnTopHint
    modality: Qt.ApplicationModal

    property string name
    property string address
    property int port
    property string password
    property string url
    property string qrcode

    onVisibilityChanged: {
        if (!visible) {
            nodeShareFormWindow.close()
            nodeShareFormWindow.destroy()
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

                        source: qrcode === "" ? "qrc:/misc/icons/" + acrossConfig.iconStyle
                                                + "/repo_qr_code.svg" : qrcode
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
                            font.pixelSize: 18
                        }

                        Label {
                            text: qsTr("Name")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: name
                            readOnly: true
                        }

                        Label {
                            text: qsTr("Address")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: address
                            readOnly: true
                        }

                        Label {
                            text: qsTr("Port")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: port
                            readOnly: true
                        }

                        Label {
                            text: qsTr("Password")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: password
                            readOnly: true
                        }

                        Label {
                            Layout.fillWidth: true
                            Layout.columnSpan: 2

                            text: qsTr("Copy URL")
                            color: acrossConfig.textColor
                            font.pixelSize: 18
                        }

                        TextAreaBox {
                            Layout.fillWidth: true
                            Layout.columnSpan: 2
                            Layout.fillHeight: true

                            text: url
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Layout.columnSpan: 2

                            Item {
                                Layout.fillWidth: true
                            }

                            ButtonBox {
                                text: qsTr("Save")
                            }

                            ButtonBox {
                                text: qsTr("Copy")
                            }
                        }
                    }
                }
            }
        }
    }
}
