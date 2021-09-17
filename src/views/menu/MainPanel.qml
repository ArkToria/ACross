import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

Rectangle {
    id: mainPanelBackground
    implicitWidth: 120
    implicitHeight: 720
    color: acrossConfig.backgroundColor

    ColumnLayout {
        anchors.fill: parent

        LogoItem {
            id: logo

            Layout.topMargin: 12
            Layout.bottomMargin: 12
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
        }

        ListView {
            id: menuListView
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true

            onHeightChanged: {
                if (menuListView.height <= mainWindow.minimumHeight) {
                    interactive = true
                } else {
                    interactive = false
                }
            }

            // disable highlight animation
            highlightMoveDuration: 0

            model: menuModel

            delegate: MenuItemCard {
                implicitWidth: menuListView.width
            }

            highlight: Rectangle {
                color: acrossConfig.deepColor
            }
        }

        InfoItem {
            id: info
            Layout.bottomMargin: 12
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
        }

        Connections {
            target: pageLoader
            function onCurrentIndexChanged() {
                menuListView.currentIndex = pageLoader.currentIndex
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.9}
}
##^##*/

