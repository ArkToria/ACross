import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

import ACross

Window {
    id: nodeEditFormPopWindow
    width: 960
    height: 680
    minimumWidth: 680
    minimumHeight: 420
    title: qsTr("Edit Configuration")

    flags: Qt.WindowStaysOnTopHint
    modality: Qt.ApplicationModal

    property var nodeModel: null
    property int fontSize: 14

    onVisibilityChanged: {
        if (!visible) {
            nodeEditFormPopWindow.close()
            nodeEditFormPopWindow.destroy()
        }
    }

    Rectangle {
        anchors.fill: parent
        color: acrossConfig.deepColor

        CardBox {
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing

            GridLayout {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing

                columns: 2
                rows: 3
                columnSpacing: acrossConfig.itemSpacing
                rowSpacing: acrossConfig.itemSpacing

                TextAreaBox {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.rowSpan: 3
                    Layout.maximumWidth: parent.width / 2

                    text: acrossNodes.jsonHighlighting(nodeModel.raw)
                    textFormat: Text.RichText
                }

                TabBar {
                    id: bar
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop

                    Repeater {
                        model: ["URL", "Manual", "Outbound"]

                        TabButton {
                            id: tabButton

                            contentItem: Text {
                                text: modelData
                                color: bar.currentIndex === model.index ? acrossConfig.textColor : acrossConfig.deepTextColor
                                font.pointSize: fontSize
                                horizontalAlignment: Text.AlignHCenter
                            }

                            background: Rectangle {
                                implicitHeight: 24
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

                StackLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop

                    currentIndex: bar.currentIndex

                    Item {
                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: acrossConfig.itemSpacing

                            Label {
                                text: qsTr("Import From URL")
                                font.pointSize: fontSize
                            }

                            TextFieldBox {
                                Layout.fillWidth: true
                                wrapMode: Text.WrapAnywhere
                                text: nodeModel.url
                                placeholderText: "support url scheme: ss:// vmess:// trojan://"
                            }

                            Item {
                                Layout.fillHeight: true
                            }
                        }
                    }
                    Rectangle {
                        Label {
                            text: "Manual"
                            font.pointSize: fontSize * 2
                        }
                    }
                    Rectangle {
                        Label {
                            text: "Outbound"
                            font.pointSize: fontSize * 2
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignBottom
                    spacing: acrossConfig.itemSpacing

                    Item {
                        Layout.fillWidth: true
                    }

                    ButtonBox {
                        id: acceptFormButton

                        text: qsTr("Accept")
                        onClicked: {

                        }
                    }

                    ButtonBox {
                        text: qsTr("Cancel")
                        basicColor: acrossConfig.warnColor
                        basicState: "WarnState"

                        onClicked: {
                            nodeEditFormPopWindow.close()
                        }
                    }
                }
            }
        }
    }
}
