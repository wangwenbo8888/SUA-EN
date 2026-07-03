import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2

ColumnLayout {
    id:root
    anchors.margins: 3
    spacing: 5

    signal doubleClicked
    signal start
    signal shapeStateChange
    signal scatterState
    signal resultConfirm
    signal continueNext

    //Table
    TableView
    {
        id:current_view
        anchors{left:parent.left;right:parent.right; top:parent.top; bottom:parent.bottom;bottomMargin: 800;}
        model:CurrentPlan

        //Columns
        TableViewColumn{role: "id"; title: "ID"; width:40;}
        TableViewColumn{role: "angle"; title: "Angle"; width:50;}
        TableViewColumn{role: "layer"; title: "Plane";width:50;}
        TableViewColumn{role: "power"; title: "Power";width:50;}
        TableViewColumn{role: "energy"; title: "Energy";width:50;}
        TableViewColumn{role: "status"; title: "Status";width:50;}


        onDoubleClicked:
        {
            console.log("double clicked !")
            root.doubleClicked()
        }
    }

    /*
    CheckBox
    {
        id:box_complete1
        text:"Complete"
        checkedState: state_1
        anchors{left:parent.left; leftMargin:current_view.width*0.8; top:current_view.top; topMargin: 21}
        onStateChanged : root.spot1Complete()
    }

    CheckBox
    {
        id:box_complete2
        text:"Complete"
        checkedState: state_2
        anchors{left:parent.left; leftMargin:current_view.width*0.8; top:current_view.top; topMargin: 37}
        onStateChanged : root.spot2Complete()
    }

    CheckBox
    {
        id:box_complete3
        text:"Complete"
        checkedState: state_3
        anchors{left:parent.left; leftMargin:current_view.width*0.8; top:current_view.top; topMargin: 53}
        onStateChanged : root.spot3Complete()
    }

    CheckBox
    {
        id:box_complete4
        text:"Complete"
        checkedState: state_4
        anchors{left:parent.left; leftMargin:current_view.width*0.8; top:current_view.top; topMargin: 69}
        onStateChanged : root.spot4Complete()
    }

    CheckBox
    {
        id:box_complete5
        text:"Complete"
        checkedState: state_5
        anchors{left:parent.left; leftMargin:current_view.width*0.8; top:current_view.top; topMargin: 85}
        onStateChanged : root.spot5Complete()
    }

    CheckBox
    {
        id:box_complete6
        text:"Complete"
        checkedState: state_6
        anchors{left:parent.left; leftMargin:current_view.width*0.8; top:current_view.top; topMargin: 101}
        onStateChanged : root.spot6Complete()
    }*/

    //Buttons
    Button
    {
        id:btn_start
        text: "Start treatment"
        anchors{left:parent.left; leftMargin:parent.width*0.3; top:current_view.bottom; topMargin: 20}
        onClicked:
        {
            console.log("start treatment !")
            root.start()
        }
    }

    CheckBox{
        id:box_scatter
        text:"Sonication Point Distribution"
        anchors{left:parent.left; leftMargin:parent.width*0.3; top:btn_start.bottom; topMargin: 20}
        onClicked :
        {
            console.log("current table scatter tick !")
            root.scatterState()
        }
    }

    CheckBox{
        id:box_result
        text:"Effect Confirmation"
        anchors{left:parent.left; leftMargin:parent.width*0.3; top:box_scatter.bottom; topMargin: 20}
        onClicked:
        {
            console.log("current table result tick !")
            root.resultConfirm()
        }
    }

    //Buttons
    Button
    {
        id:btn_continue
        text: "Continue"
        anchors{left:parent.left; leftMargin:parent.width*0.3; top:box_result.bottom; topMargin: 20}
        onClicked :
        {
            console.log("continue next treatment !");
            box_scatter.checkedState = Qt.Unchecked
            box_result.checkedState = Qt.Unchecked
            root.continueNext()
        }
    }

    /*
    function stateChanged(index)
    {
        switch (index)
        {
        case 1:
            state_1 = Qt.Checked;
        break;
        case 1:
            state_2 = Qt.Checked;
        break;
        case 1:
            state_3 = Qt.Checked;
        break;
        case 1:
            state_4 = Qt.Checked;
        break;
        case 1:
            state_5 = Qt.Checked;
        break;
        case 1:
            state_6 = Qt.Checked;
        break;

        default:
            break;

        }
    }
    */

}

