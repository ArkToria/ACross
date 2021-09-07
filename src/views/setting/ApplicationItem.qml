import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

Item {
    anchors.margins: spacingWidth

    property int textBoxWidth: 72
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

    ColumnLayout {
        anchors.fill: parent
        spacing: spacingWidth

        Label {
            text: qsTr("Application Setting")
            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        GridLayout {
            columns:4
            rowSpacing: acrossConfig.itemSpacing

            Label {
                text: qsTr("Language")
                color: acrossConfig.textColor
            }

            DropDownBox {
                Layout.fillWidth: true

                displayText: acrossConfig.currentLanguage
                model: ["current", "en_US", "zh_CN"]

                onEditTextChanged: {
                    acrossConfig.currentLanguage = editText
                }
            }

            Label {
                text: qsTr("Theme")
                horizontalAlignment: Text.AlignLeft
                color: acrossConfig.textColor
            }

            DropDownBox {
                Layout.fillWidth: true

                displayText: acrossConfig.currentTheme
                model: ["current", "default-light", "dark", "nord-dark"]

                onEditTextChanged: {
                    if (editText !== "current") {
                        acrossConfig.currentTheme = editText
                    }
                }
            }

            Label {
                text: qsTr("Log Output")
                color: acrossConfig.textColor
            }

            DropDownBox {
                Layout.fillWidth: true

                model: ["stdout", "file", "both"]
            }

            Label {
                text: qsTr("Log Level")
                color: acrossConfig.textColor
            }

            DropDownBox {
                Layout.fillWidth: true

                displayText: acrossConfig.logLevel
                model: ["current", "debug", "info", "warn", "error", "none"]

                onEditTextChanged: {
                    if (editText !== "current") {
                        acrossConfig.logLevel = editText
                    }
                }
            }
        }

        RowLayout {
            spacing: spacingWidth

            Label {
                text: qsTr("DB Path")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                placeholderText: acrossConfig.dbPath
                                 === "" ? qsTr("Enter Database Path Here") : acrossConfig.dbPath
                Layout.fillWidth: true
            }

            ButtonBox {
                text: qsTr("Open File")
                onClicked: {
                    dbFileDialog.open()
                }
            }

            ButtonBox {
                text: qsTr("Folder")
                onClicked: {
                    dbFileDirDialog.open()
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

