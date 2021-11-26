import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

CardBox {
    implicitHeight: 300

    property int fontSize: 14

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 4
        spacing: acrossConfig.itemSpacing

        Label {
            text: qsTr("Release News")
            font.pointSize: Math.round(fontSize * 1.2)
            color: acrossConfig.textColor
        }

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true

            spacing: acrossConfig.itemSpacing
            model: acrossConfig.versionNews
            delegate: Label {
                text: modelData
                textFormat: Text.MarkdownText
                color: acrossConfig.textColor
                
                onLinkActivated : (link) => {
                    Qt.openUrlExternally(link)
                }
            }
        }
    }
}
