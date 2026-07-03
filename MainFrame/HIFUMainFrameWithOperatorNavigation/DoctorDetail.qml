import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

Item
{
    id: doctorDetail_gui
    width: 680
    height: 384
    visible: true
    SystemPalette { id: activePalette }

    property string id_Dr : ""
    property string fname_Dr : ""
    property string lname_Dr : ""
    property string gender_Dr : ""
    property string age_Dr : ""
    property string post_Dr : ""
    property string mobile_Dr : ""
    property string email_Dr : ""
    property string fullName_Dr : ""
    property string userName_Dr : ""
    property string pw_Dr : ""

    signal basicChangeCommit(string id,string fname,string lname,string gender,string age,string post,string isAdmin,string mobile,string email)
    signal loginChangeCommit(string fullName,string userName,string password);

    Rectangle{
        id:dotoctor_back_ground
        width :parent.width
        height:parent.height
        anchors.fill: parent
        color:Qt.rgba(76/255,79/255,81/255,1.0)
    }

    //title
    Item
    {
        id: title_bar
        width: doctorDetail_gui.width
        height: doctorDetail_gui.height * 0.08
        anchors.top: doctorDetail_gui.top

        Text
        {
            id: title
            anchors.centerIn: parent
            text: "Doctor Details"
            font.bold: true
            color: "dark red"
        }
    }

    //Tabs
    TabView
    {
        id: doctorTabView
        width:doctorDetail_gui.width
        height:doctorDetail_gui.height*0.9
        anchors.top: title_bar.bottom
        anchors.margins: 3

        Tab
        {
            id:doctorBasicInfo_tab
            title: "Basic"
            DoctorBasicInfo {
                id:basic_tab
                anchors.fill: parent
                idText: id_Dr
                firstNameText: fname_Dr
                lastNameText: lname_Dr
                genderText: gender_Dr
                ageText: age_Dr
                postText: post_Dr
                mobileText: mobile_Dr
                emailText: email_Dr
                onCommitClicked: basicChangeCommit(idText,firstNameText,lastNameText,genderComboText,ageText,postComboText,isAdminText,mobileText,emailText)
            }
        }

        Tab
        {
            id:doctorLoginInfo_tab
            title: "Login"
            DoctorLoginInfo {
                anchors.fill: parent
                fullNameText: fullName_Dr
                userNameText: userName_Dr
                pwText: pw_Dr
                onCommitClicked: loginChangeCommit(fullNameText,userNameText,pwText)
            }
        }
    }

    function showBasicInfo(id,fname,lname,gender,age,post,mobile,email)
    {
        id_Dr = id
        fname_Dr = fname
        lname_Dr = lname
        gender_Dr = gender
        age_Dr = age
        post_Dr = post
        mobile_Dr = mobile
        email_Dr = email
    }

    function showLoginInfo(fullName,userName,pw)
    {
        fullName_Dr = fullName
        userName_Dr = userName
        pw_Dr = pw
    }

    function authorizeResult()
    {
//        doctorLoginInfo_tab.visible = false
//        basic_tab.btnEditVisibility = false
//        basic_tab.btnCommitVisibility = false
    }
}
