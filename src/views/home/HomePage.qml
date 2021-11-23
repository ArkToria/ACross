import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ACross

Item {
    implicitWidth: 840
    implicitHeight: 720

    SplitView {
        id: splitView
        anchors.fill: parent

        orientation: Qt.Horizontal

        handle: Rectangle {
            implicitWidth: acrossConfig.itemSpacing / 2
            implicitHeight: 4

            color: SplitHandle.hovered ? acrossConfig.highlightColor : acrossConfig.deepColor
        }

        Rectangle {
            implicitWidth: homePage.width * 0.35
            SplitView.maximumWidth: homePage.width * 0.4
            SplitView.minimumWidth: homePage.width * 0.3

            color: acrossConfig.backgroundColor
            border.color: acrossConfig.deepColor
            border.width: 1

            GroupListPanel {
                id: groupListPanel
                anchors.fill: parent
                anchors.margins: 1
            }
        }

        Rectangle {
            SplitView.fillHeight: true
            SplitView.fillWidth: true

            color: acrossConfig.deepColor

            ColumnLayout {
                anchors.fill: parent
                spacing: acrossConfig.itemSpacing

                CurrentPanel {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 230
                }

                GroupControl {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                }

                NodeListPanel {
                    id: nodeListPanel
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    clip: true
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.75;height:480;width:640}
}
##^##*/

