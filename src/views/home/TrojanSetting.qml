import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

Item {
    implicitWidth: 720
    implicitHeight: 260

    property int textBoxWidth: 72

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing
        spacing: 0

        Label {
            Layout.margins: acrossConfig.itemSpacing

            text: qsTr("Trojan Setting")
            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Host")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                id: serverNameText
                Layout.fillWidth: true
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Network")
                color: acrossConfig.textColor
            }

            DropDownBox {
                id: networkSelect
                Layout.fillWidth: true

                model: ["tcp"]
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Security")
                color: acrossConfig.textColor
            }

            DropDownBox {
                id: securitySelect
                Layout.fillWidth: true

                model: ["tls", "none"]
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("ALPN")
                color: acrossConfig.textColor
            }

            DropDownBox {
                id: alpnSelect
                Layout.fillWidth: true

                model: ["h2+http/1.1", "h2", "http/1.1"]
            }
        }
    }

    Connections {
        target: streamSettingLoader

        function onAcceptAll() {
            trojanFormModel.serverName = serverNameText.text
            trojanFormModel.network = networkSelect.editText
            trojanFormModel.security = securitySelect.editText
            trojanFormModel.alpn = alpnSelect.editText
        }
    }
}
