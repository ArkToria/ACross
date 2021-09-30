import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

import ACross

Menu {
    id: groupItemPopMenu
    topPadding: acrossConfig.itemSpacing
    bottomPadding: acrossConfig.itemSpacing

    property real menuWidth: 168

    onClosed: {
        groupItemPopMenu.destroy()
    }

    background: CardBox {
        id: popMenuBackground

        implicitWidth: menuWidth

        color: acrossConfig.backgroundColor
        border.width: 1
        border.color: acrossConfig.deepColor
    }

    delegate: MenuItem {
        id: menuItem

        visible: enabled
        height: visible ? implicitHeight : 0

        contentItem: Text {
            text: menuItem.text
            color: menuItem.highlighted ? acrossConfig.highlightTextColor : acrossConfig.textColor
        }

        background: Rectangle {
            implicitWidth: menuWidth
            color: menuItem.highlighted ? acrossConfig.highlightColor : "transparent"
        }
    }

    Action {
        text: qsTr("Edit")
        onTriggered: {
            acrossGroups.editItem(index)
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
        text: qsTr("ICMP Ping")
    }

    Action {
        text: qsTr("TCP Ping")
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
