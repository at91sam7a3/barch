import QtQuick 2.15

Rectangle {
    id: root
    property alias text: buttonText.text
    signal click()

    Text {
        id: buttonText
        anchors.centerIn: root

    }

    MouseArea {
        id: ma
        hoverEnabled: true
        anchors.fill: root
        onClicked: root.click()
    }
    states: [
        State {
            name: "main"
            when: (!ma.containsMouse)
            PropertyChanges {
                target: root

                color: "white"
            }
        },
        State {
            name: "hovered"
            when: (ma.containsMouse && !ma.pressed)
            PropertyChanges {
                target: root

                color: "#FACE8D"
            }
        },
        State {
            name: "pressed"
            when: (ma.pressed)
            PropertyChanges {
                target: root

                color: "yellow"
            }
        }
    ]
}
