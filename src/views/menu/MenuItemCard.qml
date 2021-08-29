import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

Item {
    id: menuItemCard
    implicitWidth: 120
    implicitHeight: 80

    state: menuItemCard.ListView.isCurrentItem ? "CurrentState" : "NormalState"

    states: [
        State {
            name: "NormalState"
            PropertyChanges {
                target: background
                color: acrossConfig.backgroundColor
            }
            PropertyChanges {
                target: menuItemName
                color: acrossConfig.textColor
            }
        },
        State {
            name: "CurrentState"
            PropertyChanges {
                target: background
                color: "transparent"
            }
            PropertyChanges {
                target: menuItemName
                color: acrossConfig.deepTextColor
            }
        }
    ]

    Rectangle {
        id: background
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent

            SVGBox {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                Layout.topMargin: 8

                source: "qrc:/misc/icons/" + acrossConfig.iconStyle + iconSource
                sourceWidth: 32
                sourceHeight: 32
            }

            Label {
                id: menuItemName
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                Layout.bottomMargin: 8

                text: name
                font.pixelSize: 18
            }
        }
    }

    MouseArea {
        anchors.fill: background

        onClicked: {
            menuItemCard.state = "CurrentState"

            menuListView.currentIndex = index
            pageLoader.source = source
        }
    }
}
