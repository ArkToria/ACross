import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic as ControlsBasic
import QtQuick.Layouts

Item {
    id: settingPage

    implicitWidth: 840
    implicitHeight: 720

    ControlsBasic.ScrollView {
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
                    id: acolorsItemCard

                    Layout.fillWidth: true
                    implicitHeight: 240

                    AColoRSItem {
                        anchors.fill: parent
                    }

                }

                CardBox {
                    id: coreItemCard

                    Layout.fillWidth: true
                    implicitHeight: 320

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
