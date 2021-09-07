import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "../components"

Item {
    anchors.margins: acrossConfig.itemSpacing * 2

    GridLayout {
        anchors.fill: parent

        columnSpacing: acrossConfig.itemSpacing
        rowSpacing: acrossConfig.itemSpacing
        columns: 4

        Label {
            Layout.fillWidth: true
            Layout.columnSpan: 4

            text: qsTr("Inbound Listening")
            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        Label {
            text: qsTr("Address")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            Layout.fillWidth: true
            Layout.leftMargin: acrossConfig.itemSpacing * 2
            Layout.columnSpan: 3

            placeholderText: acrossConfig.inboundAddress
            text: acrossConfig.inboundAddress

            onTextEdited: {
                var result = acrossConfig.testAndSetAddr(text)
                if (result) {
                    color = acrossConfig.textColor
                } else {
                    color = acrossConfig.warnColor
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 2

            GridLayout {
                anchors.fill: parent

                columnSpacing: acrossConfig.itemSpacing
                rowSpacing: acrossConfig.itemSpacing
                columns: 3

                Label {
                    text: qsTr("SOCKS5")
                    font.pixelSize: 18
                    color: acrossConfig.textColor
                }

                Item {
                    Layout.fillWidth: true
                }

                SwitchBox {
                    id: socksSwitch
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    checked: acrossConfig.socksEnable
                    onCheckedChanged: {
                        acrossConfig.socksEnable = checked
                    }
                }

                Label {

                    text: qsTr("Port")
                    color: acrossConfig.textColor
                }

                TextFieldBox {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2

                    text: acrossConfig.socksPort
                    inputMethodHints: Qt.ImhDigitsOnly
                    readOnly: socksSwitch.checked ? false : true

                    onTextEdited: {
                        acrossConfig.socksPort = text
                    }
                }

                Label {
                    text: qsTr("Username")
                    color: acrossConfig.textColor
                }

                TextFieldBox {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2

                    text: acrossConfig.socksUsername
                    readOnly: socksSwitch.checked ? false : true

                    onTextChanged: {
                        acrossConfig.socksUsername = text
                    }
                }

                Label {
                    text: qsTr("Password")
                    color: acrossConfig.textColor
                }

                TextFieldBox {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2

                    text: acrossConfig.socksPassword
                    readOnly: socksSwitch.checked ? false : true
                    echoMode: "Password"
                    onFocusChanged: {
                        if (focus) {
                            echoMode = "Normal"
                        } else {
                            echoMode = "Password"
                        }
                    }

                    onTextChanged: {
                        acrossConfig.socksPassword = text
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 2

            GridLayout {
                anchors.fill: parent

                columnSpacing: acrossConfig.itemSpacing
                rowSpacing: acrossConfig.itemSpacing
                columns: 3

                Label {
                    text: qsTr("HTTP")
                    font.pixelSize: 18
                    color: acrossConfig.textColor
                }

                Item {
                    Layout.fillWidth: true
                }

                SwitchBox {
                    id: httpSwitch

                    checked: acrossConfig.httpEnable
                    onCheckedChanged: {
                        acrossConfig.httpEnable = checked
                    }
                }

                Label {
                    text: qsTr("Port")
                    color: acrossConfig.textColor
                }

                TextFieldBox {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2

                    text: acrossConfig.httpPort
                    readOnly: httpSwitch.checked ? false : true

                    onTextChanged: {
                        acrossConfig.httpPort = text
                    }
                }

                Label {
                    text: qsTr("Username")
                    color: acrossConfig.textColor
                }

                TextFieldBox {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2

                    text: acrossConfig.httpUsername
                    readOnly: httpSwitch.checked ? false : true

                    onTextChanged: {
                        acrossConfig.httpUsername = text
                    }
                }

                Label {
                    text: qsTr("Password")
                    color: acrossConfig.textColor
                }

                TextFieldBox {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2

                    text: acrossConfig.httpPassword
                    readOnly: httpSwitch.checked ? false : true
                    echoMode: "Password"
                    onFocusChanged: {
                        if (focus) {
                            echoMode = "Normal"
                        } else {
                            echoMode = "Password"
                        }
                    }

                    onTextChanged: {
                        acrossConfig.httpPassword = text
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

