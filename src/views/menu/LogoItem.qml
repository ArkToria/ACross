import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross
import "../typescripts/help.js" as HelpJS

Item {
    implicitHeight: 96
    implicitWidth: 120

    ColumnLayout {
        anchors.fill: parent

        SVGBox {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

            sourceWidth: 64
            sourceHeight: 64
            source: HelpJS.getLogo(acrossConfig.iconStyle)
        }

        Label {
            text: "ACross"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

            font.pixelSize: 24
            color: acrossConfig.textColor
        }
    }
}
