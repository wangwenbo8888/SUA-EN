import QtQuick 2.0

Rectangle {
    id: container

    property alias text: buttonLabel.text
    property int font_size: 18
    property alias fillColor: container.color
    property alias textColor : buttonLabel.color

    signal clicked

    width: buttonLabel.width + 20; height: buttonLabel.height + 5
    border { width: 1; color: Qt.darker(activePalette.button) }
    antialiasing: true
    radius: 10

    // color the button with a gradient
//    gradient: Gradient {
//        GradientStop {
//            position: 0.0
//            color: {
//                if (mouseArea.pressed)
//                    return activePalette.dark
//                else
//                    return activePalette.light
//            }
//        }
//        GradientStop { position: 1.0; color: activePalette.button }
//    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: container.clicked();
    }

    Text {
        id: buttonLabel
        anchors.centerIn: container
        color: activePalette.buttonText
        text: "Button"
        font.bold: true
        font.pixelSize: font_size
    }
}

