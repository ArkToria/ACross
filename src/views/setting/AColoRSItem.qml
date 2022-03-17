/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

import Arktoria.ACross
//import QtQuick.Dialogs
import Qt.labs.platform
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    property int fontSize: 14
    property int keyBoxWidth: 72

    property Component corePathWindowComponent: null

    FileDialog {
        id: acolorsFileDialog

        title: qsTr("Select AColoRS Core Path")
        nameFilters: ["Select AColoRS core (acolors*)", "All files (*)"]
        onAccepted: {
            acrossConfig.acolorsPath = currentFile;
        }
    }

    FileDialog {
        id: configFileDialog

        title: qsTr("Select Config Path")
        nameFilters: ["Select AColoRS config (*.json)", "All files (*)"]
        onAccepted: {
            acrossConfig.acolorsConfigPath = currentFile;
        }
    }

    FileDialog {
        id: databaseFileDialog

        title: qsTr("Select Database Path")
        nameFilters: ["Select AColoRS database (*.db)", "All files (*)"]
        onAccepted: {
            acrossConfig.acolorsDbPath = currentFile;
        }
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 4
        columns: 5
        rowSpacing: acrossConfig.itemSpacing * 2
        columnSpacing: acrossConfig.itemSpacing * 2

        Label {
            Layout.columnSpan: 5
            text: qsTr("AColoRS Configuration")
            font.pointSize: Math.round(fontSize * 1.2)
            color: acrossConfig.textColor
        }

        Label {
            Layout.preferredWidth: keyBoxWidth
            text: qsTr("Core path")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            Layout.fillWidth: true
            Layout.columnSpan: 3
            placeholderText: acrossConfig.acolorsPath === "" ? qsTr("Enter AColoRS Core Executable Path Here") : acrossConfig.acolorsPath
        }

        ButtonBox {
            Layout.alignment: Qt.AlignRight
            text: qsTr("Select")
            onClicked: {
                acolorsFileDialog.open();
            }
        }

        Label {
            Layout.preferredWidth: keyBoxWidth
            text: qsTr("Config path")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            Layout.fillWidth: true
            Layout.columnSpan: 3
            placeholderText: acrossConfig.acolorsConfigPath === "" ? qsTr("Enter Config Directory Here") : acrossConfig.acolorsConfigPath
        }

        ButtonBox {
            Layout.alignment: Qt.AlignRight
            text: qsTr("Select")
            onClicked: {
                configFileDialog.open();
            }
        }

        Label {
            Layout.preferredWidth: keyBoxWidth
            text: qsTr("Database path")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            Layout.fillWidth: true
            Layout.columnSpan: 3
            placeholderText: acrossConfig.acolorsDbPath === "" ? qsTr("Enter Database Directory Here") : acrossConfig.acolorsDbPath
        }

        ButtonBox {
            Layout.alignment: Qt.AlignRight
            text: qsTr("Select")
            onClicked: {
                databaseFileDialog.open();
            }
        }


        Label {
            Layout.preferredWidth: keyBoxWidth
            text: qsTr("API Port")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: apiPortText

            Layout.preferredWidth: keyBoxWidth
            //Layout.preferredWidth: 72
            placeholderText: acrossConfig.acolorsAPIPort
            inputMethodHints: Qt.ImhDigitsOnly
            onTextEdited: {
                acrossConfig.acolorsAPIPort = text;
            }
        }
        
        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 1
        }

        Label {
            Layout.preferredWidth: keyBoxWidth
            text: qsTr("Core Path")
            color: acrossConfig.textColor
        }

        ButtonBox {
            id: corePathButton
            Layout.alignment: Qt.AlignRight
            text: qsTr("Select")
            onClicked: {
                if (corePathWindowComponent == null)
                        corePathWindowComponent = Qt.createComponent("qrc:/Arktoria/ACross/src/views/setting/CorePathForm.qml");

                    if (corePathWindowComponent.status === Component.Ready)
                        corePathWindowComponent.createObject(corePathButton).show();
            }
        }

    }

}