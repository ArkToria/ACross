import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "../components"

Item {
    anchors.margins: spacingWidth

    property int spacingWidth: acrossConfig.itemSpacing * 2

    ColumnLayout {
        anchors.fill: parent

        spacing: spacingWidth

        Label {
            text: qsTr("Inbound Listening")
            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        RowLayout {
            Layout.fillWidth: true

            spacing: spacingWidth

            Label {
                text: qsTr("Address")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                Layout.fillWidth: true

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
        }

        RowLayout {
            Layout.fillWidth: true

            spacing: spacingWidth

            ColumnLayout {
                Layout.preferredWidth: (parent.width-spacingWidth)/2
                Layout.fillHeight: true

                spacing: spacingWidth

                RowLayout {
                    implicitWidth: parent.width

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
                }

                GridLayout {
                    columns:2
                    rowSpacing: spacingWidth
                    implicitWidth: parent.width

                    Label {

                        text: qsTr("Port")
                        color: acrossConfig.textColor
                    }

                    TextFieldBox {
                        Layout.fillWidth: true

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

            ColumnLayout {
                Layout.preferredWidth: (parent.width-spacingWidth)/2
                Layout.fillHeight: true

                spacing: spacingWidth

                RowLayout {
                    implicitWidth: parent.width

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
                }

                GridLayout {
                    columns:2
                    rowSpacing: spacingWidth
                    implicitWidth: parent.width

                    Label {
                        text: qsTr("Port")
                        color: acrossConfig.textColor
                    }

                    TextFieldBox {
                        Layout.fillWidth: true
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

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: spacingWidth

            property int buttonWidth: 96

            Item {
                Layout.fillWidth: true
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

