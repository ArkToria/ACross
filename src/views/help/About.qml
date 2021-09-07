import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

CardBox {
    implicitWidth: scrollView.availableWidth - acrossConfig.itemSpacing
    implicitHeight: aboutContent.height + acrossConfig.itemSpacing * 8

    RowLayout {
        id:aboutContent
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: acrossConfig.itemSpacing * 4
        spacing: acrossConfig.itemSpacing * 4

        SVGBox {
            sourceWidth: 128
            sourceHeight: 128
            source: getLogo(acrossConfig.iconStyle)
        }

        ColumnLayout {
            id: infoList
            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: acrossConfig.itemSpacing

            property int textPointSize: 16

            Label {
                text: "ACross"

                font.pixelSize: 24
                color: acrossConfig.textColor
            }

            GridLayout {
                columns: 2
                rowSpacing: acrossConfig.itemSpacing
                Layout.fillWidth: true

                Label {
                    text: qsTr("Version")
                    color: acrossConfig.textColor
                }

                Label {
                    text: acrossConfig.guiVersion
                    color: acrossConfig.highlightColor
                }

                Label {
                    text: qsTr("Build Info")
                    color: acrossConfig.textColor
                }

                Label {
                    text: acrossConfig.buildInfo
                    color: acrossConfig.textColor
                }

                Label {
                    text: qsTr("Build Time")
                    color: acrossConfig.textColor
                }

                Label {
                    text: acrossConfig.buildTime
                    color: acrossConfig.textColor
                }

                Label {
                    text: qsTr("Source Code")
                    color: acrossConfig.textColor
                }

                Rectangle {
                    id: sourceCodeField

                    color: "transparent"
                    width: urlLabel.contentWidth
                    height: urlLabel.contentHeight

                    property string urlText: acrossConfig.sourceCodeURL

                    Label {
                        id: urlLabel
                        text: parent.urlText
                        color: acrossConfig.highlightColor
                    }

                    MouseArea {
                        anchors.fill: sourceCodeField
                        hoverEnabled: true

                        onEntered: {
                            sourceCodeField.color = acrossConfig.deepColor
                            urlLabel.color = acrossConfig.deepTextColor
                        }

                        onExited: {
                            sourceCodeField.color = "transparent"
                            urlLabel.color = acrossConfig.highlightColor
                        }

                        onClicked: Qt.openUrlExternally(sourceCodeField.urlText)
                    }
                }

                Label {
                    text: qsTr("Licenses")
                    color: acrossConfig.textColor
                }

                Rectangle {
                    id: licenseField

                    color: "transparent"
                    width: licenseLabel.contentWidth
                    height: licenseLabel.contentHeight

                    property string urlText: "https://www.gnu.org/licenses/gpl-3.0.html"

                    Label {
                        id: licenseLabel
                        text: "GPLv3"
                        color: acrossConfig.highlightColor
                    }

                    MouseArea {
                        anchors.fill: licenseField
                        hoverEnabled: true

                        onEntered: {
                            licenseField.color = acrossConfig.deepColor
                            licenseLabel.color = acrossConfig.deepTextColor
                        }

                        onExited: {
                            licenseField.color = "transparent"
                            licenseLabel.color = acrossConfig.highlightColor
                        }

                        onClicked: Qt.openUrlExternally(licenseField.urlText)
                    }
                }

                Label {
                    id: extraInfoText
                    text: qsTr("Extra Info")
                    color: acrossConfig.textColor
                }

                TextAreaBox {
                    Layout.fillWidth: true
                    text: acrossConfig.extraInfo
                    color: acrossConfig.textColor
                    readOnly: true
                }
            }


            Item {
                Layout.fillHeight: true
            }

            
            Row {
                layoutDirection: Qt.RightToLeft
                Layout.fillWidth: true

                spacing: acrossConfig.itemSpacing

                ButtonBox {
                    text: qsTr("Report Bugs")
                    basicColor: acrossConfig.warnColor
                    basicState: "warnState"
                }

                ButtonBox {
                    text: qsTr("Check Update")
                }
            }
        }
    }
}
