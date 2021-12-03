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

        CardBox {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                id: ruleListView
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing * 4
                clip: true

                spacing: acrossConfig.itemSpacing
                //                model: ["0.0.0.0/8", "10.0.0.0/8", "fc00::/7", "fe80::/10", "geoip:cn", "geoip:!cn", "ext:customizedGeoIPFile.dat:cn", "fc00::/7", "fe80::/10", "geoip:cn", "geoip:!cn", "ext:customizedGeoIPFile.dat:cn", "ext:customizedGeoIPFile.dat:!cn", "ext-ip:customizedGeoIPFile.dat:cn", "ext-ip:customizedGeoIPFile.dat:!cn"]
                model: ruleModel

                delegate: Item {
                    implicitWidth: parent.width
                    implicitHeight: Math.round(ruleText.height * 1.4)

                    RowLayout {
                        anchors.fill: parent
                        spacing: acrossConfig.itemSpacing * 2

                        Item {
                            implicitWidth: parent.spacing
                        }

                        Rectangle {
                            id: indicatorLight
                            implicitWidth: 8
                            implicitHeight: 8
                            radius: 4
                        }

                        TextFieldBox {
                            id: ruleText
                            text: raw
                            Layout.fillWidth: true
                        }

                        DropDownBox {
                            model: [qsTr("Direct"), qsTr(
                                    "Proxy"), qsTr("Block")]
                        }

                        Item {
                            implicitWidth: 48
                            Layout.fillHeight: true

                            RowLayout {
                                anchors.fill: parent
                                spacing: acrossConfig.itemSpacing

                                Item {
                                    Layout.fillWidth: true
                                }

                                SVGBox {
                                    Layout.alignment: Qt.AlignVCenter
                                    source: "qrc:/misc/icons/"
                                            + acrossConfig.iconStyle + "/remove.svg"
                                    sourceWidth: 24
                                    sourceHeight: 24
                                }

                                Item {
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }

                    function getColor(tag) {
                        if (tag.includes("geo")) {
                            indicatorLight.color = acrossConfig.highlightColor
                            return
                        }

                        if (tag.includes("ext")) {
                            indicatorLight.color = acrossConfig.warnColor
                            return
                        }

                        indicatorLight.color = acrossConfig.styleColor
                    }

                    Component.onCompleted: {
                        getColor(modelData)
                    }

                    Connections {
                        target: acrossConfig

                        function onCurrentThemeChanged() {
                            getColor(modelData)
                        }
                    }
                }
            }
        }
    }
}
