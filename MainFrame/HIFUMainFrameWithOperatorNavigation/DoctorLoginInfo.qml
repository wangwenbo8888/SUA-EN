import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ColumnLayout {
    id:doctorBasic_gui
    anchors.margins: 3
    spacing: 5

    property alias fullNameText : fullName_text.text
    property alias userNameText : userName_text.text
    property alias pwText : pw_text.text
    property int editFlag : 0

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

    //Informatino
    Text{
        id:fullName_label
        text:"Staff Name:"
        anchors{left:parent.left;leftMargin: 10;top:parent.top;topMargin: 25}
    }

    TextField {
        id:fullName_text
        anchors{left:fullName_label.right;top:parent.top;topMargin: 22}
        readOnly: true
        }

    Text{
        id:userName_label
        text:"Username:"
        anchors{left:parent.left;leftMargin: 10;top:fullName_label.bottom;topMargin: 25}
    }

    TextField {
        id:userName_text
        anchors{left:userName_label.right;leftMargin: 6;top:fullName_label.bottom;topMargin: 22}
        readOnly: true
        }

    Text{
        id:pw_label
        text:"Password:"
        anchors{left:parent.left;leftMargin: 10;top:userName_label.bottom;topMargin: 25}
    }

    TextField {
        id:pw_text
        anchors{left:pw_label.right;leftMargin: 11;top:userName_label.bottom;topMargin: 22}
        readOnly: true
        }

    //Buttons
    Button
    {
        id:btn_edit
        text: "Edit"
        anchors{left:parent.left; leftMargin:200; top:pw_label.bottom; topMargin: 50}
        onClicked:{
            userName_text.readOnly = false
            pw_text.readOnly = false
            editFlag = 1
        }
    }

    Button
    {
        id:btn_commit
        text: "Commit"
        anchors{left:btn_edit.right; leftMargin:60; top:pw_label.bottom; topMargin: 50}
        onClicked:{
            userName_text.readOnly = true
            pw_text.readOnly = true
            checkEditEnable()
            editFlag = 0}
    }

    function checkEditEnable()
    {
        if(!editFlag)
        {
            messageDialog.text = "Edit login information first."
            messageDialog.visible = true
        }
        else
            commitClicked()
    }

    function closeMsgBox()
    {
        messageDialog.close()
    }
}

