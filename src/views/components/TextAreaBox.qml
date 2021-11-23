import QtQuick
import QtQuick.Controls

import ACross

TextArea {
    color: acrossConfig.textColor
    placeholderTextColor: acrossConfig.deepTextColor
    selectByMouse: true

    property string backgroundColor: acrossConfig.deepColor

    background: Rectangle {
        color: backgroundColor
        radius: acrossConfig.borderRadius
    }
}
