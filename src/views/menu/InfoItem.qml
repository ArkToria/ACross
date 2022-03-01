import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    implicitHeight: apiStateText.contentHeight + coreVersionText.contentHeight + guiVersionText.contentHeight

    ColumnLayout {
        anchors.fill: parent
        anchors.centerIn: parent

        Label {
            id: apiStateText

            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter
            text: "AColoRS"
            color: acolorsNotifications.isRunning?acrossConfig.textColor:acrossConfig.warnColor;
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                onClicked: acolorsNotifications.start();
                hoverEnabled: true
                cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
            }
        }

        Label {
            id: coreVersionText

            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter
            text: acrossConfig.coreName + "\n" + acrossConfig.coreVersion
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
