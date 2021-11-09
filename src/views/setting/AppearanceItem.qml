import QtQuick

//import QtQuick.Dialogs
import Qt.labs.platform

import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    id: control
    anchors.margins: spacingWidth

    property int spacingWidth: acrossConfig.itemSpacing * 2
    property int fontSize: 14

    FileDialog {
        id: backgroundImageDialog
        title: qsTr("Select Image")
        fileMode: FileDialog.OpenFile

        nameFilters: ["Image Files (*.jpg *.jpeg *.png *.svg)", "All files (*)"]

        onAccepted: {
            acrossConfig.backgroundImage = currentFile
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

            text: qsTr("Appearance Settings")
            font.pointSize: fontSize * 1.2
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

            displayText: acrossConfig.currentThemeName
            model: ["current", "default-light", "dark", "nord-dark", "pure-pink"]

            onEditTextChanged: {
                if (model.editText !== "current") {
                    acrossConfig.currentThemeName = editText
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
            Layout.fillWidth: true
            Layout.columnSpan: 5

            text: qsTr("Background Image")
            color: acrossConfig.textColor
            font.pointSize: fontSize
        }

        SwitchBox {
            id: enableBannerSwitch
            Layout.alignment: Qt.AlignRight

            checked: acrossConfig.enableBanner
            onCheckedChanged: {
                acrossConfig.enableBanner = checked
            }
        }

        Label {
            text: qsTr("Source File")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            Layout.fillWidth: true
            Layout.columnSpan: 4

            readOnly: true
            placeholderText: acrossConfig.backgroundImage
        }

        ButtonBox {
            Layout.alignment: Qt.AlignRight

            enabled: enableBannerSwitch.checked
            text: qsTr("Open")

            onClicked: {
                backgroundImageDialog.open()
            }
        }

        Label {
            text: qsTr("Opacity")
            color: acrossConfig.textColor
        }

        SliderBox {
            id: backgroundOpacitySlider
            Layout.fillWidth: true
            Layout.columnSpan: 4

            enabled: enableBannerSwitch.checked
            value: acrossConfig.backgroundOpacity
            from: 0.0
            to: 1.0
            stepSize: 0.05
            snapMode: Slider.SnapOnRelease

            onValueChanged: {
                acrossConfig.backgroundOpacity = value.toFixed(2)
            }
        }

        TextFieldBox {
            Layout.alignment: Qt.AlignRight
            Layout.preferredWidth: 72
            horizontalAlignment: TextInput.AlignHCenter

            text: backgroundOpacitySlider.value.toFixed(2)
            color: acrossConfig.textColor
            readOnly: true
        }
    }
}
