import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

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
