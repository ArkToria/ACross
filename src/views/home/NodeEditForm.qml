import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Window {
    id: nodeEditFormPopWindow
    width: 960
    height: 720
    minimumWidth: 680
    minimumHeight: 420
    title: qsTr("Edit Configuration")

    flags: Qt.WindowStaysOnTopHint
    modality: Qt.ApplicationModal

    property var nodeModel: null

    onVisibilityChanged: {
        if (!visible) {
            nodeEditFormPopWindow.close()
            nodeEditFormPopWindow.destroy()
        }
    }

    Rectangle {
        anchors.fill: parent
        color: acrossConfig.deepColor

        CardBox {
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing

            RowLayout {
                anchors.fill: parent

                TextFieldBox {
                    Layout.margins: acrossConfig.itemSpacing
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    text: nodeModel.raw
                }

                CardBox {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    color: "transparent"
                }
            }
        }
    }
}
