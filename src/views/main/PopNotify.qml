import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: popNotifyControl

    function notify(title = "", message = "") {
        popNotifyControl.visible = true;
        popNotifyModel.append({
            "title": title,
            "message": message
        });
    }

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
        move: add
        onCountChanged: {
            if (count === 0)
                popNotifyControl.visible = false;

        }

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

    }

}
