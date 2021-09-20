import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls

import ACross

import "../components/"

Window {
    id: nodeFormPopWindow
    width: 680
    height: 310
    minimumWidth: 680
    minimumHeight: 310
    title: qsTr("Create New Configuration")

    flags: Qt.WindowStaysOnTopHint
    modality: Qt.ApplicationModal

    onVisibilityChanged: {
        if (!visible) {
            nodeFormPopWindow.close()
            nodeFormPopWindow.destroy()
        }
    }

    ShadowsocksFormModel {
        id: shadowsocksFormModel
    }

    TrojanFormModel {
        id: trojanFormModel
    }

    VMessFormModel {
        id: vmessFormModel
    }

    RawOutboundFormModel {
        id: rawOutboundFormModel
    }

    URLSchemeFormModel {
        id: urlSchemeFormModel
    }

    NodeFormModel {
        id: nodeFormModel
        list: acrossNodes
        trojan: trojanFormModel
        shadowsocks: shadowsocksFormModel
        vmess: vmessFormModel
        raw: rawOutboundFormModel
        urlScheme: urlSchemeFormModel
    }

    Rectangle {
        id: background
        anchors.fill: parent

        color: acrossConfig.deepColor

        ScrollView {
            id: scrollView
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing
            contentHeight: column.height
            clip: true

            Column {
                id: column
                spacing: acrossConfig.itemSpacing

                CardBox {
                    id: outboundSetting
                    implicitWidth: scrollView.availableWidth
                    implicitHeight: 64

                    layer.enabled: false

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 0

                        RowLayout {
                            Layout.fillWidth: true
                            Layout.margins: acrossConfig.itemSpacing

                            spacing: acrossConfig.itemSpacing

                            Label {
                                Layout.margins: acrossConfig.itemSpacing

                                text: qsTr("Outbound")
                                font.pixelSize: 24
                                color: acrossConfig.textColor
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            DropDownBox {
                                model: ["url", "manual", "outbound"]

                                onEditTextChanged: {
                                    var height = 64
                                    manualSetting.visible = false

                                    switch (editText) {
                                    case "manual":
                                        nodeFormModel.protocol = "vmess"
                                        height += manualSetting.implicitHeight
                                        manualSetting.visible = true
                                        streamSettingLoader.source
                                                = "qrc:/src/views/home/VMESSSetting.qml"
                                        break
                                    case "outbound":
                                        nodeFormModel.protocol = "raw"
                                        streamSettingLoader.source
                                                = "qrc:/src/views/home/RawOutboundSetting.qml"
                                        break
                                    case "url":
                                        nodeFormModel.protocol = "scheme"
                                        streamSettingLoader.source
                                                = "qrc:/src/views/home/URLSchemeSetting.qml"
                                        break
                                    default:
                                        console.log("unknown setting")
                                    }

                                    outboundSetting.implicitHeight = height
                                }
                            }
                        }

                        Item {
                            Layout.fillHeight: true
                        }

                        ManualSetting {
                            id: manualSetting
                            Layout.fillWidth: true

                            visible: false
                        }
                    }
                }

                CardBox {
                    implicitWidth: scrollView.availableWidth
                    implicitHeight: streamSettingLoader.implicitHeight

                    layer.enabled: false

                    Loader {
                        id: streamSettingLoader
                        anchors.fill: parent

                        signal acceptAll
                    }
                }

                RowLayout {
                    spacing: acrossConfig.itemSpacing
                    implicitWidth: scrollView.availableWidth

                    Item {
                        Layout.fillWidth: true
                    }

                    ButtonBox {
                        id: acceptFormButton
                        text: qsTr("Accept")
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        onClicked: {
                            streamSettingLoader.acceptAll()
                            nodeFormModel.accept()
                            nodeFormPopWindow.close()
                        }
                    }

                    ButtonBox {
                        text: qsTr("Cancel")
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                        onClicked: {
                            nodeFormModel.cancel()
                            nodeFormPopWindow.close()
                        }
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/

