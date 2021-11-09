import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

CardBox {
    implicitHeight: 260

    property int fontSize: 14

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 4
        spacing: acrossConfig.itemSpacing

        Label {
            text: qsTr("Release News")
            font.pointSize: fontSize * 1.2
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
