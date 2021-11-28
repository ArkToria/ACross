import QtQuick
import QtQuick.Controls

import Arktoria.ACross

Rectangle {
    id: control
    color: Qt.darker(acrossConfig.deepColor, 5)
    opacity: 0.7
    visible: false

    function show() {
        control.visible = true
    }

    function close() {
        control.visible = false
    }
}
