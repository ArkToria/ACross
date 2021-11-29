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
            id: directRuleList
            Layout.fillWidth: true
            Layout.columnSpan: 4
            Layout.fillHeight: true
            clip: true

            model: ["baidu.com", "qq.com", "geosite:cn", "ext:customizedGeoSiteFile.dat:cn", "0.0.0.0/8", "10.0.0.0/8", "fc00::/7", "fe80::/10", "geoip:cn", "geoip:!cn", "ext:customizedGeoIPFile.dat:cn", "ext:customizedGeoIPFile.dat:!cn", "ext-ip:customizedGeoIPFile.dat:cn", "ext-ip:customizedGeoIPFile.dat:!cn"]
            delegate: RuleItemCard {
                state: control.isEdited ? "EditState" : "NormalState"
                width: directRuleList.width
            }
        }

        TextFieldBox {
            Layout.fillWidth: true
            Layout.columnSpan: 3
        }

        ButtonBox {
            text: qsTr("Add")
        }
    }
}
