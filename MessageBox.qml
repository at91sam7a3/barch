import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

Window {
    visible: MessageBoxController.visible
    width: 200
    height: 100
    minimumHeight: 100
    maximumHeight: 100
    minimumWidth: 200
    maximumWidth: 200
    title: "Warning"
    color: "white"
    flags: Qt.Tool |Qt.Popup| Qt.WindowCloseButtonHint | Qt.Dialog
    Rectangle {
        color: "white"
        border.color:"black"
        border.width: 2
        anchors.fill: parent
        ColumnLayout {
            anchors.margins: 20
            anchors.fill: parent
            Text {
                text: MessageBoxController.messageText
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter

            }
            Button {
                width: 100
                height: 50
                text: "Close"
                onClick: MessageBoxController.visible = false
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }
}
