import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    implicitHeight: coreVersionText.contentHeight + guiVersionText.contentHeight

    ColumnLayout {
        anchors.fill: parent
        anchors.centerIn: parent

        Label {
            id: coreVersionText

            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("Core") + " " + acrossConfig.coreVersion
            color: acrossConfig.textColor
        }

        Label {
            id: guiVersionText

            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("Client") + " " + acrossConfig.guiVersion
            color: acrossConfig.textColor
        }

    }

}
