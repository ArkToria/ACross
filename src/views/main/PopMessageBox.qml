import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

CardBox {
    id: control

    property int fontSize: 12
    property alias title: displayTitle.text
    property alias message: displayMessage.text
    property alias from: progressBar.from
    property alias to: progressBar.to
    property alias value: progressBar.value

    implicitWidth: 320
    implicitHeight: 84
    clip: true

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 3

        Label {
            id: displayTitle

            Layout.fillWidth: true
            color: acrossConfig.textColor
            font.pointSize: fontSize
        }

        Label {
            id: displayMessage

            color: acrossConfig.textColor
        }

        ProgressBar {
            id: progressBar

            Layout.fillWidth: true
            Layout.preferredHeight: 4
            padding: 2

            background: Rectangle {
                implicitWidth: 240
                implicitHeight: 6
                color: acrossConfig.backgroundColor
                radius: 3
            }

            contentItem: Item {
                implicitWidth: 240
                implicitHeight: 4

                Rectangle {
                    width: progressBar.visualPosition * progressBar.width
                    height: progressBar.height
                    radius: 2
                    color: acrossConfig.highlightColor
                }

            }

        }

    }

}
