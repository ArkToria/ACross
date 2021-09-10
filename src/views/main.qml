import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import ACross 1.0

import "./menu"

Window {
    id: mainWindow
    width: 960
    height: 720
    minimumWidth: 960
    minimumHeight: 720
    visible: true
    title: qsTr("ACross")

    Rectangle {
        id: popBackground
        anchors.fill: parent
        z: 1

        color: "black"
        opacity: 0.7
    }

    RowLayout {
        id: mainComponent
        anchors.fill: parent
        spacing: 0

        state: "NormalState"

        states: [
            State {
                name: "PopUpState"
                PropertyChanges {
                    target: popBackground
                    visible: true
                }
            },
            State {
                name: "NormalState"
                PropertyChanges {
                    target: popBackground
                    visible: false
                }
            }
        ]

        MenuModel {
            id: menuModel
        }

        MainPanel {
            id: mainPanel
            Layout.fillHeight: true
            Layout.preferredWidth: 120
            z: 1
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: acrossConfig.deepColor

            Loader {
                id: pageLoader
                anchors.fill: parent

                source: menuModel.get(0).source
            }
        }
    }

    function toggleVisibilty(){
        if(mainWindow.visible === false){
            mainWindow.show()
        } else{
            mainWindow.hide()
        }
    }
    Connections {
        target: acrossTray
        function onSignalShow() {
            toggleVisibilty()
        }
        function onSignalQuit() {
            Qt.quit()
        }
        function onSignalIconActivated() {
            toggleVisibilty()
        }
        
    }

    onVisibilityChanged:{
        acrossTray.toggleVisibilitySetText(mainWindow.visible)
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/

