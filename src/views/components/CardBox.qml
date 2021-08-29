import QtQuick 2.12
import QtGraphicalEffects 1.0

Rectangle {
    id: cardBackground
    implicitWidth: 48
    implicitHeight: 48
    clip: true

    radius: acrossConfig.borderRadius
    color: acrossConfig.backgroundColor

    layer.enabled: true
    layer.effect: DropShadow {
        horizontalOffset: 1
        verticalOffset: 1
        radius: 5
        samples: 11
        color: acrossConfig.shadowColor
        source: cardBackground
    }
}
