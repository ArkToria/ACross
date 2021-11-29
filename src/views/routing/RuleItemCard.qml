import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Arktoria.ACross

Item {
    implicitWidth: 240
    implicitHeight: 36
    state: "NormalState"

    states: [
        State {
            name: "EditState"
            PropertyChanges {
                target: editButton
                visible: true
            }
        },
        State {
            name: "NormalState"
            PropertyChanges {
                target: editButton
                visible: false
            }
        }
    ]

    function getColor(tag) {
        if (tag.includes("geo")) {
            indicatorLight.color = acrossConfig.highlightColor
            return
        }

        if (tag.includes("ext")) {
            indicatorLight.color = acrossConfig.warnColor
            return
        }

        indicatorLight.color = acrossConfig.styleColor
    }

    RowLayout {
        anchors.fill: parent
        spacing: acrossConfig.itemSpacing * 2

        Rectangle {
            id: indicatorLight
            width: 8
            height: 8
            radius: 4
            color: acrossConfig.highlightColor
        }

        TextFieldBox {
            id: tagText
            Layout.fillWidth: true
            clip: true

            text: modelData
            color: acrossConfig.textColor
        }

        SVGBox {
            id: editButton

            source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/remove.svg"
            sourceWidth: 16
            sourceHeight: 16
        }
    }

    Connections {
        target: acrossConfig

        function onCurrentThemeChanged() {
            getColor(modelData)
        }
    }

    Component.onCompleted: {
        getColor(modelData)
    }
}
