import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Arktoria.ACross

Item {
    id: popNotifyControl
    implicitWidth: 320
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
            to: model.to
            groupID: model.groupID
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
                popNotifyControl.visible = false
            }
        }
    }

    function notify(title = "", message = "") {
        popNotifyControl.visible = true
        popNotifyModel.append({
                                  "title": title,
                                  "message": message,
                                  "groupID": -1,
                                  "to": -1.0,
                                  "index": -1
                              })
    }
    function notifyWithProgress(title = "", message = "", index, groupID, total = 0) {
        popNotifyControl.visible = true
        popNotifyModel.append({
            "title": title,
            "message": message,
            "index": index,
            "groupID": groupID,
            "to": total
        })
    }
}
