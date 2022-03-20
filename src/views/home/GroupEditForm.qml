import Arktoria.ACross
import Qt5Compat.GraphicalEffects
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic as ControlsBasic
import QtQuick.Layouts

Popup {
    id: popWindow

    property string title: qsTr("Edit Group")
    property int fontSize: 14
    property var groupInfo
    property int index
    property bool isSubscriotion: false

    implicitWidth: 480
    implicitHeight: 500
    contentHeight: implicitHeight
    contentWidth: implicitWidth
    x: Math.round((mainWindow.width - width) / 2 - mainPanel.width)
    y: Math.round((mainWindow.height - height) / 2)
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    onOpened: {
        isSubscriotion = groupInfo["isSubscription"];
        groupName.text = groupInfo["Name"];
        pasteItemText.text = groupInfo["nodesURL"];
        if (isSubscriotion) {
            pasteItemText.readOnly = true;
            subscriptionUrl.text = groupInfo["url"];
            subscriptionType.currentIndex = groupInfo["type"];
            subscriptionCycleTime.value = groupInfo["cycleTime"];
        } else {
            pasteItemText.readOnly = false;
            subscriptionUrl.text = "";
            subscriptionType.currentIndex = 0;
            subscriptionCycleTime.value = 1;
        }
    }
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
                Layout.fillWidth: true
                Layout.columnSpan: 3
                text: qsTr("Nodes URL")
                color: acrossConfig.textColor
                font.pointSize: fontSize
            }

            CardBox {
                Layout.alignment: Qt.AlignRight
                Layout.columnSpan: 1
                Layout.preferredWidth: 30
                Layout.preferredHeight: 30
                layer.enabled: false
                color: "transparent"

                RowLayout {
                    id: groupEditFormURLCopy

                    anchors.centerIn: parent

                    SVGBox {
                        source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/copy.svg"
                        sourceWidth: 22
                        sourceHeight: 22
                    }

                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton
                    onEntered: {
                        parent.color = acrossConfig.deepColor;
                    }
                    onExited: {
                        parent.color = "transparent";
                    }
                    onClicked: {
                        acrossGroups.copyNodesToClipboard(index);
                    }
                }

            }

            ControlsBasic.ScrollView {
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
                color: acrossConfig.textColor
                font.pointSize: fontSize
            }

            SwitchBox {
                id: fromSubscriptionSwitcher

                Layout.alignment: Qt.AlignRight
                checked: isSubscriotion
                onCheckedChanged: {
                    if (checked) {
                        pasteItemText.readOnly = true;
                        subscriptionUrl.text = groupInfo["url"];
                        subscriptionType.currentIndex = groupInfo["type"];
                        subscriptionCycleTime.value = groupInfo["cycleTime"];
                    }
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

            RowLayout {
                Layout.fillWidth: true
                Layout.columnSpan: 3

                TextFieldBox {
                    id: subscriptionUrl

                    Layout.fillWidth: true
                    visible: fromSubscriptionSwitcher.checked
                    placeholderText: qsTr("Enter the unique subscription url")
                    onTextChanged: {
                        if (text !== "")
                            placeholderTextColor:
                            acrossConfig.deepTextColor;

                    }
                }

                CardBox {
                    Layout.alignment: Qt.AlignRight
                    Layout.preferredWidth: 30
                    Layout.preferredHeight: 30
                    visible: fromSubscriptionSwitcher.checked
                    layer.enabled: false
                    color: "transparent"

                    RowLayout {
                        id: groupEditFormAddressCopy

                        anchors.centerIn: parent

                        SVGBox {
                            source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/copy.svg"
                            sourceWidth: 22
                            sourceHeight: 22
                        }

                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        acceptedButtons: Qt.LeftButton
                        onEntered: {
                            parent.color = acrossConfig.deepColor;
                        }
                        onExited: {
                            parent.color = "transparent";
                        }
                        onClicked: {
                            acrossGroups.copyUrlToClipboard(index);
                        }
                    }

                }

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
                        if (fromSubscriptionSwitcher.checked && subscriptionUrl.text === "") {
                            subscriptionUrl.placeholderText = qsTr("URL cannot be empty!");
                            subscriptionUrl.placeholderTextColor = acrossConfig.warnColor;
                            return ;
                        }
                        acrossGroups.editItem(index, groupName.text, fromSubscriptionSwitcher.checked, subscriptionUrl.text, subscriptionType.currentIndex, subscriptionCycleTime.value, pasteItemText.text);
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
