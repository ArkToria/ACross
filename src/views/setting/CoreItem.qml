import QtQuick 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

Item {
    anchors.margins: acrossConfig.itemSpacing * 2

    FileDialog {
        id: coreFileDialog
        title: qsTr("Select V2ray Core Path")
        selectFolder: false
        selectExisting: true
        selectMultiple: false
        nameFilters: ["Select v2ray core (v2ray*)", "All files (*)"]

        onAccepted: {
            acrossConfig.corePath = fileUrl
        }
    }

    FileDialog {
        id: assetsFileDialog
        title: qsTr("Select Assets Directory")
        selectFolder: true
        selectExisting: true
        selectMultiple: false

        onAccepted: {
            acrossConfig.assetsPath = fileUrl
        }
    }

    GridLayout {
        anchors.fill: parent
        columns: 9
        rowSpacing: acrossConfig.itemSpacing
        columnSpacing: acrossConfig.itemSpacing

        Label {
            Layout.columnSpan: 9
            text: qsTr("Core Configuration")
            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        Label {
            text: qsTr("Core path")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            Layout.fillWidth: true
            Layout.columnSpan: 7

            placeholderText: acrossConfig.corePath === "" ? qsTr("Enter V2ray Core Executable Path Here") : acrossConfig.corePath
        }

        ButtonBox {
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
            Layout.columnSpan: 7

            placeholderText: acrossConfig.assetsPath === "" ? qsTr("Enter GeoIP and GeoSite Directory Here") : acrossConfig.assetsPath
        }

        ButtonBox {
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
            Layout.columnSpan: 7

            text: acrossConfig.coreInfo
            readOnly: true
            color: acrossConfig.textColor
        }

        ButtonBox {
            text: qsTr("Check")

            onClicked: {
                core_info.text = acrossConfig.coreInfo
            }
        }

        Label {
            text: qsTr("Enable API")
            color: acrossConfig.textColor
        }

        SwitchBox {
            id: apiSwitch
            checked: acrossConfig.apiEnable
            onCheckedChanged: {
                acrossConfig.apiEnable = checked
            }
        }

        Label {
            text: qsTr("API Port")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: apiPortText

            Layout.fillWidth: true
            Layout.columnSpan: 2

            placeholderText: acrossConfig.apiPort
            readOnly: apiSwitch.checked ? false : true
            inputMethodHints: Qt.ImhDigitsOnly

            onTextEdited: {
                acrossConfig.apiPort = text
            }
        }

        Label {
            text: qsTr("Test Result")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: testResult

            Layout.fillWidth: true
            Layout.columnSpan: 2
        }

        ButtonBox {
            text: qsTr("Test")

            enabled: apiSwitch.checked

            onClicked: {
                // sync input information
                acrossConfig.apiPort = apiPortText.text

                var stats = acrossConfig.testApi()

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

