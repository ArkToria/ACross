/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/

import Arktoria.ACross
import Qt5Compat.GraphicalEffects
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: groupListPanel

    property Component groupFormComponent: null

    function setIndexByID(group_id) {
        let index = acrossGroups.getIndexByID(group_id);
        if (index === -1)
            return ;

        listScrollView.currentIndex = index;
        acrossGroups.setDisplayGroupID(group_id);
    }

    function openGroupForm() {
        darkBackground.show();
        if (groupFormComponent == null)
            groupFormComponent = Qt.createComponent("qrc:/Arktoria/ACross/src/views/home/GroupForm.qml");

        if (groupFormComponent.status === Component.Ready)
            groupFormComponent.createObject(groupListPanel).open();

    }

    implicitWidth: 312
    implicitHeight: 720

    RemoveConfirmDialog {
        id: removeConfirmDialog

        headerText: qsTr("Warning")
        contentText: qsTr("Confirm to remove group from database")
    }

    GroupEditForm {
        id: groupEditForm
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ToolsBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
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

            property bool updateToken: false

            Layout.fillWidth: true
            Layout.fillHeight: true
            highlightMoveDuration: 0
            onContentYChanged: {
                if (-contentY > updateIcon.sourceHeight * 2) {
                    updateIcon.visible = true;
                    updateToken = true;
                } else {
                    updateIcon.visible = false;
                }
                if (updateToken && this.contentY === 0) {
                    popNotify.notify(qsTr("Updating..."), qsTr("The default node will be reset"), 0.0, 1.0, 0.0, 2000);
                    acrossGroups.checkAllUpdate(true); // force update
                    updateToken = false;
                }
            }

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
        onClicked: {
            openGroupForm();
        }

        SVGBox {
            anchors.centerIn: parent
            source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/add.svg"
            sourceWidth: 36
            sourceHeight: sourceWidth
        }

        layer.effect: DropShadow {
            horizontalOffset: 2
            verticalOffset: 2
            radius: 8
            transparentBorder: true
            color: acrossConfig.shadowColor
        }

    }

}
