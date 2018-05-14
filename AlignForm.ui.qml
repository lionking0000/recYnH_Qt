import QtQuick 2.4

Item {
    width: 400
    height: 400

    TextInput {
        id: textInput
        x: 51
        y: 67
        width: 80
        height: 20
        text: qsTr("Text Input")
        font.pixelSize: 12
    }

    TextEdit {
        id: textEdit
        x: 51
        y: 136
        width: 80
        height: 20
        text: qsTr("Text Edit")
        font.pixelSize: 12
    }

    Text {
        id: text1
        x: 62
        y: 200
        text: qsTr("Text")
        font.pixelSize: 12
    }
}
