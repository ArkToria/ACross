import Arktoria.ACross
import QtQuick
import QtQuick.Controls

TextArea {
    property string backgroundColor: acrossConfig.deepColor

    implicitWidth: 128
    color: acrossConfig.textColor
    placeholderTextColor: acrossConfig.deepTextColor
    selectByMouse: true

    background: Rectangle {
        color: backgroundColor
        radius: acrossConfig.borderRadius
    }

}
