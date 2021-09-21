import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

import ACross

Popup {
    id: popWindow
    implicitWidth: 480
    implicitHeight: 300
    contentHeight: implicitHeight
    contentWidth: implicitWidth
    x: Math.round((mainWindow.width - width) / 2 - mainPanel.width)
    y: Math.round((mainWindow.height - height) / 2)

    property int spacingWidth: 16

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    onClosed: {
        darkBackground.close()
    }

    background: CardBox {}

    contentItem: GridLayout {
        rowSpacing: acrossConfig.itemSpacing
        columnSpacing: acrossConfig.itemSpacing
        columns: 4

        Label {
            Layout.fillWidth: true
            Layout.columnSpan: 4

            text: qsTr("Create a new group")
            font.pixelSize: 18
            color: acrossConfig.textColor
        }

        Label {
            text: qsTr("Name")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: groupName
            Layout.columnSpan: 3
            Layout.fillWidth: true

            placeholderText: qsTr("Enter the unique group name")
        }

        Label {
            text: qsTr("Subscription")
            color: acrossConfig.textColor
            font.pixelSize: 18
        }

        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }

        SwitchBox {
            id: fromSubscriptionSwitcher
            Layout.alignment: Qt.AlignRight
        }

        Label {
            Layout.fillWidth: true
            Layout.columnSpan: 4
            visible: !fromSubscriptionSwitcher.checked

            text: qsTr("From Paste")
            color: acrossConfig.textColor
            font.pixelSize: 18
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 4

            visible: !fromSubscriptionSwitcher.checked

            TextAreaBox {
                id: pasteItemText
                implicitHeight: 72

                placeholderText: "ss://\n" + "vmess://\n" + "v2ray json config"
                wrapMode: Text.NoWrap
            }
        }

        Label {
            visible: fromSubscriptionSwitcher.checked
            text: qsTr("Type")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: subscriptionType
            Layout.fillWidth: true
            visible: fromSubscriptionSwitcher.checked

            // defined at dbtools.h SubscriptionType enum class
            model: ["SIP008", "Base64", "JSON"]
        }

        Label {
            visible: fromSubscriptionSwitcher.checked

            text: qsTr("Cycle (days)")
            color: acrossConfig.textColor
        }

        NumBox {
            id: subscriptionCycleTime
            Layout.fillWidth: true
            visible: fromSubscriptionSwitcher.checked

            value: 15
            from: 1
            to: 31
        }

        Label {
            visible: fromSubscriptionSwitcher.checked

            text: qsTr("Address")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: subscriptionUrl
            Layout.fillWidth: true
            Layout.columnSpan: 3
            visible: fromSubscriptionSwitcher.checked

            placeholderText: qsTr("Enter the unique subscription url")
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 4
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.columnSpan: 4

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
                        acrossGroups.appendItem(groupName.text,
                                                pasteItemText.text)
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
