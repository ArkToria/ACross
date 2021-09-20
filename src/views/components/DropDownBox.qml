import QtQuick
import QtQuick.Controls

ComboBox {
    id: comboBox
    implicitWidth: 128
    implicitHeight: 36

    property int controlWidth: 10
    property int controlHeight: 8

    contentItem: Text {
        anchors.left: parent.left
        anchors.leftMargin: acrossConfig.itemSpacing

        text: comboBox.displayText
        color: acrossConfig.deepTextColor
        verticalAlignment: Text.AlignVCenter
    }

    delegate: ItemDelegate {
        implicitWidth: comboBox.width

        contentItem: Text {
            id: itemText
            text: modelData
            color: hovered ? acrossConfig.highlightTextColor : acrossConfig.textColor
        }
    }

    indicator: Canvas {
        id: canvas
        x: comboBox.width - width - comboBox.rightPadding
        y: Math.round((comboBox.height - height) / 2)

        implicitWidth: controlWidth
        implicitHeight: controlHeight

        Connections {
            target: comboBox
            function onPressedChanged() {
                canvas.requestPaint()
            }
        }

        onPaint: {
            var ctx = getContext("2d")

            ctx.reset()
            ctx.moveTo(0, 0)
            ctx.lineTo(width, 0)
            ctx.lineTo(Math.round(width / 2), height)
            ctx.closePath()
            ctx.fillStyle
                    = comboBox.pressed ? acrossConfig.backgroundColor : acrossConfig.highlightColor
            ctx.fill()
        }
    }

    popup: Popup {
        y: comboBox.height

        implicitWidth: comboBox.width

        contentItem: ListView {
            implicitHeight: contentHeight
            implicitWidth: comboBox.width

            clip: true
            model: comboBox.delegateModel
            currentIndex: comboBox.highlightedIndex
            ScrollIndicator.vertical: ScrollIndicator {}

            highlight: Rectangle {
                color: acrossConfig.highlightColor
            }
        }

        background: CardBox {
            color: acrossConfig.backgroundColor
            border.width: 1
            border.color: acrossConfig.deepColor
        }
    }

    background: CardBox {
        color: acrossConfig.deepColor
        layer.enabled: false
    }
}
