import QtQuick 2.12
import QtQuick.Controls 2.12

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
