import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

CardBox {
    id: control

    property int fontSize: 12
    property string title: ""
    property string message: ""
    property int index: -1
    property int groupID: -1
    property real to: -1.0
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

            text: groupID===-1?control.message:control.message.concat(" ", progressBar.value.toString(), "/", to.toString())
            color: acrossConfig.textColor
        }

        ProgressBar {
            id: progressBar

            Layout.fillWidth: true
            Layout.preferredHeight: 4
            padding: 2

            value: 0.0
            to: control.to===-1.0?1:control.to

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
                to: control.to===-1.0?1:control.to
                duration: control.to===-1.0?control.intervalTime:0
            }
            Connections {
                target: acrossGroups
                function onNodeLatencyProgressChanged(group_id, left) {
                    if(group_id === control.groupID) {
                        progressBar.value=control.to-left;
                    }
                }
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
