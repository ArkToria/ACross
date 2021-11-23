import QtQuick
import Qt5Compat.GraphicalEffects

import ACross

Item {
    implicitWidth: 48
    implicitHeight: 48
    clip: true

    property string color: acrossConfig.backgroundColor
    property int radius: acrossConfig.borderRadius
    property int borderWidth: 0
    property string borderColor: acrossConfig.borderColor
    property bool layerEnabled: true

    Rectangle {
        id: background
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing

        radius: parent.radius
        color: parent.color
        border.width: parent.borderWidth
        border.color: parent.borderColor

        layer.enabled: parent.layerEnabled
        layer.effect: DropShadow {
            horizontalOffset: 0
            verticalOffset: 0
            radius: 6
            transparentBorder: true
            color: acrossConfig.shadowColor
            source: background
        }
    }
}
