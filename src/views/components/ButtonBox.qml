/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

import Arktoria.ACross
import QtQuick
import QtQuick.Controls

Button {
    id: button

    property string basicState: "NormalState"
    property int backgroundRadius: acrossConfig.borderRadius
    property string basicColor: acrossConfig.highlightColor

    implicitWidth: contentText.contentWidth < 72 ? 72 : contentText.contentWidth + acrossConfig.itemSpacing * 2
    implicitHeight: 32
    state: basicState
    states: [
        State {
            name: "NormalState"

            PropertyChanges {
                target: background
                color: basicColor
            }

        },
        State {
            name: "WarnState"

            PropertyChanges {
                target: button
                basicColor: acrossConfig.warnColor
            }

        },
        State {
            name: "HoverState"

            PropertyChanges {
                target: background
                color: Qt.lighter(basicColor, 1.1)
            }

        },
        State {
            name: "ClickState"

            PropertyChanges {
                target: background
                color: Qt.lighter(basicColor, 0.9)
            }

        }
    ]

    MouseArea {
        hoverEnabled: true
        anchors.fill: button
        onEntered: {
            button.state = "HoverState";
            cursorShape = Qt.PointingHandCursor;
        }
        onExited: {
            button.state = basicState;
            cursorShape = Qt.ArrowCursor;
        }
        onClicked: {
            button.clicked();
        }
        onPressed: {
            button.state = "ClickState";
        }
        onReleased: {
            if (containsMouse)
                button.state = "HoverState";
            else
                button.state = basicState;
        }
    }

    contentItem: Text {
        id: contentText

        text: parent.text
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: acrossConfig.highlightTextColor
    }

    background: Rectangle {
        id: background

        color: basicColor
        radius: backgroundRadius
    }

}
