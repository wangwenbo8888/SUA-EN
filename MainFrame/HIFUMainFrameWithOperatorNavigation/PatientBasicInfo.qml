import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ColumnLayout {
    id:patientBasic_gui
    width: 700
    height: 384
    anchors.margins: 3
    spacing: 5

    property alias idText : id_text.text
    property alias firstNameText : firstName_text.text
    property alias lastNameText : lastName_text.text
    property alias ageText : age_text.text
    property alias mobileText : mobile_text.text
    property alias sizeXText : sizeX_text.text
    property alias sizeYText : sizeY_text.text
    property alias sizeZText : sizeZ_text.text
    property int editFlag : 0

    signal commitClicked()
    signal start()

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
        text:"Patient ID:"
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
        anchors{left:parent.left;leftMargin: 10;top:id_label.bottom;topMargin: 20}
    }

    TextField {
        id:firstName_text
        anchors{left:firstName_label.right;top:id_label.bottom;topMargin: 17}
        readOnly: true
        }

    Text{
        id:lastName_label
        text:"Last Name:"
        anchors{left:firstName_text.right;leftMargin: 80;top:id_label.bottom;topMargin: 20}
    }

    TextField {
        id:lastName_text
        anchors{left:lastName_label.right;top:id_label.bottom;topMargin: 17}
        readOnly: true
        }

    Text{
        id:age_label
        text:"Age:"
        anchors{left:parent.left;leftMargin: 10;top:firstName_label.bottom;topMargin: 20}
    }

    TextField {
        id:age_text
        anchors{left:age_label.right;leftMargin: 42;top:firstName_label.bottom;topMargin: 17}
        readOnly: true
        }

    Text{
        id:mobile_label
        text:"Mobile:"
        anchors{left:age_text.right;leftMargin: 80;top:firstName_label.bottom;topMargin: 20}
    }

    TextField {
        id:mobile_text
        anchors{left:mobile_label.right;leftMargin: 18;top:firstName_label.bottom;topMargin: 17}
        readOnly: true
        }

    Text{
        id:size_label
        text:"Tumor Size:"
        anchors{left:parent.left;leftMargin: 10;top:mobile_label.bottom;topMargin: 20}
    }

    TextField {
        id:sizeX_text
        anchors{left:size_label.right;top:mobile_label.bottom;topMargin: 17}
        readOnly: true
        }

    Text{
        id:symbol1_label
        text:" * "
        anchors{left:sizeX_text.right;top:mobile_label.bottom;topMargin: 20}
    }

    TextField {
        id:sizeY_text
        anchors{left:symbol1_label.right;top:mobile_label.bottom;topMargin: 17}
        readOnly: true
        }

    Text{
        id:symbol2_label
        text:" * "
        anchors{left:sizeY_text.right;top:mobile_label.bottom;topMargin: 20}
    }

    TextField {
        id:sizeZ_text
        anchors{left:symbol2_label.right;top:mobile_label.bottom;topMargin: 17}
        readOnly: true
        }

    Text{
        id:unit_label
        text:" mm^3 "
        anchors{left:sizeZ_text.right;top:mobile_label.bottom;topMargin: 20}
    }

    Button
    {
        id:btn_edit
        text: "Edit"
        anchors{left:parent.left; leftMargin:200; top:sizeX_text.bottom; topMargin: 50}
        onClicked:{
            enableEdit()
            editFlag = 1
        }
    }

    Button
    {
        id:btn_commit
        text: "Submit"
        anchors{left:btn_edit.right; leftMargin:60; top:sizeX_text.bottom; topMargin: 50}
        onClicked:{
            firstName_text.readOnly = true
            lastName_text.readOnly = true
            age_text.readOnly = true
            mobile_text.readOnly = true
            sizeX_text.readOnly = true
            sizeY_text.readOnly = true
            sizeZ_text.readOnly = true
            checkEditEnable()
            editFlag = 0
        }
    }

    Button
    {
        id:btn_start
        text: "Start treatment"
        anchors{left:btn_commit.right; leftMargin:60; top:sizeX_text.bottom; topMargin: 50}
        onClicked:showMainWindow()
    }

    function showMainWindow()
    {
        w.show()
        // 发送当前患者基本信息
        w.sendPatientInfo(idText,firstNameText,ageText,mobileText,sizeXText,sizeYText,sizeZText)
        start()
    }

    function enableEdit()
    {
        firstName_text.readOnly = false
        lastName_text.readOnly = false
        age_text.readOnly = false
        mobile_text.readOnly = false
        sizeX_text.readOnly = false
        sizeY_text.readOnly = false
        sizeZ_text.readOnly = false
    }

    function checkEditEnable()
    {
        if(!editFlag)
        {
            messageDialog.text = "Edit patient basic information first. "
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
