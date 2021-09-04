import QtQuick 2.0

import QtQuick 2.12
import QtQuick.Controls 2.12

Switch {
    id: control
    implicitWidth: 72
    implicitHeight: 26

    indicator: Rectangle {
        anchors.fill: parent
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: parent.height / 2
        color: acrossConfig.deepColor

        Rectangle {
            id: background
            x: control.checked ? parent.width - width : 0
            width: parent.height
            height: parent.height
            radius: parent.height / 2
            color: acrossConfig.highlightColor
        }
    }
}
