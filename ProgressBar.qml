import QtQuick 2.15

    Rectangle {
        id: root
        property int progress: 0

        color: "grey"
        Rectangle
        {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: root.width*progress/100
            color: "green"
        }
        Text {
            id: name
            text: progress + "%"
            anchors.centerIn: parent
        }
    }
