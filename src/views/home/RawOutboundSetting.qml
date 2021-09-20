import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    implicitWidth: 720
    implicitHeight: 300

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing
        spacing: 0

        Label {
            Layout.margins: acrossConfig.itemSpacing

            text: qsTr("Import From Raw Outbound")
            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            Label {
                text: qsTr("Name")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                id: outboundNameText
                Layout.fillWidth: true
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: acrossConfig.itemSpacing
            clip: true
            TextAreaBox {
                id: rawOutboundText
                implicitWidth: parent.availableWidth
                implicitHeight: parent.availableHeight

                wrapMode: Text.NoWrap
            }
        }
    }

    Connections {
        target: streamSettingLoader

        function onAcceptAll() {
            rawOutboundFormModel.rawText = rawOutboundText.text
            nodeFormModel.name = outboundNameText.text
        }
    }
}
