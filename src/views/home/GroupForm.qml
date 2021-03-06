import Arktoria.ACross
import Qt5Compat.GraphicalEffects
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic as ControlsBasic
import QtQuick.Layouts

Popup {
    id: popWindow

    property string title: qsTr("Create a new group")
    property int fontSize: 14

    implicitWidth: 480
    implicitHeight: 300
    contentHeight: implicitHeight
    contentWidth: implicitWidth
    x: Math.round((mainWindow.width - width) / 2 - mainPanel.width)
    y: Math.round((mainWindow.height - height) / 2)
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    onClosed: {
        darkBackground.close();
    }

    background: CardBox {
    }

    contentItem: Item {
        anchors.fill: parent

        GridLayout {
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing * 4
            rowSpacing: acrossConfig.itemSpacing * 2
            columnSpacing: acrossConfig.itemSpacing * 2
            columns: 4

            Label {
                Layout.fillWidth: true
                Layout.columnSpan: 4
                text: title
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: Math.round(fontSize * 1.2)
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
                text: qsTr("Subscription")
                color: acrossConfig.textColor
                font.pointSize: fontSize
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
                font.pointSize: fontSize
            }

            ControlsBasic.ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.columnSpan: 4
                visible: !fromSubscriptionSwitcher.checked

                TextAreaBox {
                    id: pasteItemText

                    implicitHeight: 80
                    placeholderText: "ss://\n" + "vmess://\n" + "trojan://"
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
                model: ["Base64", "SIP008", "JSON"]
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
                        if (fromSubscriptionSwitcher.checked)
                            acrossGroups.appendItem(groupName.text, subscriptionUrl.text, subscriptionType.currentIndex, subscriptionCycleTime.value);
                        else
                            acrossGroups.appendItem(groupName.text, pasteItemText.text);
                        popWindow.close();
                    }
                }

                ButtonBox {
                    text: qsTr("Cancel")
                    onClicked: {
                        popWindow.close();
                    }
                }

            }

        }

    }

}
