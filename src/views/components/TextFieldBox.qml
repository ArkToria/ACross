import QtQuick
import QtQuick.Controls

TextField {
    color: acrossConfig.deepTextColor
    placeholderTextColor: acrossConfig.deepTextColor

    property string backgroundColor: acrossConfig.deepColor

    background: Rectangle {
        color: backgroundColor
        radius: acrossConfig.borderRadius
    }
}
