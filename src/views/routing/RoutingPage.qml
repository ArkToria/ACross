import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Arktoria.ACross

Item {
    implicitWidth: 840
    implicitHeight: 720

    property int fontSize: 14

    GridLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing

        clip: true
        columns: 3
        columnSpacing: acrossConfig.itemSpacing

        CardBox {
            Layout.fillWidth: true
            Layout.columnSpan: 3
            Layout.preferredHeight: 128

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing * 4

                Label {
                    text: qsTr("Routing")
                    color: acrossConfig.textColor
                    font.pointSize: Math.round(fontSize * 1.2)
                }

                ListView {
                    id: ruleListView
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    clip: true
                    interactive: false
                    spacing: acrossConfig.itemSpacing * 4
                    model: ["Default Routing", "Bypass China", "AD Block"]
                    orientation: ListView.Horizontal

                    delegate: Label {
                        text: modelData
                        color: acrossConfig.textColor
                        font.pointSize: fontSize

                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                ruleListView.currentIndex = index
                            }
                        }
                    }

                    highlight: Rectangle {
                        color: acrossConfig.deepColor
                    }
                }
            }
        }

        RulePanel {
            Layout.fillWidth: true
            Layout.fillHeight: true

            title: qsTr("Direct")
        }

        RulePanel {
            Layout.fillWidth: true
            Layout.fillHeight: true

            title: qsTr("Proxy")
        }

        RulePanel {
            Layout.fillWidth: true
            Layout.fillHeight: true

            title: qsTr("Block")
        }
    }
}
