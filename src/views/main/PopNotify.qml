import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ACross

CardBox {
    id: control
    implicitWidth: Math.max(
                       displayTitle.contentWidth,
                       displayMessage.contentWidth) < 240 ? 240 : Math.max(
                                                                displayTitle.contentWidth,
                                                                displayMessage.contentWidth)
    implicitHeight: 84
    visible: false
    clip: true

    property int fontSize: 12
    property int intervalTime: 5000

    Timer {
        id: displayTimer
        interval: control.intervalTime
        repeat: false
        onTriggered: {
            displayTimer.stop()
            progressBarAnimation.complete()
            control.visible = false
        }
    }

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
            Layout.fillWidth: true

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

    function show(title = "", message = "") {
        control.visible = true

        if (title !== "") {
            displayTitle.text = title
        }

        if (message !== "") {
            displayMessage.text = message
        }

        progressBarAnimation.restart()
        displayTimer.start()
    }
}
