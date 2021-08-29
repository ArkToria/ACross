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
        id: coreFileDialog
        title: qsTr("Select V2ray Core Path")
        selectFolder: false
        selectExisting: true
        selectMultiple: false
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

    ColumnLayout {
        anchors.fill: parent
        spacing: acrossConfig.itemSpacing

        Label {
            text: qsTr("Core Configuration")
            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        RowLayout {
            spacing: spacingWidth

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Core path")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                placeholderText: acrossConfig.corePath === "" ? qsTr("Enter V2ray Core Executable Path Here") : acrossConfig.corePath
                Layout.fillWidth: true
            }

            ButtonBox {
                text: qsTr("Select")
                onClicked: {
                    coreFileDialog.open()
                }
            }
        }

        RowLayout {
            spacing: spacingWidth

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Assets path")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                placeholderText: acrossConfig.assetsPath === "" ? qsTr("Enter GeoIP and GeoSite Directory Here") : acrossConfig.assetsPath
                Layout.fillWidth: true
            }

            ButtonBox {
                text: qsTr("Select")
                onClicked: {
                    assetsFileDialog.open()
                }
            }
        }

        RowLayout {
            spacing: spacingWidth
            Layout.fillWidth: true

            Label {
                Layout.preferredWidth: textBoxWidth

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
                placeholderText: acrossConfig.apiPort
                Layout.fillWidth: true
                readOnly: apiSwitch.checked ? false : true
                onFocusChanged: {
                    acrossConfig.apiPort = text
                }
                onAccepted: {
                    acrossConfig.apiPort = text
                }
            }

            Label {
                id: testResult
                visible: false
            }

            ButtonBox {
                text: qsTr("Test")
                enabled: apiSwitch.checked
                onClicked: {
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

        RowLayout {
            spacing: spacingWidth
            Layout.fillWidth: true
            Layout.fillHeight: true

            property int buttonWidth: 96

            TextFieldBox {
                id: core_info
                Layout.fillWidth: true

                text: acrossConfig.coreInfo
                readOnly: true
                color: acrossConfig.textColor
            }

            ButtonBox {
                text: qsTr("Check Core")
                implicitWidth: parent.buttonWidth

                onClicked: {
                    core_info.text = acrossConfig.coreInfo
                }
            }

            ButtonBox {
                text: qsTr("Save Config")
                implicitWidth: parent.buttonWidth

                onClicked: {
                    acrossConfig.saveConfig()
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

