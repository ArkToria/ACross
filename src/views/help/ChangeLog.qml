import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "../components"

CardBox {
    implicitHeight: 260

    property int fontSize: 12

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 2
        spacing: acrossConfig.itemSpacing

        Label {
            text: qsTr("Release News")
            font.pointSize: 12
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
