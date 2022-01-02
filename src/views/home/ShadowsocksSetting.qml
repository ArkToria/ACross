import "../typescripts/home.js" as HomeJS
import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    property int fontSize: 14

    implicitWidth: 680
    implicitHeight: 160
    onVisibleChanged: {
        HomeJS.shadowsocksComponentSetting(visible, {
            "securitySelect": securitySelect,
            "ivCheckSelect": ivCheckSelect
        }, nodeModel);
    }

    GridLayout {
        anchors.fill: parent
        columns: 4
        rowSpacing: acrossConfig.itemSpacing * 2
        columnSpacing: acrossConfig.itemSpacing * 2

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
                nodeEditFormPopWindow.configChanged();
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
                nodeEditFormPopWindow.configChanged();
            }
        }

        Item {
            Layout.fillHeight: true
        }

    }

    Connections {
        function onAcceptAll() {
            shadowsocksSetting = {
                "ivCheck": ivCheckSelect.checked,
                "security": securitySelect.currentText
            };
        }

        target: protocolSettingsLoader
    }

}
