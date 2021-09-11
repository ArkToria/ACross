import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

Item {
    anchors.margins: spacingWidth

    property int spacingWidth: acrossConfig.itemSpacing * 2

    FileDialog {
        id: dbFileDialog
        title: qsTr("Select Database Path")
        selectFolder: false
        selectExisting: true
        selectMultiple: false
        nameFilters: ["SQLite3 Database (*.db)", "All files (*)"]
        onAccepted: {
            acrossConfig.dbPath = fileUrl
        }
    }

    FileDialog {
        id: dbFileDirDialog
        title: qsTr("Select Database Path")
        selectFolder: true
        selectExisting: false
        selectMultiple: false
        onAccepted: {
            acrossConfig.dbPath = fileUrl
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

            text: qsTr("Application Setting")
            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        Label {
            text: qsTr("Theme")
            horizontalAlignment: Text.AlignLeft
            color: acrossConfig.textColor
        }

        DropDownBox {
            Layout.fillWidth: true
            Layout.columnSpan: 2

            displayText: acrossConfig.currentTheme
            model: ["current", "default-light", "dark", "nord-dark"]

            onEditTextChanged: {
                if (editText !== "current") {
                    acrossConfig.currentTheme = editText
                }
            }
        }

        Label {
            text: qsTr("Language")
            color: acrossConfig.textColor
        }

        DropDownBox {
            Layout.fillWidth: true
            Layout.columnSpan: 2

            displayText: acrossConfig.currentLanguage
            model: ["current", "en_US", "zh_CN"]

            onEditTextChanged: {
                acrossConfig.currentLanguage = editText
            }
        }

        Label {
            text: qsTr("Log Level")
            color: acrossConfig.textColor
        }

        DropDownBox {
            Layout.fillWidth: true
            Layout.columnSpan: 2

            displayText: acrossConfig.logLevel
            model: ["current", "debug", "info", "warn", "error", "none"]

            onEditTextChanged: {
                if (editText !== "current") {
                    acrossConfig.logLevel = editText
                }
            }
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
            text: qsTr("Log Output")
            color: acrossConfig.textColor
        }

        DropDownBox {
            Layout.fillWidth: true
            Layout.columnSpan: 2

            model: ["stdout", "file", "both"]
        }

        Label {
            text: qsTr("Log File")
            color: acrossConfig.textColor
        }

        ButtonBox {
            Layout.fillWidth: true

            text: qsTr("Open File")
            onClicked: {
                dbFileDialog.open()
            }
        }

        ButtonBox {
            Layout.fillWidth: true

            text: qsTr("Folder")
            onClicked: {

            }
        }

        Label {
            text: qsTr("DB Path")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            Layout.fillWidth: true
            Layout.columnSpan: 3

            placeholderText: acrossConfig.dbPath
                             === "" ? qsTr("Enter Database Path Here") : acrossConfig.dbPath
        }

        ButtonBox {
            Layout.fillWidth: true

            text: qsTr("Open File")
            onClicked: {
                dbFileDialog.open()
            }
        }

        ButtonBox {
            Layout.fillWidth: true

            text: qsTr("Folder")
            onClicked: {
                dbFileDirDialog.open()
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

