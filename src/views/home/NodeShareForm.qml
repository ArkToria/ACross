import QtQuick 2.12
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import ACross 1.0

import "../components/"

Window {
    id: nodeShareFormWindow
    width: 680
    height: 360
    minimumWidth: 680
    minimumHeight: 360
    title: qsTr("Share Configuration")

    flags: Qt.WindowStaysOnTopHint
    modality: Qt.ApplicationModal

    Rectangle {
        id: background
        anchors.fill: parent

        color: acrossConfig.deepColor

        CardBox {
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing

            Row {
                anchors.fill: parent
                anchors.margins: acrossConfig.itemSpacing

                Rectangle {
                    implicitWidth: parent.width / 2
                    implicitHeight: parent.height

                    color: "transparent"

                    SVGBox {
                        anchors.centerIn: parent

                        source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/qrcode.svg"
                        sourceWidth: 256
                        sourceHeight: 256
                    }
                }

                ColumnLayout {
                    implicitWidth: parent.width / 2
                    implicitHeight: parent.height

                    spacing: acrossConfig.itemSpacing

                    Label {
                        Layout.margins: acrossConfig.itemSpacing

                        text: qsTr("Share Format")
                        color: acrossConfig.textColor
                        font.pixelSize: 18
                    }

                    DropDownBox {
                        Layout.fillWidth: true

                        model: ["sip008", "base64", "OOCv2"]
                    }

                    Label {
                        Layout.margins: acrossConfig.itemSpacing

                        text: qsTr("Copy URL")
                        color: acrossConfig.textColor
                        font.pixelSize: 18
                    }

                    TextAreaBox {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }
        }
    }
}
