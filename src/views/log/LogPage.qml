import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    implicitWidth: 840
    implicitHeight: 720

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing
        clip: true
        spacing: acrossConfig.itemSpacing

        CoreLog {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        AppLog {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

    }

}
