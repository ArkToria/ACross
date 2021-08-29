import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "../components"

Item {
    implicitWidth: 840
    implicitHeight: 720

    ScrollView {
        id: scrollView
        anchors.fill: parent

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        clip: true

        Column {
            id: column
            topPadding: acrossConfig.itemSpacing
            leftPadding: acrossConfig.itemSpacing
            bottomPadding: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            CardBox {
                implicitWidth: scrollView.availableWidth - acrossConfig.itemSpacing
                implicitHeight: 260

                CoreItem {
                    anchors.fill: parent
                }
            }

            CardBox {
                implicitWidth: scrollView.availableWidth - acrossConfig.itemSpacing
                implicitHeight: 360

                InboundItem {
                    anchors.fill: parent
                }
            }

            CardBox {
                implicitWidth: scrollView.availableWidth - acrossConfig.itemSpacing
                implicitHeight: 260

                ApplicationItem {
                    anchors.fill: parent
                }
            }
        }
    }
}
