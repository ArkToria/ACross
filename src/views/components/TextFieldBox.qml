import QtQuick 2.0
import QtQuick.Controls 2.12

TextField {
    color: acrossConfig.deepTextColor

    property string backgroundColor: acrossConfig.deepColor

    background: Rectangle {
        color: backgroundColor
        radius: acrossConfig.borderRadius
    }
}
