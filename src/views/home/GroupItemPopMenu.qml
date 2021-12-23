import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

import Arktoria.ACross

Menu {
    id: groupItemPopMenu
    topPadding: acrossConfig.borderRadius * 2
    bottomPadding: acrossConfig.borderRadius * 2

    property real menuWidth: 168

    onClosed: {
        groupItemPopMenu.destroy()
    }

    background: CardBox {
        id: popMenuBackground

        implicitWidth: menuWidth

        color: acrossConfig.backgroundColor
        radius: acrossConfig.borderRadius
        borderWidth: 1
        borderColor: acrossConfig.deepColor
    }

    delegate: MenuItem {
        id: menuItem

        visible: enabled
        height: visible ? implicitHeight : 0

        contentItem: Text {
            padding: Math.round(acrossConfig.itemSpacing / 2)

            text: menuItem.text
            color: menuItem.highlighted ? acrossConfig.highlightTextColor : acrossConfig.textColor
        }

        background: Rectangle {
            anchors.centerIn: parent
            width: menuItem.width - acrossConfig.itemSpacing * 2
            height: menuItem.height

            color: menuItem.highlighted ? acrossConfig.highlightColor : "transparent"
        }
    }

    Action {
        text: qsTr("Edit")
        onTriggered: {
            darkBackground.show()
            var groupInfo = acrossGroups.getGroupInfo(index)

            groupEditForm.groupInfo = groupInfo
            groupEditForm.index = index
            groupEditForm.open()
        }
    }

    Action {
        enabled: isSubscription

        text: qsTr("Copy URL")

        onTriggered: {
            acrossGroups.copyUrlToClipboard(index)
        }
    }

    Action {
        text: qsTr("Copy Nodes")

        onTriggered: {
            acrossGroups.copyNodesToClipboard(index)
        }
    }

    Action {
        text: qsTr("TCP Ping")
        onTriggered: {
            if(acrossGroups.testTcpPing(index) === 0){
                popNotify.notifyWithProgress(qsTr("[%1] TCP Pinging...").arg(name),
                                             qsTr("Testing:"),
                                             index,
                                             group_id,
                                             items)
            }
        }
    }

    MenuSeparator {
        visible: 0 === model.index ? false : true

        background: Rectangle {
            height: 1
            color: acrossConfig.deepColor
        }
    }

    Action {
        enabled: isSubscription

        text: qsTr("Update")
        onTriggered: {
            acrossGroups.checkUpdate(index)
            popNotify.notify(qsTr("[%1] Updating...").arg(name),
                             qsTr("Updated: %1").arg(modifiedAt))
        }
    }

    Action {
        text: qsTr("Delete")
        enabled: 0 === model.index ? false : true

        onTriggered: {
            darkBackground.show()
            removeConfirmDialog.index = index
            removeConfirmDialog.open()
        }
    }
}
