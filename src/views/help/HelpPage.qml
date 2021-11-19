import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic as ControlsBasic

import ACross

Item {
    implicitWidth: 840
    implicitHeight: 720

    ControlsBasic.ScrollView {
        id: scrollView
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing

        clip: true
        contentHeight: column.height

        Flickable {
            ColumnLayout {
                id: column
                spacing: acrossConfig.itemSpacing
                width: scrollView.availableWidth

                About {
                    Layout.fillWidth: true
                }

                Licenses {
                    Layout.fillWidth: true
                }

                ChangeLog {
                    Layout.fillWidth: true
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}
}
##^##*/

