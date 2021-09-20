import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../components"

Item {
    id: settingPage
    implicitWidth: 840
    implicitHeight: 720

    ScrollView {
        id: scrollView
        anchors.fill: parent

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
                implicitHeight: 280

                InboundItem {
                    anchors.fill: parent
                }
            }

            CardBox {
                implicitWidth: scrollView.availableWidth - acrossConfig.itemSpacing
                implicitHeight: 320

                ApplicationItem {
                    anchors.fill: parent
                }
            }
        }
    }
}
