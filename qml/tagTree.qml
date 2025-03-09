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
    width: screenGlobal.getScreenWidth()//480
    height: screenGlobal.getScreenHeight()//800

    property int treeViewHeight : screenGlobal.adaptYSize(300)
    property int treeViewYOffset : screenGlobal.adaptYSize(50)
    property int columntWidth : screenGlobal.adaptXSize(300)
    property int aimListYOffset : screenGlobal.adaptYSize(400)
    property int aimListHeight : screenGlobal.adaptXSize(300)

    signal requestOpenAddAim()
    signal requestOpenEditAim(var aimId)
    signal requestOpenSingleAim(var aimId)

    Component.onCompleted:  {
        localBase.fillTreeModelWithTags()
    }

    QMLOld.TreeView
    {
        id: treeView
        model: tagTree

        width: parent.width
        height: treeViewHeight
        x: 0
        y: treeViewYOffset

        itemDelegate:  Item {
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
                    tagTree.setItemText(tagTree.currentIndex,editingField.text, tagTree.getColumnIndex(styleData.role))
                }
            }
        }

        QMLOld.TableViewColumn {
            role: tagTree.getColumnName(0)
            title: "tag"
            width: columntWidth
        }

        onClicked: {
            aimList.searchModelTag(tagTree.getFieldByIndex(index,0))
        }
    }

    AimList {
        y: aimListYOffset
        height: aimListHeight
        width: parent.width
        id: aimList
    }
}
