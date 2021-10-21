import QtQuick
import QtQuick.Controls

SpinBox {
    id: spinBox
    implicitWidth: 96
    implicitHeight: 36

    leftPadding: acrossConfig.itemSpacing
    rightPadding: acrossConfig.itemSpacing

    property int controlSpacing: 0
    property int controlWidth: 10
    property int controlHeight: 8

    editable: true
    contentItem: TextInput {
        text: spinBox.value
        verticalAlignment: Text.AlignVCenter
        color: acrossConfig.deepTextColor
        selectionColor: acrossConfig.highlightColor
        selectedTextColor: acrossConfig.highlightTextColor
        readOnly: !spinBox.editable
        validator: spinBox.validator
        inputMethodHints: Qt.ImhFormattedNumbersOnly
    }

    up.indicator: Canvas {
        id: upCanvas
        x: spinBox.width - width - spinBox.rightPadding
        y: spinBox.topPadding - controlSpacing

        implicitWidth: controlWidth
        implicitHeight: controlHeight

        Connections {
            target: spinBox.up
            function onPressedChanged() {
                upCanvas.requestPaint()
            }
        }

        onPaint: {
            var ctx = getContext("2d")

            ctx.reset()
            ctx.moveTo(0, height)
            ctx.lineTo(width, height)
            ctx.lineTo(Math.round(width / 2), 0)
            ctx.closePath()
            ctx.fillStyle = spinBox.up.pressed ? acrossConfig.backgroundColor : acrossConfig.highlightColor
            ctx.fill()
        }
    }

    down.indicator: Canvas {
        id: downCanvas
        x: spinBox.width - width - spinBox.rightPadding
        y: spinBox.height - spinBox.bottomPadding - height + controlSpacing

        implicitWidth: controlWidth
        implicitHeight: controlHeight

        Connections {
            target: spinBox.down
            function onPressedChanged() {
                downCanvas.requestPaint()
            }
        }

        onPaint: {
            var ctx = getContext("2d")

            ctx.reset()
            ctx.moveTo(0, 0)
            ctx.lineTo(width, 0)
            ctx.lineTo(Math.round(width / 2), height)
            ctx.closePath()
            ctx.fillStyle = spinBox.down.pressed ? acrossConfig.backgroundColor : acrossConfig.highlightColor
            ctx.fill()
        }
    }

    background: CardBox {
        color: acrossConfig.deepColor
        layer.enabled: false
    }
}
