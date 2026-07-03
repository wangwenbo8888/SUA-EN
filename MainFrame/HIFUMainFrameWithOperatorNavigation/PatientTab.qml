import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2

ColumnLayout {
    id:root
    anchors.margins: 3
    spacing: 5

    property int currentIndex : 1
    property alias searchContent : search_text.text
    property alias rowIndex : table_view.currentRow
    signal textChanged
    signal indexChanged
    signal rowSelected
    signal doubleClicked
    signal addRow

    //Search
    Text{
        id:search_label
        font.family: "微软雅黑"
        anchors{left:parent.left; leftMargin: 40; top:parent.top; topMargin: 20}
        text:"Search: "
    }

    TextField {
        id: search_text
        anchors{left:search_label.right;leftMargin: 5;top:parent.top; topMargin: 15}
        placeholderText: "To be Searched..."
        focus:true
        onTextChanged: root.textChanged()
    }

    //Table
    TableView
    {
        id:table_view
        anchors{left:parent.left;right:parent.right; rightMargin: 150; top:search_text.bottom; topMargin: 5; bottom:parent.bottom; bottomMargin: 40}
        model:PatientModel

        //Columns
        TableViewColumn{role: "id"; title: "ID"; width:80;}
        TableViewColumn{role: "firstName"; title: "First Name"}
        TableViewColumn{role: "lastName"; title: "Last Name"}
        TableViewColumn{role: "age"; title: "Age"}
        TableViewColumn{role: "mobile"; title: "Mobile"}

        onDoubleClicked: root.doubleClicked()
    }

    //RadioButton
    GroupBox{
        title: "Search Criteria"
        anchors{left:table_view.right; leftMargin: 5; right:parent.right; rightMargin: 5; top:search_text.bottom; topMargin: 5}

        ColumnLayout{
            ExclusiveGroup{id:searchConditionGroup}
            RadioButton{
                text:"ID"
                checked:true
                exclusiveGroup: searchConditionGroup
                onClicked: {
                    currentIndex = 1
                    root.indexChanged()
                }
            }
            RadioButton{
                text:"First Name"
                exclusiveGroup: searchConditionGroup
                onClicked: {
                    currentIndex = 2
                    root.indexChanged()
                }
            }
            RadioButton{
                text:"Last Name"
                exclusiveGroup: searchConditionGroup
                onClicked: {
                    currentIndex = 3
                    root.indexChanged()
                }
            }
            RadioButton{
                text:"Age"
                exclusiveGroup: searchConditionGroup
                onClicked: {
                    currentIndex = 4
                    root.indexChanged()
                }
            }
            RadioButton{
                text:"Mobile"
                exclusiveGroup: searchConditionGroup
                onClicked: {
                    currentIndex = 5
                    root.indexChanged()
                }
            }
        }
    }


    //Buttons
    Button
    {
        id:btn_add
        text: "Add"
        anchors{left:parent.left; leftMargin:Screen.width*0.5; top:table_view.bottom; topMargin: 10}
        onClicked:root.addRow()
    }

    Button
    {
        text: "Delete"
        anchors{left:btn_add.right; leftMargin:40; top:table_view.bottom; topMargin: 10}
        onClicked: root.rowSelected()
    }

}

