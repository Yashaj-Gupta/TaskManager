import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.15
import QtQuick.Extras 1.4 as QmlExtra
import QtQuick.Controls 1.4 as QMLOld
import QtQuick.Controls.Styles.Flat 1.0 as Flat


Item {
    id: structureItem

    signal requestOpenSingleAim(var aimId)

    width: screenGlobal.getScreenWidth()//480
    height: screenGlobal.getScreenHeight()//800

    Component.onCompleted:  {
        localBase.fillTreeModelWithAims()
    }

    property int microOffset : screenGlobal.adaptXSize(10)
    property int saveButtonOffset : screenGlobal.adaptYSize(650) //y
    property int treeOldButtonsOffset : screenGlobal.adaptYSize(700)
    property int treeViewHeight : screenGlobal.adaptYSize(600)
    property int yOffset : screenGlobal.adaptYSize(50)
    property int column1Width : screenGlobal.adaptXSize(150)
    property int column2Width : screenGlobal.adaptXSize(70)
    property int column3Width : screenGlobal.adaptXSize(70)
    property int column4Width : screenGlobal.adaptXSize(250)

    Button{
        y: saveButtonOffset
        x: 10//parent.width - microOffset - width
        text: "Save"
        onClicked:{
            localBase.editTreeAims(treeModel)
        }
        onDoubleClicked:{
            if (text == "Save"){
                text = "Keep save";
                highlighted = true
            }
            else{
                text = "Save";
                highlighted = false
            }
        }
        highlighted: false
    }

    Button{
        y: saveButtonOffset
        x: parent.width - microOffset - width //10
        text: "Open aim"
        onClicked:{
            //console.log("Aim " + )
             structureItem.requestOpenSingleAim(treeModel.getAimId(aimsTree.currentIndex))
        }
        highlighted: false
    }

    RowLayout {
        y: treeOldButtonsOffset
        x: microOffset
        Button{
            text: "New top aim"
            onClicked: treeModel.addTopItem(aimsTree.currentIndex)
            highlighted: true
        }
        Button{
            text: "New child aim"
            onClicked:  treeModel.addChildItem(aimsTree.currentIndex)
            highlighted: true
        }
        Button{
            text: "Delete aim"
            onClicked: treeModel.removeItem(aimsTree.currentIndex)
            highlighted: true
        }
    }

    QMLOld.TreeView {
        id: aimsTree
        model: treeModel //hierarchy

        Component.onCompleted: {
            aimsTree.expandAll()
        }

        function expandAll() {
            var none = true
            for(var i = 0; i < treeModel.rowCount(); ++i)
            {
                var index = treeModel.index(i,0)
                if(!aimsTree.isExpanded(index)) {
                    aimsTree.expand(index)
                    none = false
                }
            }
            return none
        }

        width: parent.width
        height: treeViewHeight

        x: 0
        y: yOffset

        itemDelegate:   Item {
            id: treeDelegate
            TextEdit {
                id: editingField
                anchors.fill: parent
                color: styleData.textColor
                text: styleData.value
            }
            Connections {
                target: editingField
                onEditingFinished: {
                    treeModel.setItemText(aimsTree.currentIndex,editingField.text, treeModel.getColumnIndex(styleData.role))
                }
            }
        }

        QMLOld.TableViewColumn {
            role: treeModel.getColumnName(0)
            title: "Aim name"
            width: column1Width
        }
        QMLOld.TableViewColumn {
            role: treeModel.getColumnName(1)
            title: "Time"
            width: column2Width
        }
        QMLOld.TableViewColumn {
            role: treeModel.getColumnName(2)
            title: "Date"
            width: column3Width
        }
        QMLOld.TableViewColumn {
            role: treeModel.getColumnName(3)
            title: "Comment" //will autocalculate
            width: column4Width
        }
    }
}
