import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    implicitWidth: 680
    implicitHeight: 300

    property int fontSize: 14

    onVisibleChanged: {
        if (visible) {
            let raw = JSON.parse(nodeModel.raw)
            console.log(nodeModel.raw)

            if (raw.hasOwnProperty("protocol") && raw["protocol"] === "vmess") {

            } else {
                return
            }

            if (raw.hasOwnProperty("settings")) {
                let vmess
                if (raw["settings"].hasOwnProperty("vmess"))
                    vmess = raw["settings"]["vmess"]["vnext"]
                else
                    vmess = raw["settings"]["vnext"]

                if (Object.keys(vmess).length > 0) {
                    let server = vmess[0]
                    if (server.hasOwnProperty("users") && Object.keys(
                                server["users"]).length > 0) {
                        let user = server["users"][0]
                        if (user.hasOwnProperty("alterId")) {
                            alterIDText.text = user["alterId"]
                        }

                        if (user.hasOwnProperty("security")) {
                            securitySelect.currentIndex = securitySelect.find(
                                        user["security"])
                        }
                    }
                }
            }

            if (!raw.hasOwnProperty("streamSettings")) {
                return
            }

            let streamSettings = raw["streamSettings"]
            if (streamSettings.hasOwnProperty("network")) {
                let network = streamSettings["network"]
                networkSelect.currentIndex = networkSelect.find(network)

                switch (network) {
                case "ws":
                    if (streamSettings.hasOwnProperty("wsSettings")) {
                        let wsSettings = streamSettings["wsSettings"]

                        if (wsSettings.hasOwnProperty("path")) {
                            pathText.text = wsSettings["path"]
                        }

                        if (wsSettings.hasOwnProperty("headers")
                                && wsSettings["headers"].hasOwnProperty(
                                    "Host")) {
                            hostText.text = wsSettings["headers"]["Host"]
                        }
                    }
                    break
                default:
                    break
                }
            }

            if (streamSettings.hasOwnProperty("security")) {
                if (streamSettings["security"] === "tls")
                    tlsEnableSelect.checked = true
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

            text: qsTr("VMESS Setting")
            font.pointSize: fontSize
            color: acrossConfig.textColor
        }

        Label {
            text: qsTr("Enable TLS")
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }

        SwitchBox {
            id: tlsEnableSelect

            onCheckedChanged: {
                nodeEditFormPopWindow.configChanged()
            }
        }

        Label {
            text: qsTr("AlterID")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: alterIDText
            Layout.fillWidth: true
            Layout.columnSpan: 3

            placeholderText: "[0-65535] 0: auto enable VMessAEAD"
            validator: IntValidator {
                bottom: 0
                top: 65535
            }

            onTextChanged: {
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

            model: ["auto", "aes-128-gcm", "chacha20-poly1305", "none", "zero"]

            onEditTextChanged: {
                nodeEditFormPopWindow.configChanged()
            }
        }

        Label {
            text: qsTr("Network")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: networkSelect
            Layout.fillWidth: true
            Layout.columnSpan: 3

            model: ["tcp", "kcp", "ws", "http", "domainsocket", "quic", "grpc"]

            onEditTextChanged: {
                nodeEditFormPopWindow.configChanged()
            }
        }

        Label {
            text: qsTr("Host")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: hostText
            Layout.fillWidth: true
            Layout.columnSpan: 3

            color: acrossConfig.textColor

            onTextChanged: {
                nodeEditFormPopWindow.configChanged()
            }
        }

        Label {
            text: qsTr("Path")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: pathText
            Layout.fillWidth: true
            Layout.columnSpan: 3

            color: acrossConfig.textColor

            onTextChanged: {
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
            vmessSetting = {
                "enableTLS": tlsEnableSelect.checked,
                "alterID": alterIDText.text,
                "security": securitySelect.currentText,
                "network": networkSelect.currentText,
                "host": hostText.text,
                "path": pathText.text
            }
        }
    }
}
