import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2

ColumnLayout {
    id:root
    anchors.margins: 3
    spacing: 5

    property alias dateText : date_text.text
    property alias startTimeText : startTime_text.text
    property alias finishTimeText : finishTime_text.text
    property alias currFocusCountText : currFocusCount_text.text
    property alias currDutyOnText : currDutyOn_text.text
    property alias currDutyOffText : currDutyOff_text.text
    property alias currCoolingText : currCooling_text.text
    property alias isCompleteText : isComplete_text.text

    signal start()

    //Table
    Item
    {
        id:tableview_item
        width: root.width
        height: root.height * 0.5

        TableView
        {
            id:table_view
            anchors{left:parent.left;right:parent.right; top:parent.top; topMargin: 5; bottom:parent.bottom; bottomMargin: 40}
            model:TPModel

            //Columns
            TableViewColumn{role: "id"; title: "No"; width:50}
            TableViewColumn{role: "focus_x"; title: "X"; width:50}
            TableViewColumn{role: "focus_y"; title: "Y"; width:50}
            TableViewColumn{role: "focus_y"; title: "Z"; width:50}
            TableViewColumn{role: "voltage"; title: "Voltage(V)"; width:80}
            TableViewColumn{role: "sonicationTime"; title: "Total Time(s)"; width:100}
            TableViewColumn{role: "sonicationPeriod"; title: "Period(ms)"; width:100}
            TableViewColumn{role: "dutyCycle"; title: "Cycle(%)"; width:90}
            TableViewColumn{role: "coolingTime"; title: "Cooling Time(s)"; width:100}
        }

        Text{
            id:title_label
            text:"Treatment Session:"
            anchors{left:parent.left;leftMargin: 5;top:table_view.bottom;topMargin: 15}
        }
    }

    Text{
        id:date_label
        text:"Date:"
        anchors{left:parent.left;leftMargin: 5;top:tableview_item.bottom ;topMargin: 3}
    }

    TextField {
        id:date_text
        anchors{left:date_label.right;top:tableview_item.bottom}
        readOnly: true
        }

    Text{
        id:startTime_label
        text:"Start Time:"
        anchors{left:date_text.right;leftMargin: 50;top:tableview_item.bottom;topMargin: 3}
    }

    TextField {
        id:startTime_text
        anchors{left:startTime_label.right;top:tableview_item.bottom}
        readOnly: true
        }

    Text{
        id:finishTime_label
        text:"Finish Time:"
        anchors{left:startTime_text.right;leftMargin: 50;top:tableview_item.bottom;topMargin: 3}
    }

    TextField {
        id:finishTime_text
        anchors{left:finishTime_label.right;top:tableview_item.bottom}
        readOnly: true
        }

    Text{
        id:currFocusCount_label
        text:"Current Focus #:"
        anchors{left:parent.left;leftMargin: 5;top:date_label.bottom;topMargin: 25}
    }

    TextField {
        id:currFocusCount_text
        anchors{left:currFocusCount_label.right;top:date_label.bottom;topMargin: 22}
        readOnly: true
        }

    Text{
        id:isComplete_label
        text:"Completion:"
        anchors{left:currFocusCount_text.right;leftMargin: 50;top:date_label.bottom;topMargin: 25}
    }

    TextField {
        id:isComplete_text
        anchors{left:isComplete_label.right;top:date_label.bottom;topMargin: 22}
        readOnly: true
        }

    Text{
        id:currDutyOn_label
        text:"Current DutyOn #:"
        anchors{left:parent.left;leftMargin: 5;top:currFocusCount_label.bottom;topMargin: 25}
    }

    TextField {
        id:currDutyOn_text
        anchors{left:currDutyOn_label.right;top:currFocusCount_label.bottom;topMargin: 22}
        readOnly: true
        }

    Text{
        id:currDutyOff_label
        text:"Current DutyOff #:"
        anchors{left:currDutyOn_text.right;top:currFocusCount_label.bottom;topMargin: 25}
    }

    TextField {
        id:currDutyOff_text
        anchors{left:currDutyOff_label.right;top:currFocusCount_label.bottom;topMargin: 22}
        readOnly: true
        }

    Text{
        id:currCooling_label
        text:"Current Cooling #:"
        anchors{left:currDutyOff_text.right;top:currFocusCount_label.bottom;topMargin: 25}
    }

    TextField {
        id:currCooling_text
        anchors{left:currCooling_label.right;top:currFocusCount_label.bottom;topMargin: 22}
        readOnly: true
        }

    //Button
    Button
    {
        id:btn_start
        text: "Start Treatment"
        anchors{left:parent.left; leftMargin:400; top:currDutyOn_label.bottom; topMargin: 35}
        onClicked:showMainWindow()
    }

    function showMainWindow()
    {
        w.show()
        start()
    }
}
