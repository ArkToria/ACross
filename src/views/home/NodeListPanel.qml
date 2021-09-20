import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    id: root
    implicitWidth: 648
    implicitHeight: 480

    property Component nodeShareFormComponent: null
    function openShareForm(name, address, port, password, url, qrcode) {
        if (nodeShareFormComponent == null) {
            nodeShareFormComponent = Qt.createComponent(
                        "qrc:/src/views/home/NodeShareForm.qml")
        }
        if (nodeShareFormComponent.status === Component.Ready) {
            var window = nodeShareFormComponent.createObject(mainWindow, {
                                                                 "name": name,
                                                                 "address": address,
                                                                 "port": port,
                                                                 "password": password,
                                                                 "url": url,
                                                                 "qrcode": qrcode
                                                             })
            window.show()
        }
    }

    GridView {
        id: nodeGridView
        anchors.fill: parent
        anchors.leftMargin: acrossConfig.itemSpacing / 2

        model: NodeModel {
            list: acrossNodes
        }

        cellWidth: Math.round((width - acrossConfig.itemSpacing) / 2)
        cellHeight: 192

        delegate: NodeItemCard {
            clip: true
            implicitWidth: nodeGridView.cellWidth - acrossConfig.itemSpacing
            implicitHeight: nodeGridView.cellHeight - acrossConfig.itemSpacing
        }

        highlightMoveDuration: 0
        highlightFollowsCurrentItem: true
        focus: true

        highlight: CardBox {
            border.color: acrossConfig.highlightColor
        }

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
            smooth: true
        }
    }
}
