import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ColumnLayout {
    id:doctorBasic_gui
    anchors.margins: 3
    spacing: 5

    property int editFlag : 0
    property alias idText : id_text.text
    property alias firstNameText : firstName_text.text
    property alias lastNameText : lastName_text.text
    property alias genderText : gender_text.text
    property alias genderComboText: gender_combo.currentText
    property alias ageText : age_text.text
    property alias postText : post_text.text
    property alias postComboText : post_combo.currentText
    property alias isAdminText: admin_combo.currentText
    property alias mobileText : mobile_text.text
    property alias emailText : email_text.text
//    property alias btnEditVisibility : btn_edit.visible
//    property alias btnCommitVisibility : btn_commit.visible

    signal commitClicked()

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

    //Information
    Text{
        id:id_label
        text:"Staff ID:"
        anchors{left:parent.left;leftMargin: 10;top:parent.top;topMargin: 25}
    }

    TextField {
        id:id_text
        anchors{left:firstName_label.right;top:parent.top;topMargin: 22}
        readOnly: true
        }

    Text{
        id:firstName_label
        text:"First Name:"
        anchors{left:parent.left;leftMargin: 10;top:id_label.bottom;topMargin: 30}
    }

    TextField {
        id:firstName_text
        anchors{left:firstName_label.right;top:id_label.bottom;topMargin: 27}
        readOnly: true
        }

    Text{
        id:lastName_label
        text:"Last Name:"
        anchors{left:firstName_text.right;leftMargin: 80;top:id_label.bottom;topMargin: 30}
    }

    TextField {
        id:lastName_text
        anchors{left:lastName_label.right;top:id_label.bottom;topMargin: 27}
        readOnly: true
        }

    Text{
        id:gender_label
        text:"Gender:"
        anchors{left:parent.left;leftMargin: 10;top:firstName_label.bottom;topMargin: 30}
    }

    TextField{
        id:gender_text
        anchors{left:gender_label.right;leftMargin: 24;top:firstName_label.bottom;topMargin: 27}
        readOnly: true
    }

    ComboBox{
        id:gender_combo
        anchors{left:gender_label.right;leftMargin: 24;top:firstName_label.bottom;topMargin: 27}
        model:["M","F"]
        visible: false
    }

    Text{
        id:age_label
        text:"Age:"
        anchors{left:gender_text.right;leftMargin: 82;top:firstName_label.bottom;topMargin: 30}
    }

    TextField {
        id:age_text
        anchors{left:age_label.right;leftMargin: 35;top:firstName_label.bottom;topMargin: 27}
        readOnly: true
        }

    Text{
        id:post_label
        text:"Post:"
        anchors{left:parent.left;leftMargin: 10;top:age_label.bottom;topMargin: 30}
    }

    TextField{
        id:post_text
        anchors{left:post_label.right;leftMargin: 36;top:age_label.bottom;topMargin: 27}
        readOnly: true
    }

    ComboBox{
        id:post_combo
        anchors{left:post_label.right;leftMargin: 36;top:age_label.bottom;topMargin: 27}
        model:["Doctor","Intern","Physical Therapist"]
        visible: false
    }

    Text{
        id:admin_label
        text:"Autherization:"
        anchors{left:post_text.right;leftMargin: 82;top:age_label.bottom;topMargin: 30}
        visible: false
    }

    ComboBox{
        id:admin_combo
        anchors{left:admin_label.right;leftMargin: 1;top:age_label.bottom;topMargin: 27}
        model:["True","False"]
        visible: false
    }

    Text{
        id:mobile_label
        text:"Mobile:"
        anchors{left:parent.left;leftMargin: 10;top:post_label.bottom;topMargin: 30}
    }

    TextField {
        id:mobile_text
        anchors{left:mobile_label.right;leftMargin: 25;top:post_label.bottom;topMargin: 27}
        readOnly: true
        }

    Text{
        id:email_label
        text:"E-mail:"
        anchors{left:mobile_text.right;leftMargin: 79;top:post_label.bottom;topMargin: 30}
    }

    TextField {
        id:email_text
        anchors{left:email_label.right;leftMargin: 20;top:post_label.bottom;topMargin: 27}
        readOnly: true
        }

    //Buttons
    Button
    {
        id:btn_edit
        text: "Edit"
        anchors{left:parent.left; leftMargin:200; top:mobile_text.bottom; topMargin: 50}
        onClicked:{
            editFlag = 1
            firstName_text.readOnly = false
            lastName_text.readOnly = false
            age_text.readOnly = false
            mobile_text.readOnly = false
            email_text.readOnly = false
            gender_text.visible = false
            gender_combo.visible = true
            post_text.visible = false
            post_combo.visible = true
            admin_label.visible = true
            admin_combo.visible = true
        }
    }

    Button
    {
        id:btn_commit
        text: "Commit"
        anchors{left:btn_edit.right; leftMargin:60; top:mobile_text.bottom; topMargin: 50}
        onClicked:{
            clearBasic()
            checkEditEnable()
            editFlag = 0
        }
    }

    function checkEditEnable()
    {
        if(!editFlag)
        {
            messageDialog.text = "Edit doctor basic information first."
            messageDialog.visible = true
        }
        else
            commitClicked()
    }

    function closeMsgBox()
    {
        messageDialog.close()
    }

    function clearBasic()
    {
        firstName_text.readOnly = true
        lastName_text.readOnly = true
        age_text.readOnly = true
        mobile_text.readOnly = true
        email_text.readOnly = true

        gender_text.visible = true
        gender_combo.visible = false

        post_text.visible = true
        post_combo.visible = false

        admin_label.visible = false
        admin_combo.visible = false
    }
}
