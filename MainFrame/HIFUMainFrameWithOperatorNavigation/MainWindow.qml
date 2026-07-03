import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

Item
{
    id: main_gui
    width: Screen.width
    // height: Screen.height
    height: Screen.height
    visible: true
    SystemPalette { id: activePalette }

    signal getSearchTextChanged_Pt(int index, string searchContent)
    signal getDelete_Pt(int rowIndex)
    signal getAdd_Pt()
    signal showPatientDetailTab(int rowIndex)
    signal getSearchTextChanged_Dr(int index, string searchContent)
    signal getDelete_Dr(int rowIndex)
    signal getAdd_Dr()
    signal showDoctorDetailTab(int rowIndex)

    Rectangle{
        id:main_window_back_ground
        width :parent.width
        height:parent.height
        anchors.fill: parent
        color:Qt.rgba(76/255,79/255,81/255,1.0)
    }

    //title
    Item
    {
        id: title_bar
        width: main_gui.width
        height: main_gui.height * 0.1
        anchors.top: main_gui.top

        Text
        {
            id: title
            anchors.centerIn: parent
            text: "Information Management System"
            font.family: "微软雅黑"
            font.bold: true
            font.pointSize: main_gui.height * 0.05 * 0.5
            color: "dark red"
        }
    }

    //Tabs
    TabView
    {
        id: mainTabView
        width:main_gui.width
        height:main_gui.height*0.9
        anchors.top: title_bar.bottom
        anchors.margins: 3

        Rectangle{
            id:patient_tab_back_ground
            width :parent.width
            height:parent.height
            anchors.fill: parent
            color:Qt.rgba(76/255,79/255,81/255,1.0)
        }

        Tab
        {
            id:patient_tab
            title: "Patients"
            PatientTab {
                id:component_PT

                anchors.fill: parent
                onTextChanged : getSignalSearch_Pt(currentIndex, searchContent)
                onIndexChanged: getSignalSearch_Pt(currentIndex, searchContent)
                onRowSelected : getSignalDelete_Pt(rowIndex)
                onDoubleClicked: showPatientDetailTab(rowIndex)
                onAddRow:getSignalAdd_Pt()}
        }

        Tab
        {
            id:doctor_tab
            title: "Physicians"
            DoctorTab {
                anchors.fill: parent
                onTextChanged : getSignalSearch_Dr(currentIndex, searchContent)
                onIndexChanged: getSignalSearch_Dr(currentIndex, searchContent)
                onRowSelected : getSignalDelete_Dr(rowIndex)
                onDoubleClicked: showDoctorDetailTab(rowIndex)
                onAddRow:getSignalAdd_Dr()}
        }
    }

    function getSignalSearch_Pt(currentIndex,searchContent)
    {
        getSearchTextChanged_Pt(currentIndex, searchContent)
    }

    function getSignalDelete_Pt(rowIndex)
    {
        getDelete_Pt(rowIndex)
    }

    function getSignalAdd_Pt()
    {
        getAdd_Pt()
    }

    function getSignalSearch_Dr(currentIndex,searchContent)
    {
        getSearchTextChanged_Dr(currentIndex, searchContent)
    }

    function getSignalDelete_Dr(rowIndex)
    {
        getDelete_Dr(rowIndex)
    }

    function getSignalAdd_Dr()
    {
        getAdd_Dr()
    }
}

