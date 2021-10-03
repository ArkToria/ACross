import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    id: groupControlBar
    implicitHeight: 36
    implicitWidth: 648

    property Component popMenuComponent: null
    property Component nodeShareFormComponent: null
    function openShareForm(nodeID, name, address, port, password, url) {
        if (nodeShareFormComponent == null) {
            nodeShareFormComponent = Qt.createComponent(
                        "qrc:/ACross/src/views/home/NodeShareForm.qml")
        }
        if (nodeShareFormComponent.status === Component.Ready) {
            var window = nodeShareFormComponent.createObject(mainWindow, {
                                                                 "nodeID": nodeID,
                                                                 "name": name,
                                                                 "address": address,
                                                                 "port": port,
                                                                 "password": password,
                                                                 "url": url
                                                             })
            window.show()
        }
    }

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
            implicitHeight: groupContorlCreate.height
            implicitWidth: groupContorlCreate.width
            layer.enabled: false
            color: "transparent"

            RowLayout {
                id: groupContorlCreate
                anchors.centerIn: parent

                SVGBox {
                    source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/add_circle.svg"
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
                    if (popMenuComponent == null) {
                        popMenuComponent = Qt.createComponent(
                                    "qrc:/ACross/src/views/home/NodeForm.qml")
                    }
                    if (popMenuComponent.status === Component.Ready) {
                        popMenuComponent.createObject(appendNodeButton).show()
                    }
                }
            }
        }

        CardBox {
            implicitHeight: groupContorlCopy.height
            implicitWidth: groupContorlCopy.width
            layer.enabled: false
            color: "transparent"

            RowLayout {
                id: groupContorlCopy
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

                onClicked: {
                    acrossNodes.copyUrlToClipboard(acrossNodes.currentNodeID)
                }
            }
        }

        CardBox {
            implicitHeight: groupContorlQRCode.height
            implicitWidth: groupContorlQRCode.width
            layer.enabled: false
            color: "transparent"

            RowLayout {
                id: groupContorlQRCode
                anchors.centerIn: parent

                SVGBox {
                    source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/qr_code.svg"
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

                onClicked: {
                    openShareForm(acrossNodes.currentNodeID,
                                  acrossNodes.currentNodeName,
                                  acrossNodes.currentNodeAddress,
                                  acrossNodes.currentNodePort,
                                  acrossNodes.currentNodePassword,
                                  acrossNodes.currentNodeURL)
                }
            }
        }

        CardBox {
            implicitHeight: groupContorlSearch.height
            implicitWidth: groupContorlSearch.width
            layer.enabled: false
            color: "transparent"

            RowLayout {
                id: groupContorlSearch
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
            implicitHeight: groupContorlLocate.height
            implicitWidth: groupContorlLocate.width
            layer.enabled: false
            color: "transparent"

            RowLayout {
                id: groupContorlLocate
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
    }
}
