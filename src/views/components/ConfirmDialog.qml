import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Dialog {
    id: rootDialog
    modal: true
    implicitWidth: 320
    implicitHeight: 160
    contentHeight:implicitHeight
    contentWidth:implicitWidth
    
    x: Math.round((mainWindow.width - width) / 2 - mainPanel.width)
    y: Math.round((mainWindow.height - height) / 2)

    property int index
    property string headerText
    property string contentText

    background: CardBox {}

    onClosed: {
        mainComponent.state = "NormalState"
    }

    onAccepted: {
        mainComponent.state = "NormalState"

        acrossGroups.removeItem(index)
    }

    onFocusChanged: {
        mainComponent.state = "NormalState"
        rootDialog.reject()
    }

    header: RowLayout {
        Label {
            text: headerText
            Layout.margins: 16
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            color: acrossConfig.textColor
            font.pixelSize: 18
        }
    }

    contentItem: Label {
        text: contentText
        color: acrossConfig.textColor
    }

    footer: RowLayout {
        spacing: 16

        Item {
            Layout.fillWidth: true
        }

        ButtonBox {
            text: qsTr("Accept")

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
