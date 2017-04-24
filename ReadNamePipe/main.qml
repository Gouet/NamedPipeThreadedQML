import QtQuick 2.3
import QtQuick.Window 2.2
import com.namedPipe 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log(qsTr('Clicked on background. Text: "' + textEdit.text + '"'))
        }
    }

    NamedPipe {
        property var name: 0
        id: namedPipe
        fileIn: "\\\\.\\pipe\\Pipe"
        fileOut: "\\\\.\\pipe\\Pipe2"

        onReceiveData: {
            textEdit.text = data
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
}
