import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

import "../components"

Popup {
    id: popWindow
    implicitWidth: 480
    implicitHeight: 360
    x: Math.round((mainWindow.width - width) / 2 - mainPanel.width)
    y: Math.round((mainWindow.height - height) / 2)

    property int textBoxWidth: 80
    property int spacingWidth: 16

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    onClosed: {
        mainComponent.state = "NormalState"
    }

    background: CardBox {}

    contentItem: ColumnLayout {
        spacing: acrossConfig.itemSpacing

        Label {
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter

            text: qsTr("Create a new group")
            font.pixelSize: 18
            color: acrossConfig.textColor
        }

        RowLayout {
            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Name")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                id: groupName
                Layout.fillWidth: true

                placeholderText: qsTr("Enter the unique group name")
            }
        }

        ColumnLayout {
            id: fromSubscriptionItem
            spacing: 16

            RowLayout {
                Label {
                    text: qsTr("Subscription")
                    color: acrossConfig.textColor
                    font.pixelSize: 18
                }

                Item {
                    Layout.fillWidth: true
                }

                SwitchBox {
                    id: fromSubscriptionSwitcher
                }
            }


            RowLayout {
                visible: fromSubscriptionSwitcher.checked

                Label {
                    Layout.preferredWidth: textBoxWidth

                    text: qsTr("Type")
                    color: acrossConfig.textColor
                }

                DropDownBox {
                    id: subscriptionType
                    model: ["SIP008", "Base64", "JSON"]
                }

                Item {
                    Layout.fillWidth: true
                }

                Label {
                    Layout.preferredWidth: textBoxWidth

                    text: qsTr("Cycle (days)")
                    color: acrossConfig.textColor
                }

                NumBox {
                    id: subscriptionCycleTime
                    value: 15
                    from: 1
                    to: 31
                }
            }


            RowLayout {
                visible: fromSubscriptionSwitcher.checked

                Label {
                    Layout.preferredWidth: textBoxWidth

                    text: qsTr("Address")
                    color: acrossConfig.textColor
                }

                TextFieldBox {
                    id: subscriptionUrl
                    Layout.fillWidth: true

                    placeholderText: qsTr("Enter the unique subscription url")
                }
            }
        }

        ColumnLayout {
            id: fromPasteItem
            spacing: 16
            visible: !fromSubscriptionSwitcher.checked

            RowLayout {
                Label {
                    text: qsTr("From Paste")
                    color: acrossConfig.textColor
                    font.pixelSize: 18
                }
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                TextArea {
                    implicitHeight: 72

                    placeholderText: "ss://\n" + "vmess://\n" + "v2ray json config"
                    color: acrossConfig.deepTextColor
                    wrapMode: Text.NoWrap

                    background: CardBox {
                        color: acrossConfig.deepColor
                        layer.enabled: false
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }

        RowLayout {
            spacing: acrossConfig.itemSpacing * 2

            Item {
                Layout.fillWidth: true
            }

            ButtonBox {
                text: qsTr("Accept")
                onClicked: {
                    if (fromSubscriptionSwitcher.checked) {
                        acrossGroups.appendItem(groupName.text,
                                                subscriptionUrl.text,
                                                subscriptionType.currentIndex,
                                                subscriptionCycleTime.value)
                    } else {
                        acrossGroups.appendItem(groupName.text)
                    }

                    popWindow.close()
                }
            }

            ButtonBox {
                text: qsTr("Cancel")

                onClicked: {
                    popWindow.close()
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.66;height:480;width:640}
}
##^##*/

