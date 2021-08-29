import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

import ACross 1.0
import "../components"

Item {
    id: groupListPanel
    implicitWidth: 312
    implicitHeight: 720

    GroupForm {
        id: appendGroupForm
    }

    GroupForm {
        id: updateGroupForm
    }

    ConfirmDialog {
        id: removeConfirmDialog
        headerText: qsTr("Warning")
        contentText: qsTr("Confirm to remove group from database")
    }

    ColumnLayout {
        anchors.fill: parent

        spacing: 0

        ToolsBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 48

            z: 1
        }

        ListView {
            id: listScrollView
            Layout.fillWidth: true
            Layout.fillHeight: true

            highlightMoveDuration: 0

            model: GroupModel {
                id: groupModel
                list: acrossGroups
            }

            delegate: GroupItemCard {
                id: groupItemCard
                implicitWidth: listScrollView.width
            }

            highlight: Rectangle {
                color: acrossConfig.highlightColor
            }
        }
    }

    ButtonBox {
        id: panelMenuBackground
        x: parent.width - 72
        y: parent.height - 72
        implicitWidth: 56
        implicitHeight: 56
        z: 1

        basicColor: acrossConfig.styleColor
        backgroundRadius: width / 2
        layer.enabled: true
        layer.effect: DropShadow {
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8
            samples: 17
            color: acrossConfig.shadowColor
        }

        SVGBox {
            anchors.centerIn: parent

            source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/add-select.svg"
            sourceWidth: 32
            sourceHeight: 32
        }

        onClicked: {
            mainComponent.state = "PopUpState"

            appendGroupForm.open()
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/

