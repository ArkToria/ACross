import QtQuick 2.12
import QtQuick.Controls 2.12

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
