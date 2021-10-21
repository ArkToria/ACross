import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

import ACross

Popup {
    id: popWindow
    implicitWidth: 480
    implicitHeight: 500
    contentHeight: implicitHeight
    contentWidth: implicitWidth
    x: Math.round((mainWindow.width - width) / 2 - mainPanel.width)
    y: Math.round((mainWindow.height - height) / 2)

    property string title: qsTr("Edit group")
    property int fontSize: 14
    property var groupInfo
    property int index
    property bool isSubscriotion: false

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    onOpened: {
        isSubscriotion = groupInfo["isSubscription"]
        groupName.text = groupInfo["Name"]
        pasteItemText.text = groupInfo["nodesURL"]

        if (isSubscriotion) {
            pasteItemText.readOnly = true
            subscriptionUrl.text = groupInfo["url"]
            subscriptionType.currentIndex = groupInfo["type"]
            subscriptionCycleTime.value = groupInfo["cycleTime"]
        } else {
            pasteItemText.readOnly = false
            subscriptionUrl.text = ""
            subscriptionType.currentIndex = 0
            subscriptionCycleTime.value = 1
        }
    }

    onClosed: {
        darkBackground.close()
    }

    background: CardBox {}

    contentItem: Item {
        anchors.fill: parent
        GridLayout {
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing * 2
            rowSpacing: acrossConfig.itemSpacing
            columnSpacing: acrossConfig.itemSpacing
            columns: 4

            Label {
                Layout.fillWidth: true
                Layout.columnSpan: 4

                text: title
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: Math.round(fontSize * 1.1)
                color: acrossConfig.textColor
            }

            Label {
                text: qsTr("Name")
                color: acrossConfig.textColor
                font.pointSize: fontSize
            }

            TextFieldBox {
                id: groupName
                Layout.columnSpan: 3
                Layout.fillWidth: true

                placeholderText: qsTr("Enter the unique group name")
            }
            Label {
                Layout.fillWidth: true
                Layout.columnSpan: 4

                text: qsTr("Nodes URL")
                color: acrossConfig.textColor
                font.pointSize: fontSize
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.columnSpan: 4

                TextAreaBox {
                    id: pasteItemText
                    implicitHeight: 72

                    placeholderText: "ss://\n" + "vmess://\n" + "v2ray json config"
                    wrapMode: Text.NoWrap
                }
            }

            Label {
                Layout.fillWidth: true
                Layout.columnSpan: 3

                text: qsTr("Subscription")
                font.pointSize: fontSize
            }

            SwitchBox {
                id: fromSubscriptionSwitcher
                Layout.alignment: Qt.AlignRight
                checked: isSubscriotion
                checkable: index !== 0
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
                        acrossGroups.editItem(index, groupName.text,
                                              subscriptionUrl.text,
                                              subscriptionType.currentIndex,
                                              subscriptionCycleTime.value,
                                              pasteItemText.text)
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
}
