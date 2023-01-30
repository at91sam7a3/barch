import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Barch Converter")
    ColumnLayout {
        anchors.fill: parent
        Rectangle {
            height: 25
            Layout.fillWidth: true
            border.color: "black"
            border.width: 1

            Text {
                text: "Current folder: "+FileListModel.currentFolder
                elide: Text.ElideRight
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: FileListModel

            delegate:Row {
                Rectangle {
                    height: 25
                    width: 200
                    border.color: "black"
                    border.width: 1

                    Text {
                        text: Name
                        elide: Text.ElideRight
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                Rectangle {
                    height: 25
                    width: 100
                    border.color: "black"
                    border.width: 1

                    Text {
                        text: Type
                        elide: Text.ElideRight
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                Rectangle {
                    height: 25
                    width: 100
                    border.color: "black"
                    border.width: 1

                    Text {
                        text: Size
                        elide: Text.ElideRight
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                Rectangle {
                    height: 25
                    width: 100
                    border.color: "black"
                    border.width: 1

                    Button {
                        visible: Progress === 0
                        text: Type === "BMP"  ? "Pack" : "Unpack"
                        anchors.fill: parent
                        anchors.margins: 3
                        onClick: uiController.convert(Name);
                    }

                    ProgressBar {
                        visible: Progress !==0
                        anchors.fill: parent
                        anchors.margins: 3
                        progress: Progress
                    }
                }

            }
        }

    }
}
