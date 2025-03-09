import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.2
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1
import QtQuick.Controls.Material 2.1


ApplicationWindow {
    id: window
    title: qsTr("Aim")
    width: screenGlobal.getScreenWidth()
    height: screenGlobal.getScreenHeight()
    visible: true

    property int microYOffset : screenGlobal.adaptYSize(5)
    property int fontNormalSize : screenGlobal.adaptYSize(15)*1.5

    property bool desktopClient: Qt.platform.os != "android" && Qt.platform.os != "ios"

    Material.accent: userSettings.getColor("Accent");
    Material.primary: userSettings.getColor("Primary");
    Material.theme: Material.Dark

    Component.onCompleted: {
        prevPages.pageChanged("aimView.qml")
    }

    onClosing: {
        if (userSettings.isDebugBuild())
                Qt.quit()
            else
                window.hide();
    }

    ListModel{
        id: prevPages
        function pageChanged(qmlFile, params) {
            if (params !== undefined){
                prevPages.append({"pageFile":qmlFile,"params":params})
                mainLoader.setSource(qmlFile,params)
            }
            else{
                prevPages.append({"pageFile":qmlFile})
                mainLoader.setSource(qmlFile)
            }
        }
    }

   header:  ToolBar
   {
        Material.background: userSettings.getColor("Background");
        id: toolBar
        RowLayout {
            ToolButton {
                text: "<"
                font.pixelSize: fontNormalSize
                onClicked: {
                    if (prevPages.count > 1){
                         var qmlFile = prevPages.get(prevPages.count-2).pageFile
                         var params = prevPages.get(prevPages.count-2).params
                         if (params !== undefined)
                            mainLoader.setSource(qmlFile,params)
                         else
                            mainLoader.setSource(qmlFile)
                         prevPages.remove(prevPages.count-1)
                    }
                }
            }
            ToolButton {
                text: "view"
                onClicked: prevPages.pageChanged("aimView.qml")
                font.pixelSize: fontNormalSize
            }
            ToolButton {
                text: "tree"
                onClicked:  prevPages.pageChanged("aimTree.qml")
                font.pixelSize: fontNormalSize
            }
            ToolButton {
                text: "tags"
                onClicked:  prevPages.pageChanged("tagTree.qml")
                font.pixelSize: fontNormalSize
            }
            ToolButton {
                text: "run"
                onClicked:  prevPages.pageChanged("runningAim.qml")
                font.pixelSize: fontNormalSize
            }
            ToolButton {
                text: "Menu"
                onClicked: toolMenu.open()
                font.pixelSize: fontNormalSize
            }
            ToolButton {
                text: "add"
                onClicked:  drawerRight.open()
                font.pixelSize: fontNormalSize
            }
        }
    }

   Menu {
       id: toolMenu
       x: window.width - width - 10
       width: screenGlobal.adaptXSize(150)

       MenuItem {
           text: "Schedule"
           font.pixelSize: fontNormalSize
           onTriggered:  prevPages.pageChanged("schedule.qml")
       }
       MenuItem {
           text: "Done acts"
           font.pixelSize: fontNormalSize
           onTriggered:  prevPages.pageChanged("actionsDone.qml")
       }
       MenuItem {
           text: "Done chart"
           font.pixelSize: fontNormalSize
           onTriggered: prevPages.pageChanged("doneActsChart.qml")
       }
       MenuItem {
           text: "Console"
           font.pixelSize: fontNormalSize
           onTriggered: prevPages.pageChanged("consoleLog.qml")
       }
       MenuItem{
           text: "Import"
           font.pixelSize: fontNormalSize
           onTriggered: importFileDialog.open()
       }
   }

    FileDialog {
        id: importFileDialog
        title: "Select aim export file name"
        nameFilters: [ "Aim files (*.aim)", "All files (*)" ]
        selectMultiple: false
        selectExisting: true
        onAccepted: {
           var check = localBase.checkThereIsSameImportAim(importFileDialog.fileUrl)
           if (check !== 0) {
               console.log("Simmilar aim already exists: " + check)
           }
           else {
               localBase.importAim(importFileDialog.fileUrl)
               mainLoader.setSource("aimView.qml")
               mainLoader.item.loadModel()
           }
        }
    }

    Loader {
        y: toolBar.height + microYOffset
        anchors.centerIn: parent
        id: mainLoader
        focus: true
    }

    Connections{
        target: mainLoader.item
        onRequestOpenAddAim: {
            addLoader.item.loadForAddNew()
            drawerRight.open()
        }
        onRequestOpenEditAim: {
            addLoader.item.loadAimForEdit(aimId)
            drawerRight.open()
        }
        onRequestOpenSingleAim:{
            prevPages.pageChanged("singleAim.qml",{"aimId":aimId})
        }
    }

    MessageDialog {
        id: confirmExitDialog
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        width: 400
        title: "Accept quiting"
        text: "Do you really want to close app?\nNew notifications cannot be recevied while its turned off."
        onAccepted: {
           Qt.quit();
        }
        onRejected: {
            systemTray.showNotification(userCheck.getDomainName(),userCheck.getUserName(),5);
        }
    }

    Connections {
         target: systemTray
         onSignalShow: {
             window.hide();
             window.show();
         }
         onSignalAdd: {
             window.hide();
             window.show();
            drawerRight.open()
         }
         onSignalQuit: {

             if (userSettings.isDebugBuild() === false)
                confirmExitDialog.open()
             else
                Qt.quit()
         }
         onSignalIconActivated: {
              if(window.visibility === Window.Hidden)
                  window.show()
         }
     }

    Connections {
        target: popUpItem
        onRequestNotificationsViewFromCpp:{
         window.show()
         prevPages.pageChanged("runningAim.qml")
        }
     }


    Drawer {
        id: drawerRight
        width: 0.66 * window.width
        height: window.height
        edge: Qt.RightEdge
        Loader {
            y: 0
            anchors.centerIn: parent
            anchors.fill:  parent
            id: addLoader
            source: "aimAdd.qml"
        }
        Connections{
            target: addLoader.item
            onRequestOpenViewAims: {
                drawerRight.close()
                prevPages.pageChanged("aimView.qml")
                mainLoader.item.loadModel()
            }
        }
    }
}
