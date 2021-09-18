import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

import "../components"

Menu {
    id: nodeItemPopMenu
    topPadding: 8
    bottomPadding: 8

    property real menuWidth: 168
    onVisibleChanged: {
        if (!visible) {
            nodeItemPopMenu.close()
            nodeItemPopMenu.destroy()
        }
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
            if (nodeShareFormComponent == null){
                nodeShareFormComponent = Qt.createComponent("qrc:/src/views/home/NodeShareForm.qml")
            }
            if (nodeShareFormComponent.status === Component.Ready){
                var window = nodeShareFormComponent.createObject(mainWindow)

                window.name = name
                window.address = address
                window.port = port
                window.password = password
                window.url = ""
                window.qrcode = ""

                window.show()
            }
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
        text: qsTr("Delete")

        onTriggered: {
            acrossNodes.removeCurrentNode(nodeID)
        }
    }
}
