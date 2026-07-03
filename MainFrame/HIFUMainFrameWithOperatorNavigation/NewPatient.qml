import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ColumnLayout {
    id:newPt_gui
    width: 680
    height: 384
    anchors.margins: 3
    spacing: 5

    signal addNewPt(string fname,string lname,string age,string mobile,string x,string y,string z)

    //title
    Item
    {
        id: title_bar
        width: newPt_gui.width
        height: newPt_gui.height * 0.1
        anchors.top: newPt_gui.top

        Text
        {
            id: title
            anchors.centerIn: parent
            text: "Add New Patient"
            font.bold: true
            color: "dark red"
        }
    }

    //MessageBox
    MessageDialog
    {
        id: messageDialog
        title: "Warning"
        icon: StandardIcon.Warning
        text: "Check your information."
        onAccepted:closeMsgBox()
        Component.onCompleted: visible = false
    }

    //Fill in information
    Text{
        id:firstName_label
        text:"First Name:"
        anchors{left:parent.left;leftMargin: 10;top:parent.top;topMargin: 45}
    }

    TextField {
        id:firstName_text
        anchors{left:firstName_label.right;top:parent.top;topMargin: 42}
        }

    Text{
        id:lastName_label
        text:"Last Name:"
        anchors{left:firstName_text.right;leftMargin: 80;top:parent.top;topMargin: 45}
    }

    TextField {
        id:lastName_text
        anchors{left:lastName_label.right;top:parent.top;topMargin: 42}
        }

    Text{
        id:age_label
        text:"Age:"
        anchors{left:parent.left;leftMargin: 10;top:firstName_label.bottom;topMargin: 30}
    }

    TextField {
        id:age_text
        anchors{left:age_label.right;leftMargin: 42;top:firstName_label.bottom;topMargin: 27}
        }

    Text{
        id:mobile_label
        text:"Mobile:"
        anchors{left:age_text.right;leftMargin: 80;top:firstName_label.bottom;topMargin: 30}
    }

    TextField {
        id:mobile_text
        anchors{left:mobile_label.right;leftMargin: 18;top:firstName_label.bottom;topMargin: 27}
        }

    Text{
        id:size_label
        text:"Tumor Size:"
        anchors{left:parent.left;leftMargin: 10;top:mobile_label.bottom;topMargin: 30}
    }

    Text{
        id:sizeX_label
        text:"Length:"
        anchors{left:parent.left;leftMargin: 10;top:size_label.bottom;topMargin: 30}
    }

    TextField {
        id:sizeX_text
        anchors{left:sizeX_label.right;leftMargin: 25;top:size_label.bottom;topMargin: 27}
        }

    Text{
        id:sizeX_unit_label
        text:"mm"
        anchors{left:sizeX_text.right;leftMargin: 5;top:size_label.bottom;topMargin: 30}
    }

    Text{
        id:sizeY_label
        text:"Width:"
        anchors{left:parent.left;leftMargin: 10;top:sizeX_label.bottom;topMargin: 30}
    }

    TextField {
        id:sizeY_text
        anchors{left:sizeY_label.right;leftMargin: 31;top:sizeX_label.bottom;topMargin: 27}
        }

    Text{
        id:sizeY_unit_label
        text:"mm"
        anchors{left:sizeY_text.right;leftMargin: 5;top:sizeX_label.bottom;topMargin: 30}
    }

    Text{
        id:sizeZ_label
        text:"Height:"
        anchors{left:parent.left;leftMargin: 10;top:sizeY_label.bottom;topMargin: 30}
    }

    TextField {
        id:sizeZ_text
        anchors{left:size_label.right;leftMargin: 1;top:sizeY_label.bottom;topMargin: 27}
        }

    Text{
        id:sizeZ_unit_label
        text:"mm"
        anchors{left:sizeZ_text.right;leftMargin: 5;top:sizeY_label.bottom;topMargin: 30}
    }

    //Button
    Button
    {
        id:btn_add
        text: "Add"
        anchors{left:parent.left; leftMargin:300; top:sizeZ_label.bottom; topMargin: 40}
        onClicked:addPt()
    }

    function addPt()
    {
        if (firstName_text.text == "" || lastName_text.text == "" || age_text.text == "" || mobile_text.text == "" || sizeX_text.text == "" || sizeY_text.text == "" || sizeZ_text.text == "")
        {
            messageDialog.text = "Not enough information."
            messageDialog.visible = true
            return
        }
        addNewPt(firstName_text.text,lastName_text.text,age_text.text,mobile_text.text,sizeX_text.text,sizeY_text.text,sizeZ_text.text)
    }

    function closeMsgBox()
    {
        messageDialog.close()
    }
}

