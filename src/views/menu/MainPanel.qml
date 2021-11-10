import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Rectangle {
    implicitWidth: 120
    implicitHeight: 720

    color: acrossConfig.backgroundColor

    Connections {
        target: pageLoader

        function onCurrentIndexChanged() {
            if (menuListView.currentIndex !== pageLoader.currentIndex) {
                menuListView.currentIndex = pageLoader.currentIndex
            }
        }
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

            model: MenuModel {}

            delegate: MenuItemCard {}

            // disable highlight animation
            highlightMoveDuration: 0

            highlight: Rectangle {
                color: acrossConfig.deepColor
            }

            onHeightChanged: {
                if (menuListView.height <= mainWindow.minimumHeight) {
                    interactive = true
                } else {
                    interactive = false
                }
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

/*##^##
Designer {
    D{i:0;formeditorZoom:0.9}
}
##^##*/

