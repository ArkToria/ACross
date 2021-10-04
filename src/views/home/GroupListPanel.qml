import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import ACross

Item {
    id: groupListPanel
    implicitWidth: 312
    implicitHeight: 720
    
    function setIndexByID(group_id) {
        let index = -1;
        for (let i=0; i<listScrollView.count; i++){
            if(listScrollView.itemAtIndex(i).groupID === group_id){
                index = i;
                break;
            }
        }
        if(index === -1) return;
        listScrollView.currentIndex = index;

        acrossGroups.setDisplayGroupID(group_id)
    }

    GroupForm {
        id: updateGroupForm
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

        property Component appendGroupFormComponent: null

        onClicked: {
            darkBackground.show()
            if (appendGroupFormComponent == null) {
                appendGroupFormComponent = Qt.createComponent(
                            "qrc:/ACross/src/views/home/GroupForm.qml")
            }
            if (appendGroupFormComponent.status === Component.Ready) {
                appendGroupFormComponent.createObject(groupListPanel).open()
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/

