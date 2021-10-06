import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    id: root
    implicitWidth: 648
    implicitHeight: 480

    property Component nodeShareFormComponent: null
    function openShareForm(nodeID, name, address, port, password, url) {
        if (nodeShareFormComponent == null) {
            nodeShareFormComponent = Qt.createComponent(
                        "qrc:/ACross/src/views/home/NodeShareForm.qml")
        }
        if (nodeShareFormComponent.status === Component.Ready) {
            var window = nodeShareFormComponent.createObject(mainWindow, {
                                                                 "nodeID": nodeID,
                                                                 "name": name,
                                                                 "address": address,
                                                                 "port": port,
                                                                 "password": password,
                                                                 "url": url
                                                             })
            window.show()
        }
    }

    property int currentIndex : nodeGridView.currentIndex
    function setIndex(index){
        currentIndex = index;
        nodeGridView.currentIndex = index;
    }
    function locate(index) {
        if(index < nodeGridView.count) {
            nodeGridView.contentY = nodeGridView.cellHeight * Math.floor(index / nodeGridViewColumns)
        }
    }

    property int nodeGridViewColumns : 2
    GridView {
        id: nodeGridView
        anchors.fill: parent
        anchors.leftMargin: acrossConfig.itemSpacing / 2

        model: NodeModel {
            list: acrossNodes
        }

        cellWidth: Math.round(width / nodeGridViewColumns)
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
