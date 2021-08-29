import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

import "../components"

Item {
    id: currentPanel
    implicitWidth: 648
    implicitHeight: 230

    CardBox {
        id: background
        anchors.fill: parent
        anchors.topMargin: acrossConfig.itemSpacing
        anchors.leftMargin: acrossConfig.itemSpacing / 2
        anchors.rightMargin: acrossConfig.itemSpacing

        RowLayout {
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing * 2

            ColumnLayout {
                Label {
                    Layout.fillWidth: true

                    text: acrossNodes.currentNodeName
                    color: acrossConfig.textColor
                    font.pointSize: 18
                    textFormat: Text.AutoText
                    wrapMode: Text.WrapAnywhere
                    elide: Text.ElideRight
                    maximumLineCount: 2
                }

                Item {
                    Layout.fillHeight: true
                }

                RowLayout {
                    Label {
                        text: "Group:"
                        color: acrossConfig.textColor
                        font.pointSize: 12
                    }

                    Label {
                        text: acrossNodes.currentNodeGroup
                        color: acrossConfig.textColor
                        font.pointSize: 12
                    }
                }

                RowLayout {
                    Label {
                        text: "Protocol:"
                        color: acrossConfig.textColor
                        font.pointSize: 12
                    }

                    Label {
                        text: acrossNodes.currentNodeProtocol
                        color: acrossConfig.textColor
                        font.pointSize: 12
                    }
                }
                RowLayout {
                    Label {
                        text: "Address:"
                        color: acrossConfig.textColor
                        font.pointSize: 12
                    }

                    Label {
                        text: acrossNodes.currentNodeAddress
                        color: acrossConfig.textColor
                        font.pointSize: 12
                        textFormat: Text.AutoText
                        wrapMode: Text.WrapAnywhere
                        elide: Text.ElideRight
                        maximumLineCount: 2
                    }
                }

                RowLayout {
                    Label {
                        text: "Port:"
                        color: acrossConfig.textColor
                        font.pointSize: 12
                    }

                    Label {
                        text: acrossNodes.currentNodePort
                        color: acrossConfig.textColor
                        font.pointSize: 12
                    }
                }
            }

            ColumnLayout {
                Item {
                    Layout.fillHeight: true
                }

                CardBox {
                    id: stopButton
                    implicitWidth: 64
                    implicitHeight: width

                    radius: Math.round(width / 2)
                    color: acrossCore.isRunning ? acrossConfig.warnColor : acrossConfig.styleColor

                    SVGBox {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: "qrc:/misc/icons/" + acrossConfig.iconStyle
                                + (acrossCore.isRunning ? "/stop.svg" : "/play.svg")
                        sourceWidth: 28
                        sourceHeight: sourceWidth
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true

                        onEntered: {
                            var currentColor = acrossCore.isRunning ? acrossConfig.warnColor : acrossConfig.styleColor
                            stopButton.color = Qt.lighter(currentColor, 1.1)
                        }

                        onExited: {
                            var currentColor = acrossCore.isRunning ? acrossConfig.warnColor : acrossConfig.styleColor
                            stopButton.color = currentColor
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
