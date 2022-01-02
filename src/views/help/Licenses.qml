import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

CardBox {
    property int fontSize: 14

    implicitHeight: 300

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 4
        spacing: acrossConfig.itemSpacing

        Label {
            text: qsTr("Libraries and Licenses")
            font.pointSize: Math.round(fontSize * 1.2)
            color: acrossConfig.textColor
        }

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            spacing: acrossConfig.itemSpacing

            model: LicenseListModels {
            }

            delegate: RowLayout {
                spacing: acrossConfig.itemSpacing * 2

                Label {
                    text: model.key
                    color: acrossConfig.textColor
                }

                URLBox {
                    text: model.value
                    urlText: model.url
                }

            }

        }

    }

}
