import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import ACross 1.0
import "../components"

Item {
    id: groupControlBar
    implicitHeight: 36
    implicitWidth: 648

    RowLayout {
        anchors.leftMargin: acrossConfig.itemSpacing / 2
        anchors.rightMargin: acrossConfig.itemSpacing
        anchors.fill: parent
        spacing: acrossConfig.itemSpacing

        Item {
            Layout.fillWidth: true
        }

        CardBox {
            id: appendNodeButton
            implicitHeight: 32
            implicitWidth: 80
            layer.enabled: false
            color: "transparent"

            RowLayout {
                anchors.centerIn: parent

                SVGBox {
                    source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/append.svg"
                    sourceWidth: 22
                    sourceHeight: 22
                }

                Label {
                    text: qsTr("Create")
                    color: acrossConfig.textColor
                }
            }

            MouseArea {
                anchors.fill: parent

                hoverEnabled: true
                acceptedButtons: Qt.LeftButton

                onEntered: {
                    parent.color = acrossConfig.backgroundColor
                }

                onExited: {
                    parent.color = "transparent"
                }

                onClicked: {
                    var component = Qt.createComponent(
                                "qrc:/src/views/home/NodeForm.qml")
                    var window = component.createObject(appendNodeButton)
                    window.show()
                }
            }
        }

        CardBox {
            implicitHeight: 32
            implicitWidth: 80
            layer.enabled: false
            color: "transparent"

            RowLayout {
                anchors.centerIn: parent

                SVGBox {
                    source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/copy.svg"
                    sourceWidth: 22
                    sourceHeight: 22
                }

                Label {
                    text: qsTr("Copy")
                    color: acrossConfig.textColor
                }
            }

            MouseArea {
                anchors.fill: parent

                hoverEnabled: true
                acceptedButtons: Qt.LeftButton

                onEntered: {
                    parent.color = acrossConfig.backgroundColor
                }

                onExited: {
                    parent.color = "transparent"
                }
            }
        }

        CardBox {
            implicitHeight: 32
            implicitWidth: 80
            layer.enabled: false
            color: "transparent"

            RowLayout {
                anchors.centerIn: parent

                SVGBox {
                    source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/qrcode.svg"
                    sourceWidth: 22
                    sourceHeight: 22
                }

                Label {
                    text: qsTr("QRCode")
                    color: acrossConfig.textColor
                }
            }

            MouseArea {
                anchors.fill: parent

                hoverEnabled: true
                acceptedButtons: Qt.LeftButton

                onEntered: {
                    parent.color = acrossConfig.backgroundColor
                }

                onExited: {
                    parent.color = "transparent"
                }
            }
        }

        CardBox {
            implicitHeight: 32
            implicitWidth: 80
            layer.enabled: false
            color: "transparent"

            RowLayout {
                anchors.centerIn: parent

                SVGBox {
                    source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/select.svg"
                    sourceWidth: 22
                    sourceHeight: 22
                }

                Label {
                    text: qsTr("Select")
                    color: acrossConfig.textColor
                }
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onEntered: {
                    parent.color = acrossConfig.backgroundColor
                }

                onExited: {
                    parent.color = "transparent"
                }

                onClicked: {

                }
            }
        }

        CardBox {
            implicitHeight: 32
            implicitWidth: 80
            layer.enabled: false
            color: "transparent"

            RowLayout {
                anchors.centerIn: parent

                SVGBox {
                    source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/search.svg"
                    sourceWidth: 22
                    sourceHeight: 22
                }

                Label {
                    text: qsTr("Search")
                    color: acrossConfig.textColor
                }
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onEntered: {
                    parent.color = acrossConfig.backgroundColor
                }

                onExited: {
                    parent.color = "transparent"
                }
            }
        }

        CardBox {
            implicitHeight: 32
            implicitWidth: 80
            layer.enabled: false
            color: "transparent"

            RowLayout {
                anchors.centerIn: parent

                SVGBox {
                    source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/location.svg"
                    sourceWidth: 22
                    sourceHeight: 22
                }

                Label {
                    text: qsTr("Locate")
                    color: acrossConfig.textColor
                }
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onEntered: {
                    parent.color = acrossConfig.backgroundColor
                }

                onExited: {
                    parent.color = "transparent"
                }
            }
        }

        Item {
            Layout.fillWidth: true
        }
    }
}
