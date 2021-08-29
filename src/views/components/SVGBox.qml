import QtQuick 2.0

Item {
    property string source
    property int sourceWidth
    property int sourceHeight

    implicitWidth: sourceWidth > 0 ? sourceWidth : 64
    implicitHeight: sourceHeight > 0 ? sourceHeight : 64

    Image {
        anchors.fill: parent
        source: parent.source
        sourceSize.width: parent.sourceWidth
        sourceSize.height: parent.sourceHeight
    }
}
