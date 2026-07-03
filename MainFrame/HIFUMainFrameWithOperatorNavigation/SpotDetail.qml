import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

Item
{
    id: patientDetail_gui
    width: 700
    height: 384
    visible: true
    SystemPalette { id: activePalette }

    property string id_pt : ""
    property string fname_pt : ""
    property string lname_pt : ""
    property string age_pt : ""
    property string mobile_pt : ""
    property string x_pt : ""
    property string y_pt : ""
    property string z_pt : ""

    property string date_pt : ""
    property string startT_pt : ""
    property string finishT_pt : ""
    property string currFocusNo_pt : ""
    property string currDutyOn_pt : ""
    property string currDutyOff_pt : ""
    property string currCooling_pt : ""
    property string isComplete_pt : ""

    signal startTreatment()
    signal changeCommit(string id,string fname,string lname,string age,string mobile,string x,string y,string z)

    //title
    Item
    {
        id: title_bar
        width: patientDetail_gui.width
        height: patientDetail_gui.height * 0.08
        anchors.top: patientDetail_gui.top

        Text
        {
            id: title
            anchors.centerIn: parent
            text: "Patient Details"
            font.bold: true
            // font.pointSize: patientDetail_gui.height * 0.03 * 0.3
            color: "dark red "
        }
    }

    //Tabs
    TabView
    {
        id: patientTabView
        width:patientDetail_gui.width
        height:patientDetail_gui.height*0.9
        anchors.top: title_bar.bottom
        anchors.margins: 3

        Tab
        {
            id:patientBasicInfo_tab
            title: "Basic Information"
            PatientBasicInfo {
                id:basic_tab
                anchors.fill: parent
                idText: id_pt
                firstNameText: fname_pt
                lastNameText: lname_pt
                ageText: age_pt
                mobileText: mobile_pt
                sizeXText: x_pt
                sizeYText: y_pt
                sizeZText: z_pt
                onStart: startTreatment()
                onCommitClicked: changeCommit(idText,firstNameText,lastNameText,ageText,mobileText,sizeXText,sizeYText,sizeZText)
            }
        }
    }

    function showBasicInfo(id,fname,lname,age,mobile,x,y,z)
    {
        id_pt = id
        fname_pt = fname
        lname_pt = lname
        age_pt = age
        mobile_pt = mobile
        x_pt = x
        y_pt = y
        z_pt = z
    }

    function showTreatmentSession(date,startT,finishT,currFocus,currDutyOn,currDutyOff,currCooling,isComplete)
    {
        date_pt = date
        startT_pt = startT
        finishT_pt = finishT
        currFocusNo_pt = currFocus
        currDutyOn_pt = currDutyOn
        currDutyOff_pt = currDutyOff
        currCooling_pt = currCooling
        isComplete_pt = isComplete
    }

    function clearTreatmentSession()
    {
        date_pt = ""
        startT_pt = ""
        finishT_pt = ""
        currFocusNo_pt = ""
        currDutyOn_pt = ""
        currDutyOff_pt = ""
        currCooling_pt = ""
        isComplete_pt = ""
    }
}

