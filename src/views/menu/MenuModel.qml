import QtQuick

ListModel {
    ListElement {
        name: qsTr("Home")
        source: "qrc:/ACross/src/views/home/HomePage.qml"
        iconSource: "/home.svg"
    }
    //    ListElement {
    //        name: qsTr("Logs")
    //        source: "qrc:/ACross/src/views/log/LogPage.qml"
    //        iconSource: "/log.svg"
    //    }
    ListElement {
        name: qsTr("Setting")
        source: "qrc:/ACross/src/views/setting/SettingPage.qml"
        iconSource: "/setting.svg"
    }
    ListElement {
        name: qsTr("Help")
        source: "qrc:/ACross/src/views/help/HelpPage.qml"
        iconSource: "/help.svg"
    }
}
