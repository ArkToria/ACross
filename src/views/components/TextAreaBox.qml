import QtQuick
import QtQuick.Controls

TextArea {
    color: acrossConfig.deepTextColor
    placeholderTextColor: acrossConfig.deepTextColor

    property string backgroundColor: acrossConfig.deepColor

    background: Rectangle {
        color: backgroundColor
        radius: acrossConfig.borderRadius
    }
}
