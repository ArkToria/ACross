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
            font.pixelSize: 24
            color: acrossConfig.textColor
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
            text: qsTr("Theme")
            horizontalAlignment: Text.AlignLeft
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: themeDropDownBox
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
                    appearanceItemCard.implicitHeight = 230
                } else {
                    appearanceItemCard.implicitHeight = 180
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
