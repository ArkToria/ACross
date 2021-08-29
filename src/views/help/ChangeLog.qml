import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

CardBox {
    implicitWidth: scrollView.availableWidth - acrossConfig.itemSpacing
    implicitHeight: 260

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 4
        spacing: acrossConfig.itemSpacing * 4

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
