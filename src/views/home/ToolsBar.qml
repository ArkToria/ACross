import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ACross

Item {
    implicitHeight: 48
    implicitWidth: 312
    Rectangle {
        anchors.fill: parent

        color: acrossConfig.backgroundColor

        TextFieldBox {
            anchors.fill: parent
            anchors.margins: 8
        }
    }
}
