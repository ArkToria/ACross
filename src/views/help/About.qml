import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

CardBox {
    id: aboutItem
    implicitWidth: 680
    implicitHeight: 320

    property int fontSize: 14
    property bool isDev: false

    RowLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 4
        spacing: acrossConfig.itemSpacing * 2

        Item {
            property int logoBasicSize: 128

            Layout.preferredWidth: logoBasicSize * 2
            Layout.fillHeight: true

            SVGBox {
                id: logoImage
                anchors.centerIn: parent

                sourceWidth: parent.logoBasicSize
                sourceHeight: parent.logoBasicSize
                source: getLogo(acrossConfig.iconStyle)

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {
                        if (!isDev) {
                            logoImage.source = getLogo("dev")
                        } else {
                            logoImage.source = getLogo(acrossConfig.iconStyle)
                        }

                        aboutItem.isDev = !aboutItem.isDev
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            GridLayout {
                anchors.fill: parent

                columns: 2
                rowSpacing: acrossConfig.itemSpacing * 2
                columnSpacing: acrossConfig.itemSpacing * 2

                Label {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2

                    text: "ACross"
                    font.pointSize: Math.round(fontSize * 1.2)
                    color: acrossConfig.textColor
                }

                Label {
                    text: qsTr("Version")
                    color: acrossConfig.textColor
                }

                Label {
                    Layout.fillWidth: true

                    text: acrossConfig.guiVersion
                    color: acrossConfig.highlightColor
                }

                Label {
                    text: qsTr("Build Info")
                    color: acrossConfig.textColor
                }

                Label {
                    Layout.fillWidth: true

                    text: acrossConfig.buildInfo
                    color: acrossConfig.textColor
                }

                Label {
                    text: qsTr("Build Time")
                    color: acrossConfig.textColor
                }

                Label {
                    Layout.fillWidth: true

                    text: acrossConfig.buildTime
                    color: acrossConfig.textColor
                }

                Label {
                    text: qsTr("Source Code")
                    color: acrossConfig.textColor
                }

                URLBox {
                    Layout.fillWidth: true

                    urlText: acrossConfig.sourceCodeURL
                }

                Label {
                    text: qsTr("Licenses")
                    color: acrossConfig.textColor
                }

                URLBox {
                    Layout.fillWidth: true

                    urlText: acrossConfig.licenseURL
                }

                Label {
                    text: qsTr("Extra Info")
                    color: acrossConfig.textColor
                }

                TextAreaBox {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    text: acrossConfig.extraInfo
                    color: acrossConfig.textColor
                    wrapMode: Text.WrapAnywhere
                    readOnly: true
                    selectByMouse: true
                    selectedTextColor: acrossConfig.highlightTextColor
                    selectionColor: acrossConfig.highlightColor
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2

                    spacing: acrossConfig.itemSpacing * 2

                    Item {
                        Layout.fillWidth: true
                    }

                    URLBox {
                        id: newVersionText
                        visible: false

                        urlText: acrossConfig.releaseURL
                        color: acrossConfig.styleColor
                    }

                    ButtonBox {
                        text: qsTr("Report Bugs")
                        basicColor: acrossConfig.warnColor
                        basicState: "WarnState"
                        onClicked: Qt.openUrlExternally(acrossConfig.reportURL)
                    }

                    ButtonBox {
                        text: qsTr("Check Update")

                        onClicked: {
                            acrossConfig.checkUpdate()
                        }
                    }

                    Connections {
                        target: acrossConfig

                        function onUpdatedChanged(version) {
                            newVersionText.visible = true

                            if (version !== "") {
                                newVersionText.text = qsTr(
                                            "New Version: ") + version
                            } else {
                                newVersionText.text = qsTr(
                                            "Already the latest version")
                            }
                        }
                    }
                }
            }
        }
    }
}
