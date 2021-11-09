import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

CardBox {
    implicitHeight: 280

    property int fontSize: 14

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 4
        spacing: acrossConfig.itemSpacing

        Label {
            text: qsTr("Libraries and Licenses")
            font.pointSize: fontSize * 1.2
            color: acrossConfig.textColor
        }

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            spacing: acrossConfig.itemSpacing
            model: LicenseListModels {}
            delegate: Label {
                text: key + " - " + value
                color: acrossConfig.textColor
            }
        }
    }
}
