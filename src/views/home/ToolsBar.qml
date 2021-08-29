import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "../components"

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
