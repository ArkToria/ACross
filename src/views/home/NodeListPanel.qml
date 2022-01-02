import "../typescripts/home.js" as HomeJS
import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic as ControlsBasic
import QtQuick.Layouts

Item {
    id: root

    property int nodeGridViewColumns: 2
    property Component nodeShareFormComponent: null
    property Component nodeEditFormComponent: null

    function openShareForm(nodeModel) {
        if (nodeModel === null)
            return ;

        if (nodeShareFormComponent == null)
            nodeShareFormComponent = Qt.createComponent("qrc:/Arktoria/ACross/src/views/home/NodeShareForm.qml");

        if (nodeShareFormComponent.status === Component.Ready) {
            var window = nodeShareFormComponent.createObject(root, {
                "nodeModel": nodeModel
            });
            window.show();
        }
    }

    function openEditForm(nodeModel) {
        if (nodeModel === null)
            return ;

        if (nodeEditFormComponent == null)
            nodeEditFormComponent = Qt.createComponent("qrc:/Arktoria/ACross/src/views/home/NodeEditForm.qml");

        if (nodeEditFormComponent.status === Component.Ready) {
            var window = nodeEditFormComponent.createObject(root, {
                "nodeModel": nodeModel
            });
            window.show();
        }
    }

    function locateCurrent() {
        let index = acrossNodes.getIndexByNode(acrossNodes.currentNodeID, acrossNodes.currentGroupID);
        if (index !== -1)
            locate(index);

    }

    function locate(index) {
        if (index < nodeGridView.count)
            nodeGridView.contentY = nodeGridView.cellHeight * Math.floor(index / nodeGridViewColumns);

    }

    implicitWidth: 648
    implicitHeight: 480

    GridView {
        id: nodeGridView

        anchors.fill: parent
        anchors.leftMargin: acrossConfig.itemSpacing / 2
        cellWidth: Math.floor(width / nodeGridViewColumns)
        cellHeight: 192
        highlightMoveDuration: 0
        highlightFollowsCurrentItem: true
        focus: true

        model: NodeModel {
            list: acrossNodes
        }

        delegate: NodeItemCard {
            clip: true
            implicitWidth: nodeGridView.cellWidth - acrossConfig.itemSpacing
            implicitHeight: nodeGridView.cellHeight - acrossConfig.itemSpacing
        }

        highlight: CardBox {
            borderColor: acrossConfig.highlightColor
        }

        ScrollBar.vertical: ControlsBasic.ScrollBar {
            policy: ScrollBar.AsNeeded
            smooth: true
        }

    }

}
