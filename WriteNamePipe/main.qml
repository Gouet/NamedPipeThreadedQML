import QtQuick 2.3
import QtQuick.Window 2.2
import com.namedPipe 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("WRITE test")
    property var name: 0

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log(qsTr('Clicked on background. Text: "' + textEdit.text + '"'))
        }
    }

    TextEdit {
        id: textEdit
        text: qsTr("Enter some text...")
        verticalAlignment: Text.AlignVCenter
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        Rectangle {
            anchors.fill: parent
            anchors.margins: -10
            color: "transparent"
            border.width: 1
        }

        onTextChanged: {
            namedPipe.send(textEdit.text)
        }
    }

    NamedPipe {
        id: namedPipe;
        fileIn: "\\\\.\\pipe\\Pipe2"
        fileOut: "\\\\.\\pipe\\Pipe"

        onReceiveData: {
            console.log("receive : " + data)
        }

    }
}
