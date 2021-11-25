import QtQuick

//import QtQuick.Dialogs
import Qt.labs.platform

import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    property int fontSize: 14
    property int keyBoxWidth: 72

    FolderDialog {
        id: dataDirDialog
        title: qsTr("Select Data Folder")

        onAccepted: {
            acrossConfig.dataDir = currentFolder
        }
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 4

        columns: 6
        rowSpacing: acrossConfig.itemSpacing * 2
        columnSpacing: acrossConfig.itemSpacing * 2

        Label {
            Layout.fillWidth: true
            Layout.columnSpan: 6

            text: qsTr("Application Settings")
            font.pointSize: Math.round(fontSize * 1.2)
            color: acrossConfig.textColor
        }

        Label {
            text: qsTr("Data Directory")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            Layout.fillWidth: true
            Layout.columnSpan: 4

            selectByMouse: true
            readOnly: true
            placeholderText: acrossConfig.dataDir
        }

        ButtonBox {
            Layout.alignment: Qt.AlignRight

            text: qsTr("Open")
            onClicked: {
                dataDirDialog.open()
            }
        }

        Label {
            text: qsTr("Request Header")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: userAgentText
            Layout.fillWidth: true
            Layout.columnSpan: 4

            placeholderText: acrossConfig.networkUserAgent
        }

        ButtonBox {
            text: qsTr("Accept")
            Layout.alignment: Qt.AlignRight

            onClicked: {
                acrossConfig.networkUserAgent = userAgentText.text
            }
        }

        Label {
            text: qsTr("Auto Connect")
            color: acrossConfig.textColor
        }

        Label {
            Layout.fillWidth: true
            Layout.columnSpan: 4

            text: qsTr("Default > Last Connected (Updating groups will reset the default node)")
            color: acrossConfig.highlightColor
        }

        SwitchBox {
            id: enableAutoConnect
            Layout.alignment: Qt.AlignRight

            checked: acrossConfig.enableAutoConnect
            onCheckedChanged: {
                acrossConfig.enableAutoConnect = checked
            }
        }

        Label {
            text: qsTr("Tray Icon")
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.preferredHeight: enableTrayIcon.height

            RowLayout {
                anchors.fill: parent
                spacing: acrossConfig.itemSpacing * 2

                Label {
                    Layout.fillWidth: true

                    text: acrossTray.isSystemTrayAvailable(
                              ) ? "" : qsTr("Unavailable")
                    color: acrossConfig.warnColor
                }

                SwitchBox {
                    id: enableTrayIcon
                    Layout.alignment: Qt.AlignRight

                    foregroundColor: acrossConfig.highlightTextColor
                    checked: acrossConfig.enableTray

                    onCheckedChanged: {
                        acrossConfig.enableTray = checked
                    }
                }
            }
        }

        Label {
            text: qsTr("Minimize startup")
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.preferredHeight: enableTrayIcon.height

            RowLayout {
                anchors.fill: parent
                spacing: acrossConfig.itemSpacing * 2

                Item {
                    Layout.fillWidth: true
                }

                SwitchBox {
                    id: enableStartFromMinimized
                    Layout.alignment: Qt.AlignRight

                    checked: acrossConfig.enableStartFromMinimized

                    onCheckedChanged: {
                        acrossConfig.enableStartFromMinimized = checked
                    }
                }
            }
        }

        Label {
            text: qsTr("Log Outputs")
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.preferredHeight: logOutputsText.height

            RowLayout {
                anchors.fill: parent
                spacing: acrossConfig.itemSpacing * 2

                DropDownBox {
                    id: logOutputsText
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    Layout.alignment: Qt.AlignRight

                    model: ["current", "none", "stdout", "file", "stdout & file"]
                    displayText: acrossConfig.logMode

                    onEditTextChanged: {
                        if (currentText !== "current")
                            acrossConfig.logMode = currentText
                    }
                }
            }
        }

        Label {
            text: qsTr("Max Lines")
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.preferredHeight: logOutputsText.height

            RowLayout {
                anchors.fill: parent
                spacing: acrossConfig.itemSpacing * 2

                NumBox {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    Layout.alignment: Qt.AlignRight

                    value: acrossConfig.logLines
                    from: 1
                    to: 10000

                    onValueChanged: {
                        acrossConfig.logLines = value
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

