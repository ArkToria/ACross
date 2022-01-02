import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

CardBox {
    id: control

    property int fontSize: 12
    property string title: ""
    property string message: ""
    property int intervalTime: 5000

    implicitWidth: 320
    implicitHeight: 84
    clip: true

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
            value: 0

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

                from: 0
                to: 1
                duration: control.intervalTime
            }

        }

    }

    Timer {
        repeat: true
        running: true
        interval: intervalTime
        onTriggered: {
            if (popNotifyModel.hasChildren())
                popNotifyModel.remove(0);

        }
    }

}
