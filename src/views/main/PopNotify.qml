import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ACross

Item {
    id: control
    implicitWidth: 240
    implicitHeight: popNotifyListView.count >= 3 ? 84 * 3 : 84 * popNotifyListView.count
    clip: true
    visible: false

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

        onCountChanged: {
            if (count === 0) {
                control.visible = false
            }
        }
    }

    function notify(title = "", message = "") {
        control.visible = true
        popNotifyModel.append({
                                  "title": title,
                                  "message": message
                              })
    }
}
