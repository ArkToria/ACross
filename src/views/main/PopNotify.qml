import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Arktoria.ACross

Item {
<<<<<<< HEAD
    id: popNotifyControl
    implicitWidth: 320
=======
    id: control
    implicitWidth: 240
>>>>>>> 7cc215f (rebase from master)
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
<<<<<<< HEAD
                popNotifyControl.visible = false
=======
                control.visible = false
>>>>>>> 7cc215f (rebase from master)
            }
        }
    }

    function notify(title = "", message = "") {
<<<<<<< HEAD
        popNotifyControl.visible = true
=======
        control.visible = true
>>>>>>> 7cc215f (rebase from master)
        popNotifyModel.append({
                                  "title": title,
                                  "message": message
                              })
    }
}
