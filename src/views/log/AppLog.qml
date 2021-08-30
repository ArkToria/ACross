import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

CardBox {
    implicitWidth: 680
    implicitHeight: 300

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 2

        spacing: acrossConfig.itemSpacing

        Label {
            text: qsTr("Application Log")

            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        TextAreaBox {
            Layout.fillHeight: true
            Layout.fillWidth: true

            placeholderText: "application logging ..."
            color: acrossConfig.deepTextColor
        }
    }
}
