import Arktoria.ACross
import QtQuick
import QtQuick.Controls

SpinBox {
    id: spinBox

    property int controlSpacing: 0
    property int controlWidth: 10
    property int controlHeight: 8

    implicitWidth: 96
    implicitHeight: 32
    leftPadding: acrossConfig.itemSpacing * 3
    rightPadding: acrossConfig.itemSpacing
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
        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();
            ctx.moveTo(0, height);
            ctx.lineTo(width, height);
            ctx.lineTo(Math.round(width / 2), 0);
            ctx.closePath();
            ctx.fillStyle = spinBox.up.pressed ? acrossConfig.backgroundColor : acrossConfig.highlightColor;
            ctx.fill();
        }

        Connections {
            function onPressedChanged() {
                upCanvas.requestPaint();
            }

            target: spinBox.up
        }

    }

    down.indicator: Canvas {
        id: downCanvas

        x: spinBox.width - width - spinBox.rightPadding
        y: spinBox.height - spinBox.bottomPadding - height + controlSpacing
        implicitWidth: controlWidth
        implicitHeight: controlHeight
        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();
            ctx.moveTo(0, 0);
            ctx.lineTo(width, 0);
            ctx.lineTo(Math.round(width / 2), height);
            ctx.closePath();
            ctx.fillStyle = spinBox.down.pressed ? acrossConfig.backgroundColor : acrossConfig.highlightColor;
            ctx.fill();
        }

        Connections {
            function onPressedChanged() {
                downCanvas.requestPaint();
            }

            target: spinBox.down
        }

    }

    background: Rectangle {
        color: acrossConfig.deepColor
        radius: acrossConfig.borderRadius
    }

}
