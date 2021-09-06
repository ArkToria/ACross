import QtQuick 2.12
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import ACross 1.0

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

    property int textBoxWidth: 48

    Rectangle {
        id: background
        anchors.fill: parent

        color: acrossConfig.deepColor

        CardBox {
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing

            Row {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing

                Rectangle {
                    implicitWidth: parent.width / 2
                    implicitHeight: parent.height

                    color: "transparent"

                    SVGBox {
                        anchors.centerIn: parent

                        source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/qrcode.svg"
                        sourceWidth: 256
                        sourceHeight: 256
                    }
                }

                ColumnLayout {
                    implicitWidth: parent.width / 2
                    implicitHeight: parent.height

                    spacing: acrossConfig.itemSpacing

                    Label {
                        text: qsTr("Node Info")
                        color: acrossConfig.textColor
                        font.pixelSize: 18
                    }

                    RowLayout {
                        Label {
                            Layout.preferredWidth: textBoxWidth

                            text: qsTr("Name")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: name
                            readOnly: true
                        }
                    }

                    RowLayout {
                        Label {
                            Layout.preferredWidth: textBoxWidth

                            text: qsTr("Address")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: address
                            readOnly: true
                        }
                    }

                    RowLayout {
                        Label {
                            Layout.preferredWidth: textBoxWidth

                            text: qsTr("Port")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: port
                            readOnly: true
                        }
                    }

                    RowLayout {
                        Label {
                            Layout.preferredWidth: textBoxWidth

                            text: qsTr("Password")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true

                            text: password
                            readOnly: true
                        }
                    }

                    Label {
                        text: qsTr("Copy URL")
                        color: acrossConfig.textColor
                        font.pixelSize: 18
                    }

                    TextAreaBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }

                    RowLayout {
                        Layout.fillWidth: true

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
