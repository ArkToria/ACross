import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

CardBox {
    implicitHeight: 260

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 2
        spacing: acrossConfig.itemSpacing

        Label {
            text: qsTr("Release News")
            font.pixelSize: 18
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
