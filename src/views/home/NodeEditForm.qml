import Arktoria.ACross
//import QtQuick.Dialogs
import Qt.labs.platform
import Qt5Compat.GraphicalEffects
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic as ControlsBasic
import QtQuick.Layouts
import QtQuick.Window

Window {
    id: nodeEditFormPopWindow

    property var nodeModel: null
    property int fontSize: 14

    signal configChanged()

    width: 960
    height: 680
    minimumWidth: 680
    minimumHeight: 420
    title: qsTr("Edit Configuration")
    flags: Qt.platform.os === "windows" ? Qt.Window : Qt.WindowStaysOnTopHint
    modality: Qt.platform.os === "osx" ? Qt.NonModal : Qt.ApplicationModal
    onVisibilityChanged: {
        if (visible) {
            if (nodeModel !== null) {
                jsonPreview.text = acrossNodes.jsonFormat(nodeModel.raw);
                urlTextField.text = nodeModel.url;
                manualNameText.text = nodeModel.name;
                manualAddressText.text = nodeModel.address;
                manualPortText.text = nodeModel.port;
                manualPasswordText.text = nodeModel.password;
                manualProtocolType.currentIndex = nodeModel.protocol;
                importConfigName.text = nodeModel.name;
            }
        } else {
            nodeEditFormPopWindow.close();
            nodeEditFormPopWindow.destroy();
        }
    }
    onConfigChanged: {
        let variantMap = {
            "type": bar.currentIndex,
            "url": urlSetting.accept(),
            "manual": manualSetting.accept(),
            "config": importConfigSetting.accept()
        };
        if (nodeModel !== null)
            variantMap["nodeID"] = nodeModel.nodeID;

        jsonPreview.text = acrossNodes.jsonFormat(nodeFormModel.refreshPreview(variantMap));
    }

    NodeFormModel {
        id: nodeFormModel

        list: acrossNodes
    }

    Rectangle {
        anchors.fill: parent
        color: acrossConfig.deepColor

        CardBox {
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing

            GridLayout {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing * 4
                columns: 2
                rows: 3
                columnSpacing: acrossConfig.itemSpacing * 2
                rowSpacing: acrossConfig.itemSpacing * 2

                ControlsBasic.ScrollView {
                    Layout.fillHeight: true
                    Layout.preferredWidth: parent.width / 2
                    Layout.rowSpan: 3

                    TextAreaBox {
                        id: jsonPreview

                        width: parent.availableWidth
                        height: parent.availableHeight
                        selectByMouse: true
                        readOnly: true
                        font: fixedFont
                        Component.onCompleted: {
                            acrossNodes.setDocument(jsonPreview.textDocument);
                        }
                    }

                }

                TabBar {
                    id: bar

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop

                    Repeater {
                        model: [qsTr("Decode URL"), qsTr("Manual Setting"), qsTr("Import Config")]

                        TabButton {
                            id: tabButton

                            MouseArea {
                                anchors.fill: tabButton
                                hoverEnabled: true
                                onEntered: {
                                    cursorShape = Qt.PointingHandCursor;
                                }
                                onExited: {
                                    cursorShape = Qt.ArrowCursor;
                                }
                                onClicked: {
                                    bar.currentIndex = index;
                                }
                            }

                            contentItem: Text {
                                text: modelData
                                color: bar.currentIndex === model.index ? acrossConfig.textColor : acrossConfig.deepTextColor
                                horizontalAlignment: Text.AlignHCenter
                            }

                            background: Rectangle {
                                implicitHeight: 24
                                color: bar.currentIndex === model.index ? acrossConfig.backgroundColor : acrossConfig.deepColor
                            }

                        }

                    }

                }

                StackLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop
                    currentIndex: bar.currentIndex

                    Item {
                        id: urlSetting

                        function accept() {
                            return urlTextField.text;
                        }

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: acrossConfig.itemSpacing * 2

                            TextAreaBox {
                                id: urlTextField

                                Layout.fillWidth: true
                                wrapMode: Text.WrapAnywhere
                                placeholderText: "support url scheme: ss:// vmess:// trojan://"
                                onTextChanged: {
                                    configChanged();
                                }
                            }

                            Item {
                                Layout.fillHeight: true
                            }

                        }

                    }

                    Item {
                        id: manualSetting

                        function accept() {
                            protocolSettingsLoader.acceptAll();
                            return {
                                "name": manualNameText.text,
                                "address": manualAddressText.text,
                                "port": manualPortText.text,
                                "password": manualPasswordText.text,
                                "protocol": manualProtocolType.currentIndex,
                                "vmess": protocolSettingsLoader.vmessSetting,
                                "shadowsocks": protocolSettingsLoader.shadowsocksSetting,
                                "trojan": protocolSettingsLoader.trojanSetting
                            };
                        }

                        GridLayout {
                            anchors.fill: parent
                            columns: 4
                            rowSpacing: acrossConfig.itemSpacing * 2
                            columnSpacing: acrossConfig.itemSpacing * 2

                            Label {
                                Layout.fillWidth: true
                                Layout.columnSpan: 4
                                text: qsTr("Outbound Config")
                                color: acrossConfig.textColor
                                font.pointSize: fontSize
                            }

                            Label {
                                text: qsTr("Name")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                id: manualNameText

                                Layout.fillWidth: true
                                Layout.columnSpan: 3
                                onTextChanged: {
                                    configChanged();
                                }
                            }

                            Label {
                                text: qsTr("Address")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                id: manualAddressText

                                Layout.fillWidth: true
                                onTextChanged: {
                                    configChanged();
                                }
                            }

                            Label {
                                text: qsTr("Port")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                id: manualPortText

                                Layout.minimumWidth: 72
                                placeholderText: "443"
                                onTextChanged: {
                                    configChanged();
                                }

                                validator: IntValidator {
                                    bottom: 0
                                    top: 65535
                                }

                            }

                            Label {
                                text: qsTr("Password")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                id: manualPasswordText

                                Layout.fillWidth: true
                                Layout.columnSpan: 3
                                onTextChanged: {
                                    configChanged();
                                }
                            }

                            Label {
                                text: qsTr("Protocol")
                                color: acrossConfig.textColor
                            }

                            DropDownBox {
                                id: manualProtocolType

                                Layout.fillWidth: true
                                Layout.columnSpan: 3
                                model: ["vmess", "shadowsocks", "trojan"]
                                onEditTextChanged: {
                                    switch (currentIndex) {
                                    case 0:
                                        protocolSettingsLoader.source = "qrc:/Arktoria/ACross/src/views/home/VMESSSetting.qml";
                                        break;
                                    case 1:
                                        protocolSettingsLoader.source = "qrc:/Arktoria/ACross/src/views/home/ShadowsocksSetting.qml";
                                        break;
                                    case 2:
                                        protocolSettingsLoader.source = "qrc:/Arktoria/ACross/src/views/home/TrojanSetting.qml";
                                        break;
                                    default:
                                        console.log("unknown protocol");
                                        configChanged();
                                    }
                                }
                            }

                            Loader {
                                id: protocolSettingsLoader

                                property var vmessSetting: null
                                property var shadowsocksSetting: null
                                property var trojanSetting: null

                                signal acceptAll()

                                Layout.fillWidth: true
                                Layout.columnSpan: 4
                            }

                            Item {
                                Layout.fillHeight: true
                            }

                        }

                    }

                    Item {
                        id: importConfigSetting

                        function accept() {
                            return {
                                "name": importConfigName.text,
                                "filePath": importConfigPath.text
                            };
                        }

                        FileDialog {
                            id: importConfigDialog

                            title: qsTr("Import v2ray config from file")
                            fileMode: FileDialog.OpenFile
                            nameFilters: ["v2ray config (*.json)", "All files (*)"]
                            onAccepted: {
                                importConfigPath.text = file;
                                configChanged();
                            }
                        }

                        GridLayout {
                            anchors.fill: parent
                            columns: 4
                            columnSpacing: acrossConfig.itemSpacing * 2
                            rowSpacing: acrossConfig.itemSpacing * 2

                            Label {
                                text: qsTr("Node Name")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                id: importConfigName

                                Layout.fillWidth: true
                                Layout.columnSpan: 3
                                onTextChanged: {
                                    configChanged();
                                }
                            }

                            Label {
                                text: qsTr("Config File")
                                color: acrossConfig.textColor
                            }

                            TextFieldBox {
                                id: importConfigPath

                                Layout.fillWidth: true
                                Layout.columnSpan: 2
                                placeholderText: qsTr("Enter Config Path Here")
                                onTextChanged: {
                                    configChanged();
                                }
                            }

                            ButtonBox {
                                text: qsTr("Open")
                                onClicked: {
                                    importConfigDialog.open();
                                }
                            }

                            Item {
                                Layout.fillHeight: true
                            }

                        }

                    }

                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignBottom
                    spacing: acrossConfig.itemSpacing

                    Item {
                        Layout.fillWidth: true
                    }

                    ButtonBox {
                        id: acceptFormButton

                        text: qsTr("Accept")
                        onClicked: {
                            let variantMap = {
                                "type": bar.currentIndex,
                                "url": urlSetting.accept(),
                                "manual": manualSetting.accept(),
                                "config": importConfigSetting.accept()
                            };
                            if (nodeModel !== null)
                                variantMap["nodeID"] = nodeModel.nodeID;

                            nodeFormModel.accept(variantMap);
                            nodeEditFormPopWindow.close();
                        }
                    }

                    ButtonBox {
                        text: qsTr("Cancel")
                        basicColor: acrossConfig.warnColor
                        basicState: "WarnState"
                        onClicked: {
                            nodeEditFormPopWindow.close();
                        }
                    }

                }

            }

        }

    }

}
