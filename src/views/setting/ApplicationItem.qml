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
            acrossConfig.dbPath = file
        }
    }

    FileDialog {
        id: dbFileDirDialog
        title: qsTr("Select Database Path")

        onAccepted: {
            acrossConfig.dbPath = file
        }
    }

    FileDialog {
        id: backgroundImageDialog
        title: qsTr("Select Image")
        fileMode: FileDialog.OpenFile

        nameFilters: ["Image Files (*.jpg *.jpeg *.png *.svg)", "All files (*)"]

        onAccepted: {
            acrossConfig.backgroundImage = file
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
            text: qsTr("Set UserAgent")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: userAgentText
            Layout.fillWidth: true
            Layout.columnSpan: 5

            placeholderText: acrossConfig.networkUserAgent

            onTextChanged: {
                acrossConfig.networkUserAgent = text
            }
        }

        Label {
            text: qsTr("Log Level")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: logLevelText
            Layout.fillWidth: true
            Layout.columnSpan: 2

            displayText: acrossConfig.logLevel
            model: ["current", "debug", "info", "warn", "error", "none"]

            onEditTextChanged: {
                if (model.editText !== "current") {
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
            Layout.columnSpan: 2

            text: qsTr("Open Folder")
            onClicked: {
                Qt.openUrlExternally(acrossLogView.logDir())
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

        Label {
            text: qsTr("Theme")
            horizontalAlignment: Text.AlignLeft
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: themeDropDownBox
            Layout.fillWidth: true
            Layout.columnSpan: 2

            displayText: acrossConfig.currentTheme
            model: ["current", "default-light", "dark", "nord-dark", "pure-pink"]

            onEditTextChanged: {
                if (model.editText !== "current") {
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
            text: qsTr("Enable Tray Icon")
            color: acrossConfig.textColor
            font.pointSize: 12
        }

        Item {
            id: item1
            Layout.fillWidth: true
            Layout.columnSpan: 4

            Label {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left

                visible: acrossTray.isSystemTrayAvailable() ? false : true
                text: qsTr("System tray is not available")
                color: acrossConfig.warnColor
            }
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
            text: qsTr("Start From Minimized")
            color: acrossConfig.textColor
            font.pointSize: 12
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
            font.pointSize: 12
        }

        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 4
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
            text: qsTr("Enable Background Image")
            color: acrossConfig.textColor
            font.pointSize: 12
        }

        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 4
        }

        SwitchBox {
            id: enableBannerSwitch
            Layout.alignment: Qt.AlignRight

            checked: acrossConfig.enableBanner
            onCheckedChanged: {
                acrossConfig.enableBanner = checked

                if (checked) {
                    applicationItemCard.implicitHeight = 530
                } else {
                    applicationItemCard.implicitHeight = 500
                }
            }
        }

        Label {
            visible: enableBannerSwitch.checked

            text: qsTr("Background Image")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            visible: enableBannerSwitch.checked
            Layout.fillWidth: true
            Layout.columnSpan: 4

            placeholderText: acrossConfig.backgroundImage
        }

        ButtonBox {
            visible: enableBannerSwitch.checked
            Layout.fillWidth: true

            text: qsTr("Open File")

            onClicked: {
                backgroundImageDialog.open()
            }
        }

        Label {
            visible: enableBannerSwitch.checked

            text: qsTr("Opacity")
            color: acrossConfig.textColor
        }

        SliderBox {
            id: backgroundOpacitySlider
            visible: enableBannerSwitch.checked
            Layout.fillWidth: true
            Layout.columnSpan: 4

            value: acrossConfig.backgroundOpacity
            from: 0.0
            to: 1.0
            stepSize: 0.05
            snapMode: Slider.SnapOnRelease

            onValueChanged: {
                acrossConfig.backgroundOpacity = value.toFixed(2)
            }
        }

        Label {
            visible: enableBannerSwitch.checked
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            text: backgroundOpacitySlider.value.toFixed(2)
            color: acrossConfig.textColor
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

