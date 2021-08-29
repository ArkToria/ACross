import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Item {
    implicitHeight: coreVersionText.contentHeight + guiVersionText.contentHeight

    ColumnLayout {
        anchors.centerIn: parent

        Label {
            id: coreVersionText
            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter

            text: "Core" + " " + acrossConfig.coreVersion
            font.pointSize: 12
            color: acrossConfig.deepTextColor
        }

        Label {
            id: guiVersionText
            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter
            text: "GUI" + " " + acrossConfig.guiVersion
            font.pointSize: 12
            color: acrossConfig.deepTextColor
        }
    }
}

