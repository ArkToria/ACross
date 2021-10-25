import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls

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

                    TabButton {
                        text: qsTr("URL")
                    }
                    TabButton {
                        text: qsTr("Manual")
                    }
                    TabButton {
                        text: qsTr("Outbound")
                    }
                }

                StackLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop

                    currentIndex: bar.currentIndex
                    Rectangle {
                        Label {
                            text: "URL"
                            font.pointSize: fontSize * 2
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
