import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import Arktoria.ACross

Item {
    id: groupItemCard

    implicitWidth: 312
    implicitHeight: 96

    property int groupID: group_id

    state: groupItemCard.ListView.isCurrentItem ? "ClickState" : "NormalState"

    states: [
        State {
            name: "NormalState"
            PropertyChanges {
                target: background
                color: acrossConfig.backgroundColor
            }
            PropertyChanges {
                target: groupNameText
                color: acrossConfig.textColor
            }
            PropertyChanges {
                target: groupItemsText
                color: acrossConfig.textColor
            }
            PropertyChanges {
                target: groupUpdateText
                color: acrossConfig.textColor
            }
        },
        State {
            name: "HoverState"
            PropertyChanges {
                target: background
                color: Qt.lighter(acrossConfig.backgroundColor, 0.97)
            }
            PropertyChanges {
                target: groupNameText
                color: acrossConfig.deepTextColor
            }
            PropertyChanges {
                target: groupItemsText
                color: acrossConfig.deepTextColor
            }
            PropertyChanges {
                target: groupUpdateText
                color: acrossConfig.deepTextColor
            }
        },
        State {
            name: "ClickState"
            PropertyChanges {
                target: background
                color: "transparent"
            }
            PropertyChanges {
                target: groupNameText
                color: acrossConfig.highlightTextColor
            }
            PropertyChanges {
                target: groupItemsText
                color: acrossConfig.highlightTextColor
            }
            PropertyChanges {
                target: groupUpdateText
                color: acrossConfig.highlightTextColor
            }
        }
    ]

    Rectangle {
        id: background
        anchors.fill: parent

        RowLayout {
            anchors.fill: parent

            Rectangle {
                id: currentGroupTab
                Layout.preferredWidth: 8
                Layout.fillHeight: true

                color: acrossNodes.currentGroupID
                       === group_id ? acrossConfig.styleColor : "transparent"
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.margins: 8
                clip: true

                Label {
                    id: groupNameText
                    Layout.fillWidth: true

                    text: name
                    font.pixelSize: 18
                    color: acrossConfig.textColor
                }

                Label {
                    id: groupItemsText
                    Layout.fillWidth: true

                    text: qsTr("%n Items", "item count of group", items)
                    font.pixelSize: 14
                    color: acrossConfig.textColor
                }

                Label {
                    id: groupUpdateText
                    Layout.fillWidth: true

                    text: qsTr("Updated: %1").arg(modifiedAt)
                    font.pixelSize: 14
                    color: acrossConfig.textColor
                }
            }
        }

        MouseArea {
            anchors.fill: background

            hoverEnabled: true
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            property Component groupItemPopMenuComponent: null

            onEntered: {
                groupItemCard.state
                        = groupItemCard.ListView.isCurrentItem ? "ClickState" : "HoverState"
            }

            onExited: {
                groupItemCard.state
                        = groupItemCard.ListView.isCurrentItem ? "ClickState" : "NormalState"
            }

            onClicked: function (mouse) {
                if (mouse.button === Qt.RightButton) {
                    if (groupItemPopMenuComponent == null) {
                        groupItemPopMenuComponent = Qt.createComponent(
                                    "qrc:/Arktoria/ACross/src/views/home/GroupItemPopMenu.qml")
                    }
                    if (groupItemPopMenuComponent.status === Component.Ready) {
                        groupItemPopMenuComponent.createObject(parent).popup()
                    }
                }

                if (mouse.button === Qt.LeftButton) {

                    listScrollView.currentIndex = index

                    acrossGroups.setDisplayGroupID(group_id)
                }
            }
        }

        Connections {
            target: listScrollView

            function onCurrentIndexChanged() {
                if (listScrollView.currentIndex === index) {
                    groupItemCard.state = "ClickState"
                } else {
                    groupItemCard.state = "NormalState"
                }
            }
        }
    }
}
