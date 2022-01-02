import Arktoria.ACross
import Qt5Compat.GraphicalEffects
import QtQuick
import QtQuick.Controls

Menu {
    id: nodeItemPopMenu

    property real menuWidth: 168

    topPadding: acrossConfig.borderRadius * 2
    bottomPadding: acrossConfig.borderRadius * 2
    onClosed: {
        nodeItemPopMenu.close();
        nodeItemPopMenu.destroy();
    }

    Action {
        text: qsTr("Edit")
        onTriggered: {
            openEditForm(model);
        }
    }

    Action {
        text: qsTr("Set as default")
        onTriggered: {
            acrossNodes.setAsDefault(nodeID);
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
            openShareForm(model);
        }
    }

    Action {
        text: qsTr("TCP Ping")
        onTriggered: {
            acrossNodes.testLatency(nodeID);
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
            acrossNodes.removeNodeByID(nodeID);
        }
    }

    background: CardBox {
        id: popMenuBackground

        color: acrossConfig.backgroundColor
        implicitWidth: menuWidth
        borderWidth: 1
        borderColor: acrossConfig.deepColor
    }

    delegate: MenuItem {
        id: menuItem

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
