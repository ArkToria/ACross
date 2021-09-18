import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import ACross 1.0
import "../components"

Item {
    id: root
    implicitWidth: 648
    implicitHeight: 480

    NodeItemPopMenu {
        id: nodeItemPopMenu
    }

    NodeShareForm {
        id: nodeShareForm
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
