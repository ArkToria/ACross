import QtQuick
import QtQuick.Controls

TextField {
    implicitWidth: 128
    implicitHeight: contentHeight > 32 ? contentHeight : 32

    clip: true
    color: acrossConfig.textColor
    leftPadding: acrossConfig.itemSpacing * 3
    placeholderTextColor: acrossConfig.deepTextColor
    selectByMouse: true

    property string backgroundColor: acrossConfig.deepColor

    background: Rectangle {
        color: backgroundColor
        radius: acrossConfig.borderRadius
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.IBeamCursor
        acceptedButtons: Qt.NoButton
    }
}
