import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Arktoria.ACross

CardBox {
    id: control
    implicitWidth: 240
    implicitHeight: 720

    property string title: "Direct"
    property bool isEdited: false
    property string raw: ""

    onRawChanged: {
        console.log(raw)
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 4
        columns: 4

        Label {
            text: control.title
            font.pointSize: fontSize
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 22
            Layout.columnSpan: 3

            RowLayout {
                anchors.fill: parent

                Item {
                    Layout.fillWidth: true
                }

                SVGBox {
                    source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/trash.svg"
                    sourceHeight: 24
                    sourceWidth: 24
                }

                SVGBox {
                    source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/edit.svg"
                    sourceHeight: 24
                    sourceWidth: 24

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            control.isEdited = !control.isEdited
                        }
                    }
                }
            }
        }

        ListView {
            id: ruleListView
            Layout.fillWidth: true
            Layout.columnSpan: 4
            Layout.fillHeight: true
            clip: true

            //            model:
            delegate: RuleItemCard {
                state: control.isEdited ? "EditState" : "NormalState"
                width: ruleListView.width
            }
        }

        TextFieldBox {
            Layout.fillWidth: true
            Layout.columnSpan: 3
        }

        ButtonBox {
            text: qsTr("Add")

            onClicked: {

            }
        }
    }
}
