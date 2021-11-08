import QtQuick

//import QtQuick.Dialogs
import Qt.labs.platform

import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    anchors.margins: spacingWidth

    property int spacingWidth: acrossConfig.itemSpacing * 2

    FileDialog {
        id: dbFileDialog
        title: qsTr("Select Database Path")
        fileMode: FileDialog.OpenFile

        nameFilters: ["SQLite3 Database (*.db)", "All files (*)"]
        onAccepted: {
            acrossConfig.dbPath = currentFile
        }
    }

    FolderDialog {
        id: dbFileDirDialog
        title: qsTr("Select Database Path")

        onAccepted: {
            acrossConfig.dbPath = currentFolder
        }
    }

    GridLayout {
        anchors.fill: parent

        columns: 6
        rowSpacing: acrossConfig.itemSpacing
        columnSpacing: acrossConfig.itemSpacing

        Label {
            Layout.fillWidth: true
            Layout.columnSpan: 6

            text: qsTr("Application Settings")
            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        Label {
            text: qsTr("Data Directory")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            Layout.fillWidth: true
            Layout.columnSpan: 4

            placeholderText: acrossConfig.dbPath
                             === "" ? qsTr("Enter Database Path Here") : acrossConfig.dbPath
        }

        ButtonBox {
            text: qsTr("Open")
            onClicked: {
                dbFileDirDialog.open()
            }
        }

        Label {
            text: qsTr("Set UserAgent")
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

            onClicked: {
                acrossConfig.networkUserAgent = userAgentText.text
            }
        }

        Label {
            text: qsTr("Log Mode")
            color: acrossConfig.textColor
        }

        DropDownBox {
            Layout.fillWidth: true
            Layout.columnSpan: 2

            model: ["stdout", "file", "both"]
        }

        Label {
            text: qsTr("Max Lines")
            color: acrossConfig.textColor
        }

        NumBox {
            Layout.fillWidth: true
            Layout.columnSpan: 2

            value: acrossConfig.logLines
            from: 1
            to: 100000

            onFocusChanged: {
                acrossConfig.logLines = value
            }
        }

        Label {
            text: qsTr("Enable Tray Icon")
            color: acrossConfig.textColor
        }

        Label {
            Layout.fillWidth: true
            Layout.columnSpan: 4

            text: acrossTray.isSystemTrayAvailable(
                      ) ? "" : qsTr("System tray is not available")
            color: acrossConfig.warnColor
        }

        SwitchBox {
            id: enableTrayIcon
            Layout.alignment: Qt.AlignRight

            checked: acrossConfig.enableTray
            onCheckedChanged: {
                acrossConfig.enableTray = checked
            }
        }

        Label {
            text: qsTr("Minimize startup")
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 4
        }

        SwitchBox {
            id: enableStartFromMinimized
            Layout.alignment: Qt.AlignRight

            checked: acrossConfig.enableStartFromMinimized
            onCheckedChanged: {
                acrossConfig.enableStartFromMinimized = checked
            }
        }

        Label {
            text: qsTr("Auto Connect")
            color: acrossConfig.textColor
        }

        Label {
            Layout.fillWidth: true
            Layout.columnSpan: 4
            text: qsTr("Default Node > Last Connected. (Subscription updates will reset the default node)")
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
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

