import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material as MaterialControls

import Arktoria.ACross
import "../typescripts/home.js" as HomeJS

Item {
    id: nodeItemCard
    implicitWidth: 240
    implicitHeight: 192

    property int fontSize: 12
    property Component popMenuComponent: null
    property bool isCurrentNode: HomeJS.isCurrentNode(acrossNodes, groupID,
                                                      nodeID, acrossCore)
    property color backgroundColor: isCurrentNode ? acrossConfig.highlightColor : acrossConfig.backgroundColor
    property color textColor: isCurrentNode ? acrossConfig.highlightTextColor : acrossConfig.textColor
    
    function openPopMenu() {
        if (popMenuComponent == null) {
            popMenuComponent = Qt.createComponent(
                        "qrc:/Arktoria/ACross/src/views/home/NodeItemPopMenu.qml")
        }

        if (popMenuComponent.status === Component.Ready) {
            popMenuComponent.createObject(nodeItemCard).popup()
        }
    }

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
            anchors.margins: acrossConfig.itemSpacing * 3
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
                font.pointSize: fontSize
            }

            Rectangle {
                id: rectangle
                Layout.fillHeight: true
                Layout.rowSpan: 3

                color: "transparent"
                Layout.alignment: Qt.AlignRight | Qt.AlignTop
            }

            Label {
                id: addressText
                Layout.fillWidth: true

                text: address
                color: textColor
            }

            Label {
                id: protocolText
                Layout.fillWidth: true

                text: HomeJS.displayProtocolText(protocol)
                color: textColor
            }

            Label {
                id: ruleText
                Layout.fillWidth: true

                text: routing
                color: textColor
            }

            Label {
                id: latencyDisplayText
                text: model.latency === -1 ? "-- ms" : model.latency + " ms"
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

                if (!acrossCore.isRunning) {
                    popNotify.notify(qsTr("Core Error"),
                                     qsTr("Failed to start the process"))
                }
            }
        }

        onClicked: function (mouse) {
            if (mouse.button === Qt.RightButton) {
                openPopMenu();
            }
        }
    }

    MaterialControls.ToolButton {
        id: menuButton

        anchors.top: background.top
        anchors.right: background.right
        anchors.topMargin: acrossConfig.itemSpacing*3
        anchors.rightMargin: acrossConfig.itemSpacing*3

        icon.source: isCurrentNode ? "qrc:/misc/icons/" + acrossConfig.iconStyle
                                + (acrossConfig.iconStyle === "dark" ? "/more_vert.svg" : "/more_vert_reverse.svg") : "qrc:/misc/icons/"
                                + acrossConfig.iconStyle + "/more_vert.svg"
        icon.width: 24
        icon.height: 24
        icon.color: acrossConfig.textColor

        onClicked: {
            openPopMenu()
        }
    }
}
