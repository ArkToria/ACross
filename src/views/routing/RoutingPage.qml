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

            Label {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing * 4

                text: qsTr("Routing")
                color: acrossConfig.textColor
                font.pointSize: Math.round(fontSize * 1.2)
            }
        }

        CardBox {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing * 4

                Label {
                    Layout.fillWidth: true

                    text: qsTr("Direct")

                    font.pointSize: fontSize
                    color: acrossConfig.textColor
                }

                ListView {
                    id: directRuleList
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    model: 10
                    delegate: RuleItemCard {
                        width: directRuleList.width
                    }
                }
            }
        }

        CardBox {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing * 4

                Label {
                    Layout.fillWidth: true

                    text: qsTr("Proxy")

                    font.pointSize: fontSize
                    color: acrossConfig.textColor
                }

                ListView {
                    id: proxyRuleList
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    model: 10
                    delegate: RuleItemCard {
                        width: proxyRuleList.width
                    }
                }
            }
        }
        CardBox {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing * 4

                Label {
                    Layout.fillWidth: true

                    text: qsTr("Block")

                    font.pointSize: fontSize
                    color: acrossConfig.textColor
                }

                ListView {
                    id: blockRuleList
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    model: 10
                    delegate: RuleItemCard {
                        width: blockRuleList.width
                    }
                }
            }
        }
    }
}
