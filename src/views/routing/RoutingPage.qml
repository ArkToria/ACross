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
            Layout.preferredHeight: 48

            TabBar {
                id: bar
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing * 2

                Repeater {
                    model: routingModel

                    TabButton {
                        id: tabButton

                        contentItem: Text {
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter

                            text: name
                            color: bar.currentIndex === model.index ? acrossConfig.textColor : acrossConfig.deepTextColor
                            font.pointSize: fontSize
                        }

                        background: Rectangle {
                            implicitHeight: bar.availableHeight
                            color: bar.currentIndex === model.index ? acrossConfig.backgroundColor : acrossConfig.deepColor
                        }

                        MouseArea {
                            anchors.fill: tabButton
                            hoverEnabled: true

                            onEntered: {
                                cursorShape = Qt.PointingHandCursor
                            }

                            onExited: {
                                cursorShape = Qt.ArrowCursor
                            }

                            onClicked: {
                                bar.currentIndex = index
                            }
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                anchors.fill: parent
                clip: true

                model: ["0.0.0.0/8", "10.0.0.0/8", "fc00::/7", "fe80::/10", "geoip:cn", "geoip:!cn", "ext:customizedGeoIPFile.dat:cn", "ext:customizedGeoIPFile.dat:!cn", "ext-ip:customizedGeoIPFile.dat:cn", "ext-ip:customizedGeoIPFile.dat:!cn"]

                delegate: CardBox {
                    implicitWidth: parent.width
                    implicitHeight: Math.round(ruleText.height * 2)

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
                            text: modelData
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
