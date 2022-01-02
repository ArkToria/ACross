import Arktoria.ACross
import Qt5Compat.GraphicalEffects
import QtQuick
import QtQuick.Controls

Menu {
    id: groupItemPopMenu

    property real menuWidth: 168

    topPadding: acrossConfig.borderRadius * 2
    bottomPadding: acrossConfig.borderRadius * 2
    onClosed: {
        groupItemPopMenu.destroy();
    }

    Action {
        text: qsTr("Edit")
        onTriggered: {
            darkBackground.show();
            var groupInfo = acrossGroups.getGroupInfo(index);
            groupEditForm.groupInfo = groupInfo;
            groupEditForm.index = index;
            groupEditForm.open();
        }
    }

    Action {
        enabled: isSubscription
        text: qsTr("Copy URL")
        onTriggered: {
            acrossGroups.copyUrlToClipboard(index);
        }
    }

    Action {
        text: qsTr("Copy Nodes")
        onTriggered: {
            acrossGroups.copyNodesToClipboard(index);
        }
    }

    Action {
        text: qsTr("TCP Ping")
        onTriggered: {
            acrossGroups.testTcpPing(index);
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
            acrossGroups.checkUpdate(index);
            popNotify.notify(qsTr("[%1] Updating...").arg(name), qsTr("Updated: %1").arg(modifiedAt));
        }
    }

    Action {
        text: qsTr("Delete")
        enabled: 0 === model.index ? false : true
        onTriggered: {
            darkBackground.show();
            removeConfirmDialog.index = index;
            removeConfirmDialog.open();
        }
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

}
