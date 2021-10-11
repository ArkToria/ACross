import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ACross

Item {
    implicitHeight: 48
    implicitWidth: 312

    RowLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing

        TextFieldBox {
            id: searchTextField
            Layout.fillWidth: true
            Layout.fillHeight: true

            onTextChanged: {
                if (this.text === "") {
                    acrossGroups.restore()
                } else {
                    acrossGroups.search(text)
                }
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
                    if (searchTextField.model.text !== "")
                        acrossGroups.search(searchTextField.text)
                }

                onEntered: {
                    cursorShape = Qt.PointingHandCursor
                }

                onExited: {
                    cursorShape = Qt.ArrowCursor
                }
            }
        }
    }
}
