import "../typescripts/home.js" as HomeJS
import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    property int fontSize: 14

    implicitWidth: 680
    implicitHeight: 260
    onVisibleChanged: {
        HomeJS.trojanComponentSetting(visible, {
            "securitySelect": securitySelect,
            "networkSelect": networkSelect,
            "alpnSelect": alpnSelect,
            "serverNameText": serverNameText
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
            onTextChanged: {
                nodeEditFormPopWindow.configChanged();
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
            model: ["tcp"]
            onEditTextChanged: {
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
            model: ["tls", "none"]
            onEditTextChanged: {
                nodeEditFormPopWindow.configChanged();
            }
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
            trojanSetting = {
                "serverName": serverNameText.text,
                "network": networkSelect.currentText,
                "security": securitySelect.currentText,
                "alpn": alpnSelect.currentText
            };
        }

        target: protocolSettingsLoader
    }

}
