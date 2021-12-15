import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Arktoria.ACross

Item {
    implicitWidth: 840
    implicitHeight: 720

    property int fontSize: 14

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing

        clip: true
        spacing: acrossConfig.itemSpacing

        RoutingModel {
            id: routingModel
            list: acrossRoutings
        }

        RuleModel {
            id: ruleModel
        }

        CardBox {
            Layout.fillWidth: true
            Layout.preferredHeight: 128

            GridLayout {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing * 4

                columns: 6
                rowSpacing: acrossConfig.itemSpacing * 2
                columnSpacing: acrossConfig.itemSpacing * 2

                Label {
                    Layout.fillWidth: true
                    Layout.columnSpan: 6

                    text: qsTr("Routing")
                    color: acrossConfig.textColor
                    font.pointSize: Math.round(fontSize * 1.2)
                }

                Label {
                    text: qsTr("Transparent Proxy")
                    color: acrossConfig.textColor
                    font.pointSize: fontSize
                }

                Item {
                    Layout.fillWidth: true
                }

                SwitchBox {}

                Label {
                    text: qsTr("Environment Variables")
                    color: acrossConfig.textColor
                    font.pointSize: fontSize
                }

                Item {
                    Layout.fillWidth: true
                }

                SwitchBox {}
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 64

            RowLayout {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing
                spacing: acrossConfig.itemSpacing * 2

                ListView {
                    id: routingListView
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    clip: true

                    currentIndex: 0

                    model: routingModel
                    orientation: ListView.Horizontal

                    delegate: CardBox {
                        id: tabButton
                        width: routingListView.count
                               !== 0 ? Math.max(
                                           Math.round(
                                               routingNameText.implicitWidth
                                               + acrossConfig.itemSpacing * 8),
                                           Math.round(
                                               routingListView.width
                                               / routingListView.count)) : routingListView.width
                        height: routingListView.height
                        color: routingListView.currentIndex === model.index ? acrossConfig.highlightColor : acrossConfig.backgroundColor

                        Text {
                            id: routingNameText
                            anchors.fill: parent
                            anchors.centerIn: parent.Center
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter

                            text: name
                            color: routingListView.currentIndex === model.index ? acrossConfig.highlightTextColor : acrossConfig.textColor
                            font.pointSize: fontSize
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true

                            onEntered: {
                                cursorShape = Qt.PointingHandCursor
                            }

                            onExited: {
                                cursorShape = Qt.ArrowCursor
                            }

                            onClicked: {
                                routingListView.currentIndex = index
                                ruleModel.raw = raw
                            }
                        }

                        Component.onCompleted: {
                            if (routingListView.currentIndex === model.index) {
                                ruleModel.raw = raw
                            }
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 48

                    SVGBox {
                        anchors.right: parent.right
                        anchors.rightMargin: acrossConfig.itemSpacing * 6
                        anchors.verticalCenter: parent.verticalCenter

                        source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/add_circle.svg"
                        sourceWidth: 24
                        sourceHeight: 24
                    }
                }
            }
        }

        RulePanel {
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
        }
    }
}
