import QtQuick

ListModel {
    ListElement {
        name: qsTr("Home")
        source: "qrc:/Arktoria/ACross/src/views/home/HomePage.qml"
        iconSource: "/home.svg"
    }
    //    ListElement {

    //        name: qsTr("Logs")
    //        source: "qrc:/Arktoria/ACross/src/views/log/LogPage.qml"
    //        iconSource: "/log.svg"
    //    }
    //    ListElement {
    //        name: qsTr("Routing")
    //        source: "qrc:/Arktoria/ACross/src/views/routing/RoutingPage.qml"
    //        iconSource: "/routing.svg"
    //    }
    ListElement {
        name: qsTr("Setting")
        source: "qrc:/Arktoria/ACross/src/views/setting/SettingPage.qml"
        iconSource: "/setting.svg"
    }

    ListElement {
        name: qsTr("Help")
        source: "qrc:/Arktoria/ACross/src/views/help/HelpPage.qml"
        iconSource: "/help.svg"
    }

}
