import QtQuick 2.15
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import ACross 1.0

import "./menu"
import "./log"
import "./home"
import "./setting"
import "./help"

ApplicationWindow {
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

            SwipeView {
                id: pageLoader
                anchors.fill: parent

                currentIndex: 0
                orientation: Qt.Vertical

                HomePage {
                    id: homePage
                }

                LogPage {
                    id: logPage
                }

                SettingPage {
                    id: settingPage
                }

                HelpPage {
                    id: helpPage
                }
            }
        }
    }

    function toggleVisibilty() {
        if (mainWindow.visible === false) {
            mainWindow.show()
        } else {
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

    onVisibilityChanged: {
        acrossTray.toggleVisibilitySetText(mainWindow.visible)
    }

    onClosing: {
        if (acrossConfig.enableTray) {
            mainWindow.hide()
        } else {
            Qt.quit()
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/

