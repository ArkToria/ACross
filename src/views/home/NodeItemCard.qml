import QtQuick 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

Item {
    implicitWidth: 240
    implicitHeight: 192

    state: "NormalState"

    states: [
        State {
            name: "NormalState"
            PropertyChanges {
                target: background
                color: acrossConfig.backgroundColor
            }
            PropertyChanges {
                target: nodeNameText
                color: acrossConfig.textColor
            }
            PropertyChanges {
                target: addressText
                color: acrossConfig.textColor
            }
            PropertyChanges {
                target: protocolText
                color: acrossConfig.textColor
            }
            PropertyChanges {
                target: speedText
                color: acrossConfig.textColor
            }
            PropertyChanges {
                target: latencyText
                color: acrossConfig.textColor
            }
        },
        State {
            name: "HoverState"
            PropertyChanges {
                target: background
                color: Qt.lighter(acrossConfig.backgroundColor, 0.9)
            }
            PropertyChanges {
                target: nodeNameText
                color: acrossConfig.deepTextColor
            }
            PropertyChanges {
                target: addressText
                color: acrossConfig.deepTextColor
            }
            PropertyChanges {
                target: protocolText
                color: acrossConfig.deepTextColor
            }
            PropertyChanges {
                target: speedText
                color: acrossConfig.deepTextColor
            }
            PropertyChanges {
                target: latencyText
                color: acrossConfig.deepTextColor
            }
        }
    ]

    CardBox {
        id: background
        anchors.fill: parent
        layer.enabled: false

        RowLayout {
            anchors.fill: parent

            ColumnLayout {
                Layout.margins: 16
                Layout.fillWidth: true
                clip: true

                Label {
                    id: nodeNameText
                    Layout.fillWidth: true
                    clip: true

                    text: name
                    textFormat: Text.AutoText
                    wrapMode: Text.WrapAnywhere
                    elide: Text.ElideRight
                    maximumLineCount: 2
                    font.pixelSize: 16
                    color: acrossConfig.textColor
                }

                Item {
                    Layout.fillHeight: true
                }

                Label {
                    id: addressText
                    Layout.fillWidth: true

                    text: address
                    font.pixelSize: 14
                    color: acrossConfig.textColor
                }

                Label {
                    id: protocolText
                    Layout.fillWidth: true

                    text: protocol
                    font.pixelSize: 14
                    color: acrossConfig.textColor
                }

                RowLayout {
                    Layout.fillWidth: true

                    Label {
                        id: speedText
                        text: "↑ 128 M ↓ 2.4 GB"
                        font.pixelSize: 14
                        color: acrossConfig.textColor
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Label {
                        id: latencyText
                        text: "32 ms"
                        font.pixelSize: 14
                        color: acrossConfig.styleColor
                    }
                }
            }

            Item {
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.fillHeight: true
                Layout.preferredWidth: 24

                Rectangle {
                    id: popMenuArea
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 16
                    anchors.rightMargin: 8
                    implicitWidth: 24
                    implicitHeight: 24

                    color: "transparent"

                    SVGBox {
                        anchors.fill: parent

                        source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/more_vert.svg"
                        sourceWidth: 24
                        sourceHeight: 24
                    }
                }

                MouseArea {
                    anchors.fill: popMenuArea

                    onClicked: {

                    }
                }
            }
        }
    }

/*
    NodeItemPopMenu {
        id: nodeItemPopMenu
    }
    */

    MouseArea {
        anchors.fill: background

        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onEntered: {
            nodeItemCard.state = "HoverState"
        }

        onExited: {
            nodeItemCard.state = "NormalState"
        }

        onDoubleClicked: {
            if (mouse.button == Qt.LeftButton) {
                acrossNodes.setCurrentNode(id, index)

                nodeGridView.currentIndex = index
            }
        }

        onClicked: {
            if (mouse.button == Qt.RightButton) {
                var popMenuComponent = Qt.createComponent("qrc:/src/views/home/NodeItemPopMenu.qml")
                if(popMenuComponent.status == Component.Ready) {
                    var nodeItemPopMenu = popMenuComponent.createObject(parent);
                    nodeItemPopMenu.popup()
                }
            }
        }
    }
}
