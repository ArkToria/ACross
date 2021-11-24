import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Arktoria.ACross

CardBox {
    id: control
<<<<<<< HEAD
    implicitWidth: 320
=======
    implicitWidth: Math.max(
                       displayTitle.width,
                       displayMessage.width) > 240 ? Math.max(
                                                         displayTitle.width,
                                                         displayMessage.width) : 240
>>>>>>> 7cc215f (rebase from master)
    implicitHeight: 84
    clip: true

    property int fontSize: 12
    property string title: ""
    property string message: ""
    property int intervalTime: 5000

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 3

        Label {
            id: displayTitle
            Layout.fillWidth: true

            text: control.title
            color: acrossConfig.textColor
            font.pointSize: fontSize
        }

        Label {
            id: displayMessage
            Layout.fillWidth: true

            text: control.message
            color: acrossConfig.textColor
        }

        ProgressBar {
            id: progressBar
            Layout.fillWidth: true
            Layout.preferredHeight: 4
            padding: 2

            value: 0.0

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

            NumberAnimation on value {
                id: progressBarAnimation
                from: 0.0
                to: 1.0
                duration: control.intervalTime
            }
        }
    }

    Timer {
        repeat: true
        running: true
        interval: intervalTime

        onTriggered: {
            if (popNotifyModel.hasChildren()) {
                popNotifyModel.remove(0)
            }
        }
    }
}
