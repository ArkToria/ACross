import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

import "../components"

Menu {
    topPadding: 8
    bottomPadding: 8

    property real menuWidth: 168

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
        background: Rectangle {
            height: 1
            color: acrossConfig.deepColor
        }
    }

    Action {
        enabled: isSubscription

        text: qsTr("Update")
    }

    Action {
        text: qsTr("Delete")

        onTriggered: {
            mainComponent.state = "PopUpState"

            removeConfirmDialog.index = index

            removeConfirmDialog.open()
        }
    }
}
