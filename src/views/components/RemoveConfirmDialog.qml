import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: rootDialog
    modal: true
    implicitWidth: 320
    implicitHeight: 200
    contentWidth: implicitWidth
    contentHeight: implicitHeight
    x: Math.round((mainWindow.width - width) / 2 - mainPanel.width)
    y: Math.round((mainWindow.height - height) / 2)

    property int index
    property string headerText
    property string contentText
    property int fontSize: 14

    background: CardBox {}

    onClosed: {
        darkBackground.close()
    }

    onAccepted: {
        darkBackground.close()
        acrossGroups.removeItem(index)
    }

    onFocusChanged: {
        darkBackground.close()
        rootDialog.reject()
    }

    header: Item {
        implicitHeight: title.height + acrossConfig.itemSpacing * 2
        anchors.margins: acrossConfig.itemSpacing * 2

        Label {
            id: title
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter

            text: headerText
            color: acrossConfig.textColor
            font.pointSize: Math.round(fontSize * 1.1)
        }
    }
    contentItem: Item {
        anchors.margins: acrossConfig.itemSpacing * 2

        Label {
            anchors.centerIn: parent

            text: contentText
            horizontalAlignment: Text.AlignHCenter
        }
    }

    footer: RowLayout {
        spacing: acrossConfig.itemSpacing * 2

        Item {
            Layout.fillWidth: true
        }

        ButtonBox {
            text: qsTr("Remove")

            basicState: "WarnState"
            basicColor: acrossConfig.warnColor

            onClicked: {
                rootDialog.accept()
            }
        }

        ButtonBox {
            text: qsTr("Cancel")

            onClicked: {
                rootDialog.reject()
            }
        }

        Item {
            implicitHeight: 32
            Layout.bottomMargin: 32
        }
    }
}
