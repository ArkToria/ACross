import Arktoria.ACross
import Qt5Compat.GraphicalEffects
import QtQuick
import QtQuick.Controls

Switch {
    id: control

    property string foregroundColor: acrossConfig.highlightTextColor

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
            color: foregroundColor
            layer.enabled: true

            layer.effect: DropShadow {
                horizontalOffset: 2
                verticalOffset: 2
                radius: 8
                transparentBorder: true
                color: acrossConfig.shadowColor
                source: handleBackground
            }

        }

    }

}
