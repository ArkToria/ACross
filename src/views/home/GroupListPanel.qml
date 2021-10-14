import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import ACross

Item {
    id: groupListPanel
    implicitWidth: 312
    implicitHeight: 720

    property Component groupFormComponent: null

    function setIndexByID(group_id) {
        let index = acrossGroups.getIndexByID(group_id)
        if (index === -1)
            return
        listScrollView.currentIndex = index

        acrossGroups.setDisplayGroupID(group_id)
    }

    function openGroupForm() {
        darkBackground.show()
        if (groupFormComponent == null) {
            groupFormComponent = Qt.createComponent(
                        "qrc:/ACross/src/views/home/GroupForm.qml")
        }
        if (groupFormComponent.status === Component.Ready) {
            groupFormComponent.createObject(groupListPanel).open()
        }
    }

    RemoveConfirmDialog {
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

        SVGBox {
            id: updateIcon
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

            visible: false
            source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/update.svg"
            sourceWidth: 24
            sourceHeight: 24
        }

        ListView {
            id: listScrollView
            Layout.fillWidth: true
            Layout.fillHeight: true

            property bool updateToken: false

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

            onContentYChanged: {
                if (-contentY > updateIcon.sourceHeight * 2) {
                    updateIcon.visible = true
                    updateToken = true
                } else {
                    updateIcon.visible = false
                }

                if (updateToken && this.contentY === 0) {
                    acrossGroups.checkAllUpdate(true) // force update
                    updateToken = false
                }
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
            horizontalOffset: 2
            verticalOffset: 2
            radius: 8
            transparentBorder: true
            color: acrossConfig.shadowColor
        }

        SVGBox {
            anchors.centerIn: parent

            source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/add.svg"
            sourceWidth: 36
            sourceHeight: sourceWidth
        }

        onClicked: {
            openGroupForm()
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/

