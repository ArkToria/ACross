import Arktoria.ACross
import QtQuick
import QtQuick.Controls

Rectangle {
    id: control

    function show() {
        control.visible = true;
    }

    function close() {
        control.visible = false;
    }

    color: Qt.darker(acrossConfig.deepColor, 5)
    opacity: 0.7
    visible: false
}
