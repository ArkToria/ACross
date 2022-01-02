/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/

import "./typescripts/main.js" as MainJS
import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

ApplicationWindow {
    id: mainWindow

    width: 960
    height: 720
    minimumWidth: 960
    minimumHeight: 520
    visible: mainWindowVisible
    title: qsTr("ACross")
    onVisibilityChanged: {
        acrossTray.toggleVisibilitySetText(mainWindow.visible);
    }
    onClosing: {
        if (acrossConfig.enableStartFromMinimized)
            mainWindow.hide();
        else
            Qt.quit();
    }

    Connections {
        function onSignalShow() {
            MainJS.toggleVisibilty(mainWindow);
        }

        function onSignalQuit() {
            Qt.quit();
        }

        function onSignalIconActivated() {
            MainJS.toggleVisibilty(mainWindow);
        }

        target: acrossTray
    }

    DarkBackground {
        id: darkBackground

        anchors.fill: parent
        z: 1
    }

    PopNotify {
        id: popNotify

        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: acrossConfig.itemSpacing * 2
        z: 1
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        MainPanel {
            id: mainPanel

            Layout.fillHeight: true
            Layout.preferredWidth: 120
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
                interactive: false

                HomePage {
                    id: homePage
                }

                //                LogPage {
                //                    id: logPage
                //                }
                //                RoutingPage {
                //                    id: routingPage
                //                }
                SettingPage {
                    id: settingPage
                }

                HelpPage {
                    id: helpPage
                }

            }

        }

    }

}
