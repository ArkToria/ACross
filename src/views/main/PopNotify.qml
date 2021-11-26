import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ACross

Item {
    id: control
    implicitWidth: 240
    implicitHeight: 84 * 3
    clip: true

    ListModel {
        id: popNotifyModel
    }

    ListView {
        id: popNotifyListView
        anchors.fill: parent

        verticalLayoutDirection: ListView.BottomToTop

        model: popNotifyModel

        delegate: PopMessageBox {
            title: model.title
            message: model.message
        }

        add: Transition {
            NumberAnimation {
                properties: "x,y"
                easing.type: Easing.OutBack
            }
        }

        move: add
    }

    function notify(title = "", message = "") {
        popNotifyModel.append({
                                  "title": title,
                                  "message": message
                              })
    }
}
