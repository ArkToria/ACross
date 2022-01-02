import "../typescripts/help.js" as HelpJS
import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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
