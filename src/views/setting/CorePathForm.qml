//import "../typescripts/home.js" as HomeJS
//
import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic as ControlsBasic
import QtQuick.Layouts
import Qt.labs.qmlmodels 1.0

Window {
    id: corePathFormWindow
    property int fontSize: 14

    width: 480
    height: 640
    minimumWidth: 400
    minimumHeight: 600
    title: qsTr("Edit Core Path")

    flags: Qt.platform.os === "windows" ? Qt.Window : Qt.WindowStaysOnTopHint
    modality: Qt.platform.os === "osx" ? Qt.NonModal : Qt.ApplicationModal

    Rectangle {
        anchors.fill: parent
        color: acrossConfig.deepColor

        CardBox {
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing

            GridLayout {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing * 4
                columns: 6
                rows: 3
                columnSpacing: acrossConfig.itemSpacing * 2
                rowSpacing: acrossConfig.itemSpacing * 2

                Label {
                    Layout.fillWidth: true
                    Layout.columnSpan: 6
                    text: qsTr("Core List")
                    font.pointSize: fontSize
                    color: acrossConfig.textColor
                }

                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.columnSpan: 6
                    spacing: 0
                    HorizontalHeaderView {
                        Layout.fillWidth: true
                        id: coreTable
                        syncView: corePathTable
                    }
                    TableView {
                        id:corePathTable
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        ScrollBar.vertical: ControlsBasic.ScrollBar {
                            policy: ScrollBar.AsNeeded
                            smooth: true
                        }
                        ScrollBar.horizontal: ControlsBasic.ScrollBar {
                            policy: ScrollBar.AsNeeded
                            smooth: true
                        }
                        model: TableModel {
                            TableModelColumn { display: "name" }
                            TableModelColumn { display: "color" }

                            rows: [
                                {
                                    "name": "cat",
                                    "color": "black"
                                },
                                {
                                    "name": "dog",
                                    "color": "brown"
                                },
                                {
                                    "name": "bird",
                                    "color": "white"
                                }
                            ]
                        }

                        delegate: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 50
                            border.width: 1

                            Text {
                                text: display
                                anchors.centerIn: parent
                            }
                        }
                    }
                }
                

                Item {
                    Layout.fillWidth: true
                    Layout.columnSpan: 4
                }

                ButtonBox {
                    id: acceptFormButton

                    text: qsTr("Accept")
                    onClicked: {
                    }
                }

                ButtonBox {
                    text: qsTr("Cancel")
                    basicColor: acrossConfig.warnColor
                    basicState: "WarnState"
                    onClicked: {
                        corePathFormWindow.close();
                    }
                }



            }
        }
    }

}
