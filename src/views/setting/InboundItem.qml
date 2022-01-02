/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    property int fontSize: 14
    property int keyBoxWidth: 72

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 4
        spacing: acrossConfig.itemSpacing * 2

        Label {
            Layout.fillWidth: true
            text: qsTr("Inbound Listening")
            font.pointSize: Math.round(fontSize * 1.2)
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: addressText.height

            RowLayout {
                anchors.fill: parent
                spacing: acrossConfig.itemSpacing * 2

                Label {
                    Layout.preferredWidth: keyBoxWidth
                    text: qsTr("Address")
                    color: acrossConfig.textColor
                }

                TextFieldBox {
                    id: addressText

                    Layout.fillWidth: true
                    placeholderText: acrossConfig.inboundAddress
                    text: acrossConfig.inboundAddress
                    onTextEdited: {
                        var result = acrossConfig.testAndSetAddr(text);
                        if (result)
                            color = acrossConfig.textColor;
                        else
                            color = acrossConfig.warnColor;
                    }
                }

            }

        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                anchors.fill: parent
                spacing: acrossConfig.itemSpacing * 2

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    GridLayout {
                        anchors.fill: parent
                        columns: 4
                        columnSpacing: acrossConfig.itemSpacing * 2
                        rowSpacing: columnSpacing

                        Label {
                            Layout.preferredWidth: keyBoxWidth
                            text: qsTr("SOCKS5")
                            font.pointSize: fontSize
                            color: acrossConfig.textColor
                        }

                        Item {
                            Layout.fillWidth: true
                            Layout.columnSpan: 2
                        }

                        SwitchBox {
                            id: socksSwitch

                            onCheckedChanged: {
                                acrossConfig.socksEnable = checked;
                            }
                            Component.onCompleted: {
                                checked = Qt.binding(function() {
                                    return acrossConfig.socksEnable;
                                });
                            }
                        }

                        Label {
                            Layout.preferredWidth: keyBoxWidth
                            text: qsTr("Port")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true
                            text: acrossConfig.socksPort
                            inputMethodHints: Qt.ImhDigitsOnly
                            readOnly: socksSwitch.checked ? false : true
                            onTextEdited: {
                                acrossConfig.socksPort = text;
                            }
                        }

                        Label {
                            text: qsTr("UDP")
                            color: acrossConfig.textColor
                        }

                        SwitchBox {
                            onCheckedChanged: {
                                acrossConfig.socksUDPEnable = checked;
                            }
                            Component.onCompleted: {
                                checked = Qt.binding(function() {
                                    return acrossConfig.socksUDPEnable;
                                });
                            }
                        }

                        Label {
                            Layout.preferredWidth: keyBoxWidth
                            text: qsTr("Username")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true
                            Layout.columnSpan: 3
                            text: acrossConfig.socksUsername
                            readOnly: socksSwitch.checked ? false : true
                            onTextChanged: {
                                acrossConfig.socksUsername = text;
                            }
                        }

                        Label {
                            Layout.preferredWidth: keyBoxWidth
                            text: qsTr("Password")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true
                            Layout.columnSpan: 3
                            text: acrossConfig.socksPassword
                            readOnly: socksSwitch.checked ? false : true
                            echoMode: "Password"
                            onFocusChanged: {
                                if (focus)
                                    echoMode = "Normal";
                                else
                                    echoMode = "Password";
                            }
                            onTextChanged: {
                                acrossConfig.socksPassword = text;
                            }
                        }

                    }

                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    GridLayout {
                        anchors.fill: parent
                        columns: 4
                        columnSpacing: acrossConfig.itemSpacing * 2
                        rowSpacing: columnSpacing

                        Label {
                            Layout.preferredWidth: keyBoxWidth
                            text: qsTr("HTTP")
                            font.pointSize: fontSize
                            color: acrossConfig.textColor
                        }

                        Item {
                            Layout.fillWidth: true
                            Layout.columnSpan: 2
                        }

                        SwitchBox {
                            id: httpSwitch

                            onCheckedChanged: {
                                acrossConfig.httpEnable = checked;
                            }
                            Component.onCompleted: {
                                checked = Qt.binding(function() {
                                    return acrossConfig.httpEnable;
                                });
                            }
                        }

                        Label {
                            Layout.preferredWidth: keyBoxWidth
                            text: qsTr("Port")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true
                            Layout.columnSpan: 3
                            text: acrossConfig.httpPort
                            readOnly: httpSwitch.checked ? false : true
                            onTextChanged: {
                                acrossConfig.httpPort = text;
                            }
                        }

                        Label {
                            Layout.preferredWidth: keyBoxWidth
                            text: qsTr("Username")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true
                            Layout.columnSpan: 3
                            text: acrossConfig.httpUsername
                            readOnly: httpSwitch.checked ? false : true
                            onTextChanged: {
                                acrossConfig.httpUsername = text;
                            }
                        }

                        Label {
                            Layout.preferredWidth: keyBoxWidth
                            text: qsTr("Password")
                            color: acrossConfig.textColor
                        }

                        TextFieldBox {
                            Layout.fillWidth: true
                            Layout.columnSpan: 3
                            text: acrossConfig.httpPassword
                            readOnly: httpSwitch.checked ? false : true
                            echoMode: "Password"
                            onFocusChanged: {
                                if (focus)
                                    echoMode = "Normal";
                                else
                                    echoMode = "Password";
                            }
                            onTextChanged: {
                                acrossConfig.httpPassword = text;
                            }
                        }

                    }

                }

            }

        }

    }

}
