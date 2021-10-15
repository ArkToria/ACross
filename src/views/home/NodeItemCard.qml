import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    id: nodeItemCard
    implicitWidth: 240
    implicitHeight: 192

    function isCurrent() {
        if (acrossNodes.currentNodeGroup === group
                && acrossNodes.currentNodeID === Number(nodeID)
                && acrossCore.isRunning) {
            return true
        }
        return false
    }

    property Component popMenuComponent: null
    property color backgroundColor: isCurrent(
                                        ) ? acrossConfig.highlightColor : acrossConfig.backgroundColor
    property color textColor: isCurrent(
                                  ) ? acrossConfig.highlightTextColor : acrossConfig.textColor

    state: "NormalState"
    states: [
        State {
            name: "NormalState"
            PropertyChanges {
                target: background
                color: backgroundColor
            }
        },
        State {
            name: "HoverState"
            PropertyChanges {
                target: background
                color: Qt.lighter(backgroundColor, 0.97)
            }
        }
    ]

    CardBox {
        id: background
        anchors.fill: parent
        layer.enabled: false

        GridLayout {
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing
            columns: 2
            rows: 4
            clip: true

            Label {
                id: nodeNameText
                Layout.fillWidth: true
                clip: true

                text: name
                textFormat: Text.AutoText
                wrapMode: Text.WrapAnywhere
                elide: Text.ElideRight
                color: textColor
                maximumLineCount: 2
                font.pixelSize: 16
            }

            Rectangle {
                id: rectangle
                Layout.fillHeight: true
                Layout.rowSpan: 3

                color: "transparent"
                Layout.alignment: Qt.AlignRight | Qt.AlignTop

                SVGBox {
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.topMargin: acrossConfig.itemSpacing
                    anchors.rightMargin: acrossConfig.itemSpacing

                    source: isCurrent(
                                ) ? "qrc:/misc/icons/" + acrossConfig.iconStyle
                                    + (acrossConfig.iconStyle === "dark" ? "/more_vert.svg" : "/more_vert_reverse.svg") : "qrc:/misc/icons/"
                                    + acrossConfig.iconStyle + "/more_vert.svg"
                    sourceWidth: 24
                    sourceHeight: 24
                }
            }

            Label {
                id: addressText
                Layout.fillWidth: true

                text: address
                color: textColor
                font.pixelSize: 14
            }

            Label {
                id: protocolText
                Layout.fillWidth: true

                text: protocol
                color: textColor
                font.pixelSize: 14
            }

            Label {
                id: ruleText
                Layout.fillWidth: true

                text: "Bypass CN"
                font.pixelSize: 14
                color: textColor
            }

            Label {
                id: latencyDisplayText
                text: model.latency === -1 ? "-- ms" : model.latency + " ms"
                font.pixelSize: 14
                color: textColor
            }
        }
    }

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

        onDoubleClicked: function (mouse) {
            if (mouse.button === Qt.LeftButton) {
                acrossNodes.setCurrentNodeByID(nodeID)

                root.setIndex(index)
            }
        }

        onClicked: function (mouse) {
            if (mouse.button === Qt.RightButton) {
                if (popMenuComponent == null) {
                    popMenuComponent = Qt.createComponent(
                                "qrc:/ACross/src/views/home/NodeItemPopMenu.qml")
                }
                if (popMenuComponent.status === Component.Ready) {
                    popMenuComponent.createObject(nodeItemCard).popup()
                }
            }
        }
    }
}
