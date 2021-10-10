import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

import ACross

Menu {
    id: nodeItemPopMenu
    topPadding: 8
    bottomPadding: 8

    property real menuWidth: 168
    onClosed: {
        nodeItemPopMenu.close()
        nodeItemPopMenu.destroy()
    }

    background: CardBox {
        id: popMenuBackground
        color: acrossConfig.backgroundColor

        implicitWidth: menuWidth

        border.width: 1
        border.color: acrossConfig.deepColor
    }

    delegate: MenuItem {
        id: menuItem

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
        text: qsTr("Set as default")

        onTriggered: {
            acrossNodes.setAsDefault(nodeID)
        }
    }

    MenuSeparator {
        background: Rectangle {
            height: 1
            color: acrossConfig.deepColor
        }
    }

    Action {
        id: shareConfigAction
        text: qsTr("Share Config")

        onTriggered: {
            openShareForm(nodeID, name, address, port, password, url)
        }
    }

    Action {
        text: qsTr("ICMP Ping")
    }

    Action {
        text: qsTr("TCP Ping")

        onTriggered: {
            acrossNodes.setAvgLatency(nodeID)
        }
    }

    MenuSeparator {
        background: Rectangle {
            height: 1
            color: acrossConfig.deepColor
        }
    }

    Action {
        text: qsTr("Delete")

        onTriggered: {
            acrossNodes.removeNodeByID(nodeID)
        }
    }
}
