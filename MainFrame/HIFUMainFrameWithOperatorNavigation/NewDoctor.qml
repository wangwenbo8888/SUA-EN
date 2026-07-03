import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ColumnLayout {
    id:newDr_gui
    width: 680
    height: 384
    anchors.margins: 3
    spacing: 5

    signal addNewDr(string fname,string lname,string gender,string age,string post,string isAdmin,string mobile,string email)

    //title
    Item
    {
        id: title_bar
        width: newDr_gui.width
        height: newDr_gui.height * 0.1
        anchors.top: newDr_gui.top

        Text
        {
            id: title
            anchors.centerIn: parent
            text: "Add New Doctor"
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
        anchors{left:parent.left;leftMargin: 10;top:parent.top;topMargin: 65}
    }

    TextField {
        id:firstName_text
        anchors{left:firstName_label.right;top:parent.top;topMargin: 62}
        }

    Text{
        id:lastName_label
        text:"Last Name:"
        anchors{left:firstName_text.right;leftMargin: 80;top:parent.top;topMargin: 65}
    }

    TextField {
        id:lastName_text
        anchors{left:lastName_label.right;top:parent.top;topMargin: 62}
        }

    Text{
        id:gender_label
        text:"Gender:"
        anchors{left:parent.left;leftMargin: 10;top:firstName_label.bottom;topMargin: 30}
    }

    ComboBox{
        id:gender_combo
        anchors{left:gender_label.right;leftMargin: 24;top:firstName_label.bottom;topMargin: 27}
        model:["M","F"]
    }

    Text{
        id:age_label
        text:"Age:"
        anchors{left:gender_combo.right;leftMargin: 82;top:firstName_label.bottom;topMargin: 30}
    }

    TextField {
        id:age_text
        anchors{left:age_label.right;leftMargin: 35;top:firstName_label.bottom;topMargin: 27}
        }

    Text{
        id:post_label
        text:"Post:"
        anchors{left:parent.left;leftMargin: 10;top:age_label.bottom;topMargin: 30}
    }

    ComboBox{
        id:post_combo
        anchors{left:post_label.right;leftMargin: 36;top:age_label.bottom;topMargin: 27}
        model:["Doctor","Intern","Physical Therapist"]
    }

    Text{
        id:admin_label
        text:"Authorization:"
        anchors{left:post_combo.right;leftMargin: 81;top:gender_label.bottom;topMargin: 30}
    }

    ComboBox{
        id:admin_combo
        anchors{left:admin_label.right;top:gender_label.bottom;topMargin: 27}
        model:["True","False"]
    }

    Text{
        id:mobile_label
        text:"Mobile:"
        anchors{left:parent.left;leftMargin: 10;top:post_label.bottom;topMargin: 30}
    }

    TextField {
        id:mobile_text
        anchors{left:mobile_label.right;leftMargin: 25;top:post_label.bottom;topMargin: 27}
        } 

    Text{
        id:email_label
        text:"E-mail:"
        anchors{left:mobile_text.right;leftMargin: 79;top:post_label.bottom;topMargin: 30}
    }

    TextField {
        id:email_text
        anchors{left:email_label.right;leftMargin: 20;top:post_label.bottom;topMargin: 27}
        }

    //Button
    Button
    {
        id:btn_add
        text: "Add"
        anchors{left:parent.left; leftMargin:300; top:mobile_label.bottom; topMargin: 50}
        onClicked:addDr()
    }

    function addDr()
    {
        if (firstName_text.text == "" || lastName_text.text == "" || gender_combo.currentText == "" || age_text.text == "" || post_combo.currentText == "" || admin_combo.currentText == "" || mobile_text.text == "" || email_text.text == "")
        {
            messageDialog.text = "Not enough information."
            messageDialog.visible = true
            return
        }
        addNewDr(firstName_text.text,lastName_text.text,gender_combo.currentText,age_text.text,post_combo.currentText,admin_combo.currentText,mobile_text.text,email_text.text)
    }

    function closeMsgBox()
    {
        messageDialog.close()
    }
}

