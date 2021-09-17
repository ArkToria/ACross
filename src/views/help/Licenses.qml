import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

CardBox {
    implicitHeight: 280
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 2
        spacing: acrossConfig.itemSpacing

        Label {
            text: qsTr("Libraries and Licenses")
            font.pixelSize: 18
            color: acrossConfig.textColor
        }

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            spacing: acrossConfig.itemSpacing

            model: ListModel {
                ListElement {
                    key: "Qt"
                    value: "Copyright (C) 2020 The Qt Company Ltd and other contributors. Under the GNU (L)GPL"
                }
                ListElement {
                    key: "Icons"
                    value: "Google Material Icons. Apache licenses 2.0"
                }
            }

            delegate: Label {
                text: key + " - " + value
                color: acrossConfig.textColor
            }
        }
    }
}
