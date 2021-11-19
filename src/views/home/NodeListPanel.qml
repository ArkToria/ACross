import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic as ControlsBasic

import ACross
import "../typescripts/home.js" as HomeJS

Item {
    id: root
    implicitWidth: 648
    implicitHeight: 480

    property int nodeGridViewColumns: 2
    property Component nodeShareFormComponent: null
    property Component nodeEditFormComponent: null

    function openShareForm(nodeModel) {
        if (nodeModel === null)
            return

        if (nodeShareFormComponent == null) {
            nodeShareFormComponent = Qt.createComponent(
                        "qrc:/ACross/src/views/home/NodeShareForm.qml")
        }

        if (nodeShareFormComponent.status === Component.Ready) {
            var window = nodeShareFormComponent.createObject(root, {
                                                                 "nodeModel": nodeModel
                                                             })
            window.show()
        }
    }

    function openEditForm(nodeModel) {
        if (nodeModel === null)
            return

        if (nodeEditFormComponent == null) {
            nodeEditFormComponent = Qt.createComponent(
                        "qrc:/ACross/src/views/home/NodeEditForm.qml")
        }

        if (nodeEditFormComponent.status === Component.Ready) {
            var window = nodeEditFormComponent.createObject(root, {
                                                                "nodeModel": nodeModel
                                                            })
            window.show()
        }
    }

    function locateCurrent() {
        let index = acrossNodes.getIndexByNode(acrossNodes.currentNodeID,
                                               acrossNodes.currentGroupID)
        if (index !== -1) {
            locate(index)
        }
    }

    function locate(index) {
        if (index < nodeGridView.count) {
            nodeGridView.contentY = nodeGridView.cellHeight * Math.floor(
                        index / nodeGridViewColumns)
        }
    }

    GridView {
        id: nodeGridView
        anchors.fill: parent
        anchors.leftMargin: acrossConfig.itemSpacing / 2

        model: NodeModel {
            list: acrossNodes
        }

        cellWidth: Math.floor(width / nodeGridViewColumns)
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
            borderColor: acrossConfig.highlightColor
        }

        ScrollBar.vertical: ControlsBasic.ScrollBar {
            policy: ScrollBar.AsNeeded
            smooth: true
        }
    }
}
