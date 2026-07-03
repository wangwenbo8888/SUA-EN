import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.0

ApplicationWindow {
    id: login_gui
    flags:Qt.FramelessWindowHint | Qt.Window
    width: Screen.width
    height: Screen.height
    visible: true
//    flags: Qt.Sheet
    SystemPalette { id: activePalette }

    property int colorFlag : 0
    property alias titleFillColor : top_bar.color
    property alias loginFillColor : rect1.color
    property alias userNameFillColor : username_text.color
    property alias userNameLabelColor : userNameLabel.color
    property alias userNameTextColor: username_text.textColor
    property alias pwFillColor : pw_text.color
    property alias pwLabelColor : pwLabel.color
    property alias pwTextColor : pw_text.textColor
    property alias bottomFillColor : bottom_bar.color
    property alias backgroundColor : login_gui.color

    signal btnLoginClicked(string name,string pw)
    signal shutLoginQml()
    signal authorize()

    Rectangle{
        id:back_ground
        width :parent.width
        height:parent.height
        anchors.fill: parent
        color:Qt.rgba(0,0,0,1.0)
    }

    //Title
    Rectangle{
        id: top_bar
        width: login_gui.width
        height: login_gui.height * 0.05
        anchors{top:login_gui.top}

        Text
        {
            id: title
            anchors{horizontalCenter: parent.horizontalCenter;verticalCenter: parent.verticalCenter}
            text: "USgHIFU System"
            font.bold: true
            font.family: "Times New Roman"
            font.pointSize: login_gui.height * 0.05 * 0.7
        }
    }

    // system viewer
//    Item
//    {
//        id: system_bar
//        width: login_gui.width*0.6
//        height: login_gui.width*0.6*0.66
//        anchors{left:parent.left;leftMargin:login_gui.width*0.02;top:top_bar.bottom;topMargin: login_gui.height*0.02}

//        Image
//        {
//            id: picture
//            source: "systempicture.png"
//            anchors.fill: parent
//            fillMode: Image.Stretch
//        }
//    }

    //ProgressBar
    ProgressBar {
        id:progressBar
        width: 400
        height: 500 * 0.05
        anchors{left:parent.left;leftMargin:login_gui.width*0.55;top:top_bar.bottom;topMargin:0.2*login_gui.height}
        indeterminate: true
        }

    //Logo
    Item
    {
        id: logo_bar
        width: 400
        height: 500 * 0.3
        anchors{left:parent.left;leftMargin:login_gui.width*0.55;top: progressBar.bottom}

        Image
        {
            id: logo
            source: "logo.jpg"
            anchors.fill: parent
            fillMode: Image.Stretch
        }
    }

    //Login
    Rectangle
    {
        id: rect1
        width: 400
        height: 500 * 0.54
        anchors {left: parent.left;leftMargin: login_gui.width*0.55;top: logo_bar.bottom; }
        //border.color: "#707070"

//        Image
//        {
//            id: back_picture
//            source: "systempicture.png"
//            anchors.fill: parent
//            fillMode: Image.Stretch
//        }

        //username
        Item
        {
            id: static_text1
            width: rect1.width * 0.9
            height: rect1.height / 6
            anchors { left: rect1.left; leftMargin: rect1.width*0.1; top: rect1.top; horizontalCenter: parent.horizontalCenter}

            Text
            {
                id:userNameLabel
                anchors {bottom:parent.bottom}
                text: "UserName "
                font.bold: true
                font.family: "Times New Roman"
                font.pointSize: 500 * 0.04 * 0.7
                //color: "black"
            }
        }

        LineInput
        {
            id: username_text
            width: rect1.width * 0.7
            height: rect1.height / 6
            anchors { left: rect1.left; leftMargin: rect1.width*0.1; top: static_text1.bottom; horizontalCenter: parent.horizontalCenter}
//            hint: "Please enter your user name"
            text:"jx"
            focus: true
            KeyNavigation.tab:pw_text
        }

        //password
        Item
        {
            id: static_text2
            width: rect1.width * 0.9
            height: rect1.height * 0.15
            anchors { left: rect1.left; leftMargin: rect1.width*0.1; top: username_text.bottom; horizontalCenter: parent.horizontalCenter}

            Text
            {
                id:pwLabel
                anchors {bottom:parent.bottom}
                text: "Password"
                font.bold: true
                font.family: "Times New Roman"
                font.pointSize: 500 * 0.04 * 0.7
                //color: "black"
            }
        }

        LineInput
        {
            id: pw_text
            width: rect1.width * 0.7
            height: rect1.height * 0.15
            anchors { left: rect1.left; leftMargin: rect1.width*0.1; top: static_text2.bottom; horizontalCenter: parent.horizontalCenter}
            hint: "Please enter your password"
            text:"12345"
            echoMode: TextInput.Password
            onAccepted: login_req()
        }

        //Buttons
        MyButton
        {
            id: btn_login
            width: rect1.width * 0.25
            height: rect1.height * 0.15
            anchors { left: rect1.left; leftMargin: rect1.width*0.2; bottom: rect1.bottom; bottomMargin: rect1.height*0.1}
            text: "Sign in"
            onClicked: login_req()
        }

        MyButton
        {
            id: btn_signout
            width: rect1.width * 0.25
            height: rect1.height * 0.15
            anchors { left: btn_login.right; leftMargin: rect1.width*0.1; bottom: rect1.bottom; bottomMargin: rect1.height*0.1}
            text: "Quit"
            onClicked: Qt.quit()
        }
    }

    //Title
    Rectangle{
        id: bottom_bar
        width: login_gui.width
        height: login_gui.height * 0.06
        anchors{top:rect1.bottom;topMargin: login_gui.height*0.15}

        Text
        {
            id: company_info
            anchors{horizontalCenter: parent.horizontalCenter;verticalCenter: parent.verticalCenter}
            text: "Zhonghui Medical Technology (Shanghai) Co., Ltd."
            font.bold: true
            font.family: "Times New Roman"
            font.pointSize: login_gui.height * 0.05 * 0.7
        }
    }

    statusBar: RowLayout{
        id:status_bar
        width: 400

        Text{
            id:status_left
            anchors{left:parent.left;verticalCenter: status_bar.verticalCenter}
        }
        Text{
            id:status_right
            anchors{left:status_left.right;leftMargin: 400 * 0.5; verticalCenter: status_bar.verticalCenter}
        }
    }

    function login_req()
    {
        if (username_text.text == "")
        {
            status_right.text = "User name cannot be vacant!"
            return
        }

        if (pw_text.text == "")
        {
            status_right.text = "Password cannot be vacant!"
            return
        }

        btnLoginClicked(username_text.text,pw_text.text)
        return
    }

    function generateColor()
    {
        colorFlag = Math.round(Math.random())
        console.log(colorFlag)

        if(0)
        {
            titleFillColor = "#CDCDF5"
            loginFillColor = "white"
            userNameFillColor = "white"
            pwFillColor = "white"
//            btn_login.fillcolor = ""
//            btn_login.textColor = ""
//            btn_signout.fillColor = ""
//            btn_signout.textColor = ""
        }
        else
        {
            backgroundColor = "#FCFCFC"
            // titleFillColor = "#CDCDF5"
            titleFillColor = Qt.rgba(250/255,1.0,240/255,1.0)
            // loginFillColor = "white"
            loginFillColor = Qt.rgba(76/255,79/255,81/255,1.0)
            userNameFillColor = Qt.rgba(0,0,0,0)
            userNameLabelColor = "white"
            userNameTextColor = "black"
            pwFillColor = Qt.rgba(0,0,0,0)
            pwLabelColor = "white"
            pwTextColor = "black"
            btn_login.fillColor = Qt.rgba(0,0,0,0)
            btn_login.textColor = "black"
            btn_signout.fillColor = Qt.rgba(0,0,0,0)
            btn_signout.textColor = "black"
            bottomFillColor = Qt.rgba(250/255,1.0,240/255,1.0);
        }
    }

    function showResultMsg(success,isAdmin)
    {
        if (success)
        {
            status_right.text = "Successful login."
            shutLoginQml()
            login_gui.close()
        }

        if (!success)
        {
            status_right.text = "Login failed."
        }

        if(!isAdmin)
            authorize()

        return
    }
}
