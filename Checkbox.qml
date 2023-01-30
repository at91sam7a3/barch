import QtQuick 2.15

Row {
    property alias text: message.text
    property bool checked: false
    spacing: 5
    Text {
        id: message
        text: ""
        verticalAlignment: Text.AlignVCenter
        height: 20
    }
    Rectangle {
        width: 20
        height: 20
        border.color: "black"
        border.width: 2
        color: checked ? "grey" : "white"
        MouseArea{
            anchors.fill: parent
            onClicked: checked = !checked
        }
    }
}
