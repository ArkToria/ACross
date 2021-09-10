import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

import ACross 1.0

CardBox {
    implicitWidth: 680
    implicitHeight: 300

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 2

        spacing: acrossConfig.itemSpacing

        Label {
            text: qsTr("Core Log")

            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        CardBox {
            layer.enabled: false
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: acrossConfig.deepColor

            ListView {
                id: listView
                anchors.fill: parent

                model: LogModel {
                    list: acrossLogList
                }

                delegate: Text {
                    width: listView.width

                    text: model.log
                    color: acrossConfig.deepTextColor
                    font.family: "Mono"
                    textFormat: TextEdit.RichText
                    wrapMode: Text.NoWrap
                }
            }
        }
    }
}
