/*##^##
Designer {
    D{i:0;formeditorZoom:0.9}
}
##^##*/

import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    implicitWidth: 120
    implicitHeight: 720
    color: acrossConfig.backgroundColor

    Connections {
        function onCurrentIndexChanged() {
            if (menuListView.currentIndex !== pageLoader.currentIndex)
                menuListView.currentIndex = pageLoader.currentIndex;

        }

        target: pageLoader
    }

    ColumnLayout {
        anchors.fill: parent

        LogoItem {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
        }

        ListView {
            id: menuListView

            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            interactive: false
            // disable highlight animation
            highlightMoveDuration: 0
            onHeightChanged: {
                if (menuListView.height <= mainWindow.minimumHeight)
                    interactive = true;
                else
                    interactive = false;
            }

            model: MenuModel {
            }

            delegate: MenuItemCard {
            }

            highlight: Rectangle {
                color: acrossConfig.deepColor
            }

        }

        InfoItem {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
            Layout.bottomMargin: acrossConfig.itemSpacing * 4
        }

    }

}
