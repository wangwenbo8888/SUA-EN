import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

Item
{
    id: parameter_gui
    width: 320
    height: Screen.height*0.98
    visible: true
    SystemPalette { id: activePalette }

    signal showPatientDetailTab(int rowIndex)
    signal startIrradiate()
    signal scatter()
    signal result()
    signal next()

    //Tabs
    TabView
    {
        id: paraTabView
        width:parameter_gui.width
        height:parameter_gui.height*0.9
        anchors{top: parent.top; bottom:parent.bottom;}
        anchors.margins: 3


        Tab
        {
            id:current_tab
            title: "Current Plan"
            CurrentTab {
                id:component_PT
                anchors.fill: parent
                // onDoubleClicked: showPatientDetailTab(rowIndex)
                onStart:
                {
                    console.log("para table start treatment !")
                    startIrradiate()
                }
                onScatterState:
                {
                    console.log("confirm scatter !")
                    scatter()
                }
                onResultConfirm:
                {
                    console.log("confirm result !")
                    result()
                }
                onContinueNext:
                {
                    console.log("tables continue next treatment !");
                    next()
                }
            }
        }

        Tab
        {
            id:total_tab
            title: "Overall Plan"
            TotalTab {
                anchors.fill: parent
            }
        }
    }

//    function getSignalSearch_Pt(currentIndex,searchContent)
//    {
//        getSearchTextChanged_Pt(currentIndex, searchContent)
//    }

//    function getSignalDelete_Pt(rowIndex)
//    {
//        getDelete_Pt(rowIndex)
//    }

//    function getSignalAdd_Pt()
//    {
//        getAdd_Pt()
//    }

//    function getSignalSearch_Dr(currentIndex,searchContent)
//    {
//        getSearchTextChanged_Dr(currentIndex, searchContent)
//    }

//    function getSignalDelete_Dr(rowIndex)
//    {
//        getDelete_Dr(rowIndex)
//    }

//    function getSignalAdd_Dr()
//    {
//        getAdd_Dr()
//    }

}
