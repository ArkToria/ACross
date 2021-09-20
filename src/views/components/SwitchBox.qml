import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Switch {
    id: control
    implicitWidth: 72
    implicitHeight: 26

    indicator: Rectangle {
        anchors.fill: parent
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: parent.height / 2
        color: control.checked ? acrossConfig.highlightColor : acrossConfig.deepColor

        Rectangle {
            id: handleBackground
            x: control.checked ? parent.width - width : 0
            width: parent.height
            height: parent.height
            radius: parent.height / 2
            color: acrossConfig.highlightTextColor

            layer.enabled: true
            layer.effect: DropShadow {
                horizontalOffset: 1
                verticalOffset: 1
                radius: 3
                samples: 11
                color: acrossConfig.shadowColor
                source: handleBackground
            }
        }
    }
}
