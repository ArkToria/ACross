import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ACross

Item {
    id: settingPage
    implicitWidth: 840
    implicitHeight: 720

    ScrollView {
        id: scrollView
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing

        clip: true
        contentHeight: column.height

        Flickable {
            ColumnLayout {
                id: column
                width: scrollView.availableWidth
                spacing: acrossConfig.itemSpacing

                CardBox {
                    Layout.fillWidth: true
                    implicitHeight: 260

                    CoreItem {
                        anchors.fill: parent
                    }
                }

                CardBox {
                    Layout.fillWidth: true
                    implicitHeight: 280

                    InboundItem {
                        anchors.fill: parent
                    }
                }

                CardBox {
                    id: applicationItemCard
                    Layout.fillWidth: true
                    implicitHeight: 320

                    ApplicationItem {
                        anchors.fill: parent
                    }
                }

                CardBox {
                    id: appearanceItemCard
                    Layout.fillWidth: true
                    implicitHeight: 180

                    AppearanceItem {
                        anchors.fill: parent
                    }
                }
            }
        }
    }
}
