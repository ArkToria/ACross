import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    implicitWidth: 680
    implicitHeight: 160

    property int fontSize: 14

    onVisibleChanged: {
        if (visible) {
            let raw = JSON.parse(nodeModel.raw)

            if (raw.hasOwnProperty("protocol")
                    && raw["protocol"] === "shadowsocks") {

            } else {
                return
            }

            let server
            if (raw["settings"].hasOwnProperty("shadowsocks")) {
                server = raw["settings"]["shadowsocks"]["servers"][0]
            } else {
                server = raw["settings"]["servers"][0]
            }

            securitySelect.currentIndex = securitySelect.find(server["method"])

            if (server.hasOwnProperty("ivCheck")) {
                ivCheckSelect.checked = server["ivCheck"]
            }
        }
    }

    GridLayout {
        anchors.fill: parent

        columns: 4
        rowSpacing: acrossConfig.itemSpacing
        columnSpacing: acrossConfig.itemSpacing

        Label {
            Layout.fillWidth: true
            Layout.columnSpan: 4

            text: qsTr("Shadowsocks Setting")
            font.pointSize: fontSize
            color: acrossConfig.textColor
        }

        Label {
            text: qsTr("IVCheck")
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }

        SwitchBox {
            id: ivCheckSelect

            onCheckedChanged: {
                nodeEditFormPopWindow.configChanged()
            }
        }

        Label {
            text: qsTr("Security")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: securitySelect
            Layout.fillWidth: true
            Layout.columnSpan: 3

            model: ["aes-256-gcm", "aes-128-gcm", "chacha20-ietf-poly1305", "chacha20-poly1305", "plain", "none"]

            onEditTextChanged: {
                nodeEditFormPopWindow.configChanged()
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }

    Connections {
        target: protocolSettingsLoader

        function onAcceptAll() {
            shadowsocksSetting = {
                "ivCheck": ivCheckSelect.checked,
                "security": securitySelect.currentText
            }
        }
    }
}
