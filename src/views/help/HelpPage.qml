import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

Item {
    implicitWidth: 840
    implicitHeight: 720

    function getLogo(style) {
        if (style === "dev") {
            return "qrc:/misc/design/lines.svg"
        }

        if (style === "light") {
            return "qrc:/misc/design/logo_pure.svg"
        } else {
            return "qrc:/misc/design/logo.svg"
        }
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing

        clip: true
        contentHeight: column.height

        ColumnLayout {
            id: column
            spacing: acrossConfig.itemSpacing
            implicitWidth: scrollView.availableWidth

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

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}
}
##^##*/

