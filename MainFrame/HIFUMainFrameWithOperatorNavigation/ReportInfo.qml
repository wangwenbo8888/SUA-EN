import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2

ColumnLayout {
    id:roportInfo_gui
    width: Screen.width
    height: Screen.height
    anchors.margins: 3
    spacing: 5

    property alias idText         : id_text.text
    property alias firstNameText  : firstName_text.text
    property alias ageText        : age_text.text
    property alias mobileText     : mobile_text.text
    property alias sizeXText      : sizeX_text.text
    property alias sizeYText      : sizeY_text.text
    property alias sizeZText      : sizeZ_text.text
	
    property alias meltRateText   : melt_text.text
    property alias startTimeText  : startTime_text.text
    property alias finishTimeText : finishTime_text.text

    property alias conclusionText : conclusion_text.text

    property alias irradiateText  : irradiateSpotNumber_text.text
    property alias voltageText    : irradiateVoltage_text.text
    property alias deepText       : deep_text.text
    property alias doctorText     : doctor_text.text
    property alias pulseText      : pulseNumber_text.text

	property int editFlag : 0

    signal start()
    signal loginChangeCommit()
    signal showRegistWindow()
    signal printf(string id,       string name,   string age,    string mobile,
                  string x,        string y,      string z,      string rate,
                  string startTime,string endTime,string con,    string number,
                  string voltage,  string deep,   string doctor, string pulse)

    Rectangle{
        id:report_back_ground
        width :parent.width
        height:parent.height
        anchors.fill: parent
        color:Qt.rgba(76/255,79/255,81/255,1.0)
    }

    Item
    {
        id: report_title
        width: roportInfo_gui.width
        height: roportInfo_gui.height * 0.08
        anchors{top:parent.top;topMargin: 10}

        Text
        {
            id: therpy_title
            y: 0
            width: 372
            height: 47
            anchors{left:parent.left;leftMargin: 774}
            text: "Zhonghui Medical HIFU Treatment Report"
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "微软雅黑"
            font.bold: true
            font.pointSize: roportInfo_gui.height*0.05*0.5
            color: "dark blue"
        }
    }

	    //Information
    Text{
        id:id_label
        color: "#ffffff"
        text:"Patient ID:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:parent.left;leftMargin: 30;top:report_title.bottom;topMargin: 30}
    }

    TextField {
        id:id_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:id_label.right;top:report_title.bottom;topMargin: 30}
        readOnly: true
        }

    Text{
        id:firstName_label
        color: "#ffffff"
        text:"Name:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:parent.left; leftMargin: 30; top:id_label.bottom;topMargin: 20}
    }

    TextField {
        id:firstName_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:id_label.right;top:id_label.bottom;topMargin: 20}
        readOnly: true
        }

    Text{
        id:sex_label
        color: "#ffffff"
        text:"Sex:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:firstName_text.right; leftMargin: 80; top:id_label.bottom;topMargin: 20}
    }

    TextField {
        id:sex_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:sex_label.right;leftMargin: 48;top:id_label.bottom;topMargin: 20}
        readOnly: true
        }

    Text{
        id:age_label
        color: "#ffffff"
        text:"Age:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:parent.left;leftMargin: 30;top:firstName_label.bottom;topMargin: 20}
    }

    TextField {
        id:age_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:id_label.right;top:firstName_label.bottom;topMargin: 20}
        readOnly: true
        }

    Text{
        id:ageUnit_label
        color: "#ffffff"
        text:" years"
        font.family: "微软雅黑 "
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:age_text.right;top:firstName_label.bottom;topMargin: 20}
    }

    Text{
        id:mobile_label
        color: "#ffffff"
        text:"Phone:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:age_text.right;leftMargin: 80;top:firstName_label.bottom;topMargin: 20}
    }

    TextField {
        id:mobile_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:mobile_label.right;leftMargin: 18;top:firstName_label.bottom;topMargin: 20}
        readOnly: true
        }

    Text{
        id:size_label
        color: "#ffffff"
        text:"Fibroid Size:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:parent.left;leftMargin: 30;top:age_text.bottom;topMargin: 20}
    }

    TextField {
        id:sizeX_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:id_label.right;top:age_text.bottom;topMargin: 20}
        readOnly: true
        }

    Text{
        id:symbol1_label
        color: "#ffffff"
        text:" * "
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:sizeX_text.right;top:age_text.bottom;topMargin: 20}
    }

    TextField {
        id:sizeY_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:symbol1_label.right;top:age_text.bottom;topMargin: 20}
        readOnly: true
        }

    Text{
        id:symbol2_label
        color: "#ffffff"
        text:" * "
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:sizeY_text.right;top:age_text.bottom;topMargin: 20}
    }

    TextField {
        id:sizeZ_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:symbol2_label.right;top:age_text.bottom;topMargin: 20}
        readOnly: true
        }

    Text{
        id:unit_label
        color: "#ffffff"
        text:" mm^3 "
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:sizeZ_text.right;top:age_text.bottom;topMargin: 20}
    }

    Text{
        id:melt_label
        color: "#ffffff"
        text:"Ablation Ratio:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:parent.left;leftMargin:30;top:size_label.bottom ;topMargin: 20}
    }

    TextField {
        id:melt_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:melt_label.right;leftMargin:10;top:size_label.bottom ;topMargin: 20}
        readOnly: true
        }

    Text{
        id:startTime_label
        color: "#ffffff"
        text:"Start Time:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:parent.left;leftMargin: 30;top:melt_label.bottom;topMargin: 20}
    }

    TextField {
        id:startTime_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:startTime_label.right;leftMargin: 10;top:melt_label.bottom;topMargin: 20}
        readOnly: true
        }

    Text{
        id:finishTime_label
        color: "#ffffff"
        text:"End Time:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:startTime_text.right;leftMargin: 80;top:melt_label.bottom;topMargin:20}
    }

    TextField {
        id:finishTime_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:finishTime_label.right;leftMargin: 18;top:melt_label.bottom;topMargin:20}
        readOnly: true
        }

    Text{
        id:conclusion_label
        color: "#ffffff"
        text:"Treatment Summary:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.04*0.4
        anchors{left:parent.left;leftMargin: 30;top:finishTime_label.bottom;topMargin: 20}
    }

   TextEdit {
            id:conclusion_text
            width: 600
            color: "#ffffff"
            height: parent.height*0.3
            focus: true
            wrapMode: TextEdit.Wrap
            font.family: "微软雅黑"
            anchors{left:parent.left;leftMargin: 30;top:conclusion_label.bottom;topMargin:20}
            }

    Text{
        id:parameter_label
        color: "#ffffff"
        text:"Treatment Parameters:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.04*0.4
        anchors{left:parent.left;leftMargin: 30;top:conclusion_text.bottom;topMargin: 20}
    }

    Text{
        id:irradiateSpotNumber_label
        color: "#ffffff"
        text:"Sonication Points:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:parent.left;leftMargin: 30;top:parameter_label.bottom;topMargin:30}
    }

    TextField {
        id:irradiateSpotNumber_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:irradiateSpotNumber_label.right;leftMargin: 10;top:parameter_label.bottom;topMargin:30}
        readOnly: true
        }

    Text{
        id:numberUnit_label
        color: "#ffffff"
        text:" pcs"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:irradiateSpotNumber_text.right;top:parameter_label.bottom;topMargin: 30}
    }

    Text{
        id:irradiateVoltage_label
        color: "#ffffff"
        text:"Sonication Voltage:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:irradiateSpotNumber_text.right;leftMargin: 80;top:parameter_label.bottom;topMargin:30}
    }

    TextField {
        id:irradiateVoltage_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:irradiateVoltage_label.right;leftMargin: 10;top:parameter_label.bottom;topMargin:30}
        readOnly: true
        }

    Text{
        id:voltageUnit_label
        color: "#ffffff"
        text:" volts"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:irradiateVoltage_text.right;top:parameter_label.bottom;topMargin: 30}
    }

    Text{
        id:deep_label
        color: "#ffffff"
        text:"Treatment Depth:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:parent.left;leftMargin: 30;top:irradiateSpotNumber_label.bottom;topMargin:20}
    }


    TextField {
        id:deep_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:deep_label.right;leftMargin: 10;top:irradiateSpotNumber_label.bottom;topMargin:20}
        readOnly: true
        }

    Text{
        id:deepUnit_label
        color: "#ffffff"
        text:" mm"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:deep_text.right;top:irradiateSpotNumber_label.bottom;topMargin: 20}
    }

    Text{
        id:pulseNumber_label
        color: "#ffffff"
        text:"Pulse:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:deep_text.right;leftMargin: 80;top:irradiateSpotNumber_label.bottom;topMargin:20}
    }

    TextField {
        id:pulseNumber_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:irradiateVoltage_label.right;leftMargin: 10;top:irradiateSpotNumber_label.bottom;topMargin:20}
        readOnly: true
        }

    Text{
        id:pulseUnit_label
        color: "#ffffff"
        text:" pcs"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:pulseNumber_text.right;top:irradiateSpotNumber_label.bottom;topMargin: 20}
    }

    Image
    {
        id: downImage
        width: 200
        height:200
        anchors{left:parent.left;leftMargin: 30; top: pulseNumber_label.bottom;topMargin: 20;}
        fillMode: Image.TileHorizontally
        source:"file:///E:/downimage.bmp"
    }

    Text{
        id:downImage_label
        color: "#ffffff"
        text:"Lower Treatment Plane Image"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:downImage.left;leftMargin: 50;top:downImage.bottom;topMargin:20}
    }

    Image
    {
        id: middleImage
        width: 200
        height:200
        anchors{left:downImage.right;leftMargin: 10; top: pulseNumber_label.bottom;topMargin: 20;}
        fillMode: Image.TileHorizontally
        source: "file:///E:/middleimage.bmp"
    }
    Text{
        id:middleImage_label
        color: "#ffffff"
        text:"Middle Treatment Plane Image"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:middleImage.left;leftMargin: 50;top:downImage.bottom;topMargin:20}
    }

    Image
    {
        id: upImage
        width: 200
        height:200
        anchors{left:middleImage.right;leftMargin: 10; top: pulseNumber_label.bottom;topMargin: 20;}
        fillMode: Image.TileHorizontally
        source: "file:///E:/upimage.bmp"
    }

    Text{
        id:upImage_label
        color: "#ffffff"
        text:"Upper Treatment Plane Image"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.03*0.3
        anchors{left:upImage.left;leftMargin: 50;top:downImage.bottom;topMargin:20}
    }

    Text{
        id:doctor_label
        color: "#ffffff"
        text:"Attending Physician:"
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.04*0.4
        anchors{left:parent.left;leftMargin: 30;top:pulseNumber_label.bottom;topMargin: 300}
    }

    TextField {
        id:doctor_text
        font.family: "微软雅黑"
        font.pointSize: roportInfo_gui.height*0.04*0.4
        anchors{left:doctor_label.right;leftMargin: 10;top:pulseNumber_label.bottom;topMargin:300}
        readOnly: true
        }

    //Button
    Button
    {
        id:btn_start
        width: 203
        height: 25
        text: "Print Report"
        antialiasing: false
        smooth: true
        z: 0
        clip: false
        scale: 1
        anchors{left:parent.left; leftMargin:200; top:parent.bottom; topMargin: -100}
        onClicked: printf(idText,        firstNameText,   ageText,        mobileText,
                          sizeXText,     sizeYText,       sizeZText,      meltRateText,
                          startTimeText, finishTimeText,  conclusionText, irradiateText,
                          voltageText,   deepText,        doctorText,     pulseText)
    }

    Button
    {
        id:btn_back_to_patient_regist
        text: "Back to Registration"
        anchors{left:parent.left; leftMargin:400; top:parent.bottom; topMargin: -100}
        onClicked:showRegistWindow()
    }

    function setPatientInfo(id,fname,age,mobile,x,y,z)
    {
        id_text.text = id;
        firstName_text.text = fname;
        sex_text.text = "Female"
        age_text.text = age;
        mobile_text.text = mobile;
        sizeX_text.text = x
        sizeY_text.text = y;
        sizeZ_text.text = z;
    }

    function setTerapyInfo(rate,startTime,endTime,number,voltage,deep,doctor,pulse)
    {
        melt_text.text = rate
        startTime_text.text = startTime
        finishTime_text.text = endTime

        irradiateSpotNumber_text.text  = number
        irradiateVoltage_text.text = voltage
        deep_text.text = deep
        doctor_text.text = doctor
        pulseNumber_text.text = pulse
    }
	
    function enableEdit()
    {
        firstName_text.readOnly = false
        // lastName_text.readOnly = false
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
            messageDialog.text = "Edit patient basic information first."
            messageDialog.visible = true
        }
        else
            commitClicked()
    }
}
