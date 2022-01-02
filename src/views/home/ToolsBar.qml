import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    implicitHeight: 48
    implicitWidth: 312

    Rectangle {
        anchors.fill: parent
        color: acrossConfig.backgroundColor

        RowLayout {
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing

            TextFieldBox {
                id: searchTextField

                Layout.fillWidth: true
                Layout.fillHeight: true
                onTextChanged: {
                    if (this.text === "")
                        acrossGroups.clearSearch();
                    else
                        acrossGroups.search(text);
                }
            }

            SVGBox {
                Layout.preferredWidth: 24
                source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/search.svg"
                sourceWidth: 24
                sourceHeight: 24

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        if (searchTextField.text !== "")
                            acrossGroups.search(searchTextField.text);
                        else
                            acrossGroups.clearSearch();
                    }
                    onEntered: {
                        cursorShape = Qt.PointingHandCursor;
                    }
                    onExited: {
                        cursorShape = Qt.ArrowCursor;
                    }
                }

            }

        }

    }

}
