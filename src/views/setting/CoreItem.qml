import QtQuick
//import QtQuick.Dialogs
import Qt.labs.platform
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    anchors.margins: acrossConfig.itemSpacing * 2

    property int fontSize: 14

    FileDialog {
        id: coreFileDialog
        title: qsTr("Select V2Ray Core Path")
        nameFilters: ["Select V2Ray core (v2ray*)", "All files (*)"]

        onAccepted: {
            acrossConfig.corePath = currentFile
        }
    }

    FolderDialog {
        id: assetsFileDialog
        title: qsTr("Select Assets Directory")

        onAccepted: {
            acrossConfig.assetsPath = currentFolder
        }
    }

    GridLayout {
        anchors.fill: parent
        columns: 5
        rowSpacing: acrossConfig.itemSpacing
        columnSpacing: acrossConfig.itemSpacing

        Label {
            Layout.columnSpan: 5
            text: qsTr("Core Configuration")
            font.pointSize: fontSize
            color: acrossConfig.textColor
        }

        Label {
            text: qsTr("Log Level")
        }

        DropDownBox {
            Layout.fillWidth: true
            Layout.columnSpan: 4

            model: ["debug", "info", "warning", "error", "none"]
        }

        Label {
            text: qsTr("Core path")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            Layout.fillWidth: true
            Layout.columnSpan: 3

            placeholderText: acrossConfig.corePath === "" ? qsTr("Enter V2Ray Core Executable Path Here") : acrossConfig.corePath
        }

        ButtonBox {
            Layout.alignment: Qt.AlignRight

            text: qsTr("Select")
            onClicked: {
                coreFileDialog.open()
            }
        }

        Label {
            text: qsTr("Assets path")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            Layout.fillWidth: true
            Layout.columnSpan: 3

            placeholderText: acrossConfig.assetsPath === "" ? qsTr("Enter GeoIP and GeoSite Directory Here") : acrossConfig.assetsPath
        }

        ButtonBox {
            Layout.alignment: Qt.AlignRight

            text: qsTr("Select")
            onClicked: {
                assetsFileDialog.open()
            }
        }

        Label {
            text: qsTr("Core Info")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: core_info
            Layout.fillWidth: true
            Layout.columnSpan: 3

            text: acrossConfig.coreInfo
            readOnly: true
            color: acrossConfig.textColor
        }

        ButtonBox {
            Layout.alignment: Qt.AlignRight
            text: qsTr("Check")

            onClicked: {
                core_info.text = acrossConfig.coreInfo
            }
        }

        Label {
            text: qsTr("Enable API")
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 3
        }

        SwitchBox {
            id: apiSwitch
            Layout.alignment: Qt.AlignRight

            checked: acrossConfig.apiEnable
            onCheckedChanged: {
                acrossConfig.apiEnable = checked
            }
        }

        Label {
            visible: apiSwitch.checked

            text: qsTr("API Port")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: apiPortText
            visible: apiSwitch.checked
            Layout.preferredWidth: 72

            placeholderText: acrossConfig.apiPort
            readOnly: apiSwitch.checked ? false : true
            inputMethodHints: Qt.ImhDigitsOnly

            onTextEdited: {
                acrossConfig.apiPort = text
            }
        }

        Label {
            visible: apiSwitch.checked

            text: qsTr("Test Result")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: testResult
            Layout.fillWidth: true
            visible: apiSwitch.checked
        }

        ButtonBox {
            Layout.alignment: Qt.AlignRight

            visible: apiSwitch.checked
            text: qsTr("Test")
            enabled: apiSwitch.checked

            onClicked: {
                // sync input information
                acrossConfig.apiPort = apiPortText.text

                var stats = acrossConfig.testAPI()

                if (stats && acrossConfig.apiResultText === "") {
                    testResult.text = qsTr("Success")
                    testResult.color = acrossConfig.styleColor
                } else {
                    testResult.text = acrossConfig.apiResultText
                    testResult.color = acrossConfig.warnColor
                }

                testResult.visible = true
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

