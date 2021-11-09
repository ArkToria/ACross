import QtQuick
import Qt5Compat.GraphicalEffects

Rectangle {
    id: cardBackground
    implicitWidth: 48
    implicitHeight: 48
    clip: true

    radius: acrossConfig.borderRadius
    color: acrossConfig.backgroundColor

    layer.enabled: false
    layer.effect: DropShadow {
        horizontalOffset: 0
        verticalOffset: 2
        radius: 8
        transparentBorder: true
        color: acrossConfig.shadowColor
        source: cardBackground
    }
}
