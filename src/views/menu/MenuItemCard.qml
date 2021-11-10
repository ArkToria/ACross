import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    id: menuItemCard
    implicitWidth: 120
    implicitHeight: 86

    property int fontSize: 14

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
            spacing: acrossConfig.itemSpacing

            Item {
                Layout.fillHeight: true
            }

            SVGBox {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

                source: "qrc:/misc/icons/" + acrossConfig.iconStyle + iconSource
                sourceWidth: 32
                sourceHeight: sourceWidth
            }

            Label {
                id: menuItemName
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

                text: qsTr(name)
                font.pointSize: fontSize
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }

    MouseArea {
        anchors.fill: background

        onClicked: {
            menuItemCard.state = "CurrentState"
            pageLoader.currentIndex = index
            menuListView.currentIndex = index
        }
    }
}
