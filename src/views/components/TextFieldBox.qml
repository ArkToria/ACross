import Arktoria.ACross
import QtQuick
import QtQuick.Controls

TextField {
    property string backgroundColor: acrossConfig.deepColor

    implicitWidth: 128
    implicitHeight: contentHeight > 32 ? contentHeight : 32
    clip: true
    color: acrossConfig.textColor
    leftPadding: acrossConfig.itemSpacing * 3
    placeholderTextColor: acrossConfig.deepTextColor
    selectByMouse: true

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.IBeamCursor
        acceptedButtons: Qt.NoButton
    }

    background: Rectangle {
        color: backgroundColor
        radius: acrossConfig.borderRadius
    }

}
