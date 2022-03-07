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

    FileDialog {
        id: acolorsFileDialog

        title: qsTr("Select AColoRS Core Path")
        nameFilters: ["Select AColoRS core (acolors*)", "All files (*)"]
        onAccepted: {
            acrossConfig.acolorsPath = currentFile;
        }
    }

    FolderDialog {
        id: configFileDialog

        title: qsTr("Select Config Directory")
        onAccepted: {
            acrossConfig.acolorsConfigPath = currentFolder;
        }
    }

    FolderDialog {
        id: databaseFileDialog

        title: qsTr("Select Database Directory")
        onAccepted: {
            acrossConfig.acolorsDbPath = currentFolder;
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

        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 3
        }

        TextFieldBox {
            id: apiPortText

            Layout.preferredWidth: keyBoxWidth
            Layout.columnSpan: 1
            //Layout.preferredWidth: 72
            placeholderText: acrossConfig.acolorsAPIPort
            inputMethodHints: Qt.ImhDigitsOnly
            onTextEdited: {
                acrossConfig.acolorsAPIPort = text;
            }
        }

    }

}