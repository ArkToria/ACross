import QtQuick
import QtQuick.Controls

import ACross

Label {
    id: urlLabel
    property string urlText

    text: urlText
    color: acrossConfig.highlightColor
    font.underline: true

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onEntered: {
            parent.color = acrossConfig.deepTextColor
        }

        onExited: {
            parent.color = acrossConfig.highlightColor
        }

        onClicked: Qt.openUrlExternally(urlText)
    }
}
