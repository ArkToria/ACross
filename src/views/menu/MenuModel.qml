import QtQuick

ListModel {
    ListElement {
        name: qsTr("Home")
        source: "qrc:/src/views/home/HomePage.qml"
        iconSource: "/home.svg"
    }
    ListElement {
        name: qsTr("Logs")
        source: "qrc:/src/views/log/LogPage.qml"
        iconSource: "/log.svg"
    }
    ListElement {
        name: qsTr("Setting")
        source: "qrc:/src/views/setting/SettingPage.qml"
        iconSource: "/setting.svg"
    }
    ListElement {
        name: qsTr("Help")
        source: "qrc:/src/views/help/HelpPage.qml"
        iconSource: "/help.svg"
    }
}
