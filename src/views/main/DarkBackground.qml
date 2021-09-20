import QtQuick
import QtQuick.Controls

Rectangle {
    id: control
    color: "black"
    opacity: 0.7
    visible: false

    function show() {
        control.visible = true
    }

    function close() {
        control.visible = false
    }
}
