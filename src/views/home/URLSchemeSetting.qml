import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    id: urlSchemeSetting
    implicitWidth: 720
    implicitHeight: 160

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing
        spacing: 0

        Label {
            Layout.margins: acrossConfig.itemSpacing

            text: qsTr("Import From URL")
            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: urlTextField
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing
            clip: true

            focus: true
            wrapMode: Text.NoWrap
            placeholderText: "support url scheme: ss:// vmess:// trojan://"
        }
    }

    Connections {
        target: streamSettingLoader

        function onAcceptAll() {
            urlSchemeFormModel.urlScheme = urlTextField.text
        }
    }
}
