import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ACross

CardBox {
    implicitWidth: 240
    implicitHeight: 36

    layerEnabled: false
    color: acrossConfig.deepColor

    RowLayout {
        anchors.fill: parent
        spacing: acrossConfig.itemSpacing * 4

        Rectangle {
            width: 8
            height: 8
            radius: 4
            color: acrossConfig.highlightColor
        }

        Label {
            Layout.fillWidth: true

            text: "geosite:cn"
            color: acrossConfig.textColor
        }
    }
}
