import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    id: nodeItemCard
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
                maximumLineCount: 2
                font.pixelSize: 16
                color: acrossConfig.textColor
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

                    source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/more_vert.svg"
                    sourceWidth: 24
                    sourceHeight: 24
                }
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

            Label {
                id: speedText
                Layout.fillWidth: true

                text: "↑ 128 M ↓ 2.4 GB"
                font.pixelSize: 14
                color: acrossConfig.textColor
            }

            Label {
                id: latencyText
                text: "32 ms"
                font.pixelSize: 14
                color: acrossConfig.styleColor
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

        onDoubleClicked: {
            if (mouse.button == Qt.LeftButton) {
                acrossNodes.setCurrentNode(id, index)

                nodeGridView.currentIndex = index
            }
        }

        property Component popMenuComponent: null

        onClicked: {
            if (mouse.button == Qt.RightButton) {
                if (popMenuComponent == null) {
                    popMenuComponent = Qt.createComponent(
                                "qrc:/src/views/home/NodeItemPopMenu.qml")
                }
                if (popMenuComponent.status === Component.Ready) {
                    popMenuComponent.createObject(nodeItemCard).popup()
                }
            }
        }
    }
}
