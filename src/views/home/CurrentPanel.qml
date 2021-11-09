import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import ACross

Item {
    id: currentPanel
    implicitWidth: 648
    implicitHeight: 230

    function displayProtocolText(protocol) {
        switch (protocol) {
        case 0:
            return "vmess"
        case 1:
            return "shadowsocks"
        case 2:
            return "trojan"
        case 4:
            return "raw"
        case 5:
            return "scheme"
        case 6:
            return "unknown"
        }
    }

    Connections {
        target: acrossNodes

        function onCurrentNodeInfoChanged(nodeModel) {
            currentNodeName.text = nodeModel["name"]
            currentGroupText.text = nodeModel["group"]
            currentNodeProtocol.text = displayProtocolText(
                        nodeModel["protocol"])
            currentNodeAddress.text = nodeModel["address"]
            currentNodePort.text = nodeModel["port"]
        }
    }

    Item {
        anchors.fill: parent
        anchors.rightMargin: acrossConfig.itemSpacing

        CardBox {
            id: background
            anchors.fill: parent

            Image {
                id: backgroundImage
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing

                visible: acrossConfig.enableBanner

                source: acrossConfig.backgroundImage
                fillMode: Image.PreserveAspectCrop
                opacity: acrossConfig.backgroundOpacity > 0 ? acrossConfig.backgroundOpacity : 0

                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: mask
                }
            }

            Rectangle {
                id: mask
                anchors.fill: parent

                visible: false
                radius: parent.radius
            }

            GridLayout {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing * 2

                columnSpacing: acrossConfig.itemSpacing
                rowSpacing: acrossConfig.itemSpacing
                columns: 2
                rows: 2
                z: 2

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: parent.height / 3

                    Label {
                        id: currentNodeName
                        anchors.fill: parent
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.leftMargin: acrossConfig.itemSpacing
                        anchors.topMargin: acrossConfig.itemSpacing

                        color: acrossConfig.textColor
                        font.pointSize: 14
                        textFormat: Text.AutoText
                        wrapMode: Text.WrapAnywhere
                        elide: Text.ElideRight
                        maximumLineCount: 2
                    }
                }

                Item {
                    Layout.preferredWidth: parent.width / 5
                    Layout.preferredHeight: parent.height / 3

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.rightMargin: acrossConfig.itemSpacing

                        Item {
                            Layout.fillHeight: true
                        }

                        Label {
                            id: uploadText
                            text: "↑ " + acrossNodes.uploadTraffic + "/s"
                            Layout.alignment: Qt.AlignRight | Qt.AlignTop
                            color: acrossConfig.textColor
                        }

                        Rectangle {
                            implicitWidth: Math.max(uploadText.width,
                                                    downloadText.width)
                            Layout.preferredHeight: 2
                            color: acrossConfig.textColor
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        }

                        Label {
                            id: downloadText
                            text: "↓ " + acrossNodes.downloadTraffic + "/s"
                            Layout.alignment: Qt.AlignRight | Qt.AlignBottom
                            color: acrossConfig.textColor
                        }

                        Item {
                            Layout.fillHeight: true
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    GridLayout {
                        anchors.fill: parent
                        anchors.leftMargin: acrossConfig.itemSpacing
                        anchors.bottomMargin: acrossConfig.itemSpacing
                        columns: 2

                        Label {
                            text: "Group:"
                            color: acrossConfig.textColor
                            font.pointSize: 12
                        }

                        Label {
                            id: currentGroupText
                            color: acrossConfig.textColor
                            font.pointSize: 12
                        }

                        Label {
                            text: "Protocol:"
                            color: acrossConfig.textColor
                            font.pointSize: 12
                        }

                        Label {
                            id: currentNodeProtocol
                            color: acrossConfig.textColor
                            font.pointSize: 12
                        }

                        Label {
                            text: "Address:"
                            color: acrossConfig.textColor
                            font.pointSize: 12
                        }

                        Label {
                            id: currentNodeAddress
                            color: acrossConfig.textColor
                            font.pointSize: 12
                            textFormat: Text.AutoText
                            wrapMode: Text.WrapAnywhere
                            elide: Text.ElideRight
                            maximumLineCount: 2
                        }

                        Label {
                            text: "Port:"
                            color: acrossConfig.textColor
                            font.pointSize: 12
                        }

                        Label {
                            id: currentNodePort
                            color: acrossConfig.textColor
                            font.pointSize: 12
                        }
                    }
                }

                Item {
                    Layout.preferredWidth: parent.width / 5
                    Layout.fillHeight: true

                    CardBox {
                        id: stopButton
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.rightMargin: acrossConfig.itemSpacing
                        anchors.bottomMargin: acrossConfig.itemSpacing
                        implicitWidth: 64 + 2 * acrossConfig.itemSpacing
                        implicitHeight: width
                        z: 1

                        radius: Math.round(width / 2)

                        property color basicColor: acrossCore.isRunning ? acrossConfig.warnColor : acrossConfig.styleColor
                        color: basicColor

                        SVGBox {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: "qrc:/misc/icons/" + acrossConfig.iconStyle
                                    + (acrossCore.isRunning ? "/stop.svg" : "/play.svg")
                            sourceWidth: 48 + 2 * acrossConfig.itemSpacing
                            sourceHeight: sourceWidth
                        }

                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true

                            onEntered: {
                                cursorShape = Qt.PointingHandCursor
                                stopButton.color = Qt.binding(function () {
                                    return Qt.lighter(stopButton.basicColor,
                                                      1.1)
                                })
                            }

                            onExited: {
                                cursorShape = Qt.ArrowCursor
                                stopButton.color = Qt.binding(function () {
                                    return stopButton.basicColor
                                })
                            }

                            onClicked: {
                                if (acrossCore.isRunning) {
                                    acrossCore.stop()
                                } else {
                                    acrossCore.run()
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
