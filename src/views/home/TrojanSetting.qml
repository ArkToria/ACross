import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    implicitWidth: 680
    implicitHeight: 260

    property int fontSize: 14

    onVisibleChanged: {
        if (visible) {
            let raw = JSON.parse(nodeModel.raw)

            if (raw.hasOwnProperty("protocol")
                    && raw["protocol"] === "trojan") {
                if (!raw.hasOwnProperty("streamSettings")) {
                    return
                }

                let streamSettings = raw["streamSettings"]

                if (streamSettings.hasOwnProperty("network")) {
                    networkSelect.currentIndex = networkSelect.find(
                                streamSettings["network"])
                }

                if (streamSettings.hasOwnProperty("security")) {
                    securitySelect.currentIndex = securitySelect.find(
                                streamSettings["security"])
                }

                if (streamSettings.hasOwnProperty("tlsSettings")) {
                    if (streamSettings["tlsSettings"].hasOwnProperty(
                                "serverName")) {
                        serverNameText.text = streamSettings["tlsSettings"]["serverName"]
                    }

                    if (streamSettings["tlsSettings"].hasOwnProperty("alpn")) {
                        let alpn = streamSettings["tlsSettings"]
                        let alpn_size = alpn.property("length").toInt()
                        if (alpn_size > 1) {
                            alpnSelect.currentIndex = 0
                        } else if (alpn_size > 0) {
                            alpnSelect.currentIndex = alpnSelect.find(alpn[0])
                        }
                    }
                }
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

            text: qsTr("Trojan Setting")
            font.pointSize: fontSize
            color: acrossConfig.textColor
        }

        Label {
            text: qsTr("Host")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: serverNameText
            Layout.fillWidth: true
            Layout.columnSpan: 3
        }

        Label {
            text: qsTr("Network")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: networkSelect
            Layout.fillWidth: true
            Layout.columnSpan: 3

            model: ["tcp"]
        }

        Label {
            text: qsTr("Security")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: securitySelect
            Layout.fillWidth: true
            Layout.columnSpan: 3

            model: ["tls", "none"]
        }

        Label {
            text: qsTr("ALPN")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: alpnSelect
            Layout.fillWidth: true
            Layout.columnSpan: 3

            model: ["h2+http/1.1", "h2", "http/1.1"]
        }

        Item {
            Layout.fillHeight: true
        }
    }

    Connections {
        target: protocolSettingsLoader

        function onAcceptAll() {
            trojanSetting = {
                "serverName": serverNameText.text,
                "network": networkSelect.currentText,
                "security": securitySelect.currentText,
                "alpn": alpnSelect.currentText
            }
        }
    }
}
