import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
//import QtQuick.Extras 1.4
import QtQuick.Layouts 1.15
import QtQuick.Extras 1.4 as QmlExtra
import QtQuick.Controls 1.4 as QmlOld
import QtQuick.Controls.Styles.Flat 1.0 as Flat

Item {

    id: singleAimWindow
    width: screenGlobal.getScreenWidth()//480
    height: screenGlobal.getScreenHeight()//800


    signal requestOpenAddAim()
    signal requestOpenEditAim(var aimId)
    signal requestOpenSingleAim(var aimId)

    property int elementHeight: screenGlobal.adaptYSize(45) //apply to every one
    property int widthOffset: screenGlobal.adaptXSize(20) //70
    property int yOffset: screenGlobal.adaptYSize(50) //be aware of low screens try everywhere
    property int microOffset: screenGlobal.adaptXSize(10)
    property int bigFontSize : screenGlobal.adaptYSize(35)
    property int middleFontSize : screenGlobal.adaptYSize(25)
    property int smallFontSize : screenGlobal.adaptYSize(15)
    property string aimId : ""
    property bool helpMode: false

    function loadAim(){

        if (aimId.length > 0)  {
            var aimLine = localBase.getSingleAim(aimId)

            var parentAimId = aimLine[10]
            var parentName = ""
            var parentLine = localBase.getSingleAim(parentAimId)
            if (parentLine.length > 1)
                parentName = parentLine[1]

            var dateAndTime = aimLine[3]
            if (aimLine[2].length > 0)
                dateAndTime += "T" + aimLine[2].length
            if (aimLine[11].length > 0)
                dateAndTime += " : " + aimLine[11];

            aimName.text = "<style>a:link { color:#11BB33; }</style><big><b>" + aimLine[1] + "</b></big><small>" + "    : <a href=\"" + parentAimId + "\">" + parentName + "</a>  " + "</small>"; //IN FUTURE WE CAL ALSO CREATE LINKS FOR DATES
            aimMoment.text = dateAndTime
            if (aimLine[5].length > 1)
                aimMoment.text += "  tag: " +aimLine[5]

            childList.model = localBase.getChildAimsNames(aimId);
            childInfoText.text = "Has " + childList.model.length + " child";

            if (childList.model.length === 0)
                openChildButton.enabled = false;
            else
                openChildButton.enabled = true;

            aimSummory.text = localBase.getActivitySummary(aimId)

            progressSlider.value = aimLine[8]
            progressText.text = aimLine[9]

            if (runningAims.isRunning(aimId)){
                aimRunningState.text = runningAims.getSecondsPassed(aimId) + " seconds";
                stopStartButton.text = "Stop";
            }
            else{
                aimRunningState.text = "Aim is stopped"
                stopStartButton.text = "Start";
            }

            linksRepeater.loadLinks()
        }
    }

    Component.onCompleted:{
        loadAim()
    }


    Flickable {
        id: flickName
        x: screenGlobal.adaptXSize( 5 )
        y: screenGlobal.adaptYSize( 10 )
        width: parent.width - screenGlobal.adaptYSize( 10 )
        height: elementHeight*2
        contentWidth: parent.width * 5
        contentHeight: elementHeight*2
        ScrollBar.horizontal: ScrollBar { active: true }
        TextArea{
            textFormat: "RichText"
            id: aimName
            width: parent.width * 5
            height: elementHeight*2
            readOnly:  true
            font.pixelSize: bigFontSize
            onLinkActivated:{
                singleAimWindow.requestOpenSingleAim(link)
            }
        }
        Text {
                id: aimMoment
                text: "moment" //better with period
                font.pixelSize: middleFontSize
                y: aimName.height
            }
    }

    Rectangle {
         y: flickName.y + elementHeight*3
         x: 0
         width: parent.width
         height: screenGlobal.adaptYSize(10)

         border.color: "black"
         color: "gray"
         id:firstBorder
    }

    RowLayout{
        y: flickName.y + elementHeight*3 +screenGlobal.adaptYSize(20)
        x: screenGlobal.adaptXSize( 10 )
        Text {
            id: childInfoText
            text : "Child list"
        }
        ComboBox{
            id: childList
            font.pixelSize: smallFontSize
            Layout.preferredWidth: parent.width/2 - screenGlobal.adaptXSize( 45 )

        }
        Button{
            id:openChildButton
            text: "Open child"
            font.pixelSize: smallFontSize
            onClicked:{
                var childIndex = childList.currentIndex
                var childLines = localBase.getChildAims(aimId)
                var requestedLine = childLines[childIndex]
                singleAimWindow.requestOpenSingleAim(requestedLine[0])
            }
        }
        Button{
            id:newChildButton
            text:"New child"
            font.pixelSize: smallFontSize
            onClicked:{
                singleAimWindow.requestOpenAddAim()
            }
        }
    }

    Rectangle {
         y: flickName.y + elementHeight*4 +screenGlobal.adaptYSize(30)
         x: 0
         width: parent.width
         height: screenGlobal.adaptYSize(10)

         border.color: "black"
         color: "gray"
         id: borderBeforeProgress
    }


    ScrollView {
         id: progressView
         y: borderBeforeProgress.y + borderBeforeProgress.height*2
         width: parent.width // /2
         height: elementHeight*3
         x: screenGlobal.adaptXSize(10)
        TextArea{
            id: progressText
            placeholderText: "Progress description"
            font.pixelSize: smallFontSize
        }
    }
    Slider{
        id: progressSlider
        y: progressView.y + progressView.height + screenGlobal.adaptYSize(10)
        width: parent.width - saveProgressButton.width  - screenGlobal.adaptXSize(20)
        from: 1
        to: 100
    }
    Button{
        id: saveProgressButton
        y: progressView.y + progressView.height + screenGlobal.adaptYSize(10)
        x: parent.width - width - screenGlobal.adaptXSize(10)
        text : "Save progress"
        font.pixelSize: smallFontSize
        onClicked: {
            localBase.updateAimProgress(aimId,progressSlider.value,progressText.text)
        }
    }

    Button {
        y: screenGlobal.adaptYSize(10) + elementHeight*2
        x: parent.width - width - screenGlobal.adaptXSize(10)
        font.pixelSize: smallFontSize
        text : "Edit"
        onClicked: {
            singleAimWindow.requestOpenEditAim(aimId)
        }
    }

    Rectangle {
         y: saveProgressButton.y + saveProgressButton.height + screenGlobal.adaptYSize(10)
         x: 0
         width: parent.width
         height: screenGlobal.adaptYSize(10)
         border.color: "black"
         color: "gray"
         id: borderBeforeSummary
    }

    Text {
        id: aimSummory
        font.pixelSize: smallFontSize
        y: borderBeforeSummary.y + borderBeforeSummary.height*2
        width: 3*parent.width/4
        height: elementHeight*1.5
        x: screenGlobal.adaptXSize(10)
    }
    Text {
        id: aimRunningState
        text: "Running"
        x: parent.width/2
        font.pixelSize: smallFontSize
        y: borderBeforeSummary.y + borderBeforeSummary.height*2
    }

    Button{
        id: stopAndReportButton
        y:  aimRunningState.y + aimRunningState.height + screenGlobal.adaptYSize(10)
        x: parent.width - width - screenGlobal.adaptXSize(10)
        text: "Stop + report"
        onPressed: {
            runningAims.stop(aimId)
            stopStartButton.text = "Start";
            stopAndReportButton.enabled = false

            progressLoader.setSource("ProgressReport.qml",{"aimId":singleAimWindow.aimId});
            reportProgressPopup.open()
        }
        enabled: false
    }

    Popup {
        id: reportProgressPopup
        x: 0
        y: borderBeforeProgress.y
        width:  parent.width
        height: parent.height - y
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        Loader{
            anchors.fill: parent
            id: progressLoader
        }
        Connections{
            target: progressLoader.item
            onReportCanceled: reportProgressPopup.close()
            onReportFinished: {
                reportProgressPopup.close()
                singleAimWindow.loadAim()
            }
        }
    }

    Button {
        y:  aimRunningState.y + aimRunningState.height + screenGlobal.adaptYSize(10)
        x: parent.width/2
        font.pixelSize: smallFontSize
        id: stopStartButton
        text: "stop|start"
        onClicked: {
            if (text == "Stop"){
                runningAims.stop(aimId)
                stopStartButton.text = "Start";
                stopAndReportButton.enabled = false
            } else
            if (text == "Start"){
                runningAims.start(aimId)
                stopStartButton.text = "Stop";
                stopAndReportButton.enabled = true
            }
        }
    }
    Timer {
        id: refreshTimer
        running: true; interval: 500; repeat: true
        onTriggered: {
            if (aimId.length > 0){
                if (runningAims.isRunning(aimId)){
                    aimRunningState.text = "Aim running " + runningAims.getSecondsPassed(aimId) + " seconds";
                    stopStartButton.text = "Stop";
                }
                else{
                    aimRunningState.text = "Aim is stopped"
                    stopStartButton.text = "Start";
                }
            }
         }
    }

    Rectangle {
         y: aimSummory.y + aimSummory.height + screenGlobal.adaptYSize(10)
         x: 0
         width: parent.width
         height: screenGlobal.adaptYSize(10)

         border.color: "black"
         color: "gray"
         id: borderBeforeLinks
    }

    Flickable {
        id: flickLinks

        y: borderBeforeLinks.y + screenGlobal.adaptYSize(20)
        x: screenGlobal.adaptXSize( 5 )
        width: parent.width - screenGlobal.adaptYSize( 10 )
        height: elementHeight*4
        contentWidth: parent.width - screenGlobal.adaptYSize( 10 )
        contentHeight: elementHeight*(linksRepeater.model + 1)

        ScrollBar.vertical: ScrollBar { active: true }


        property real span : contentY + height

        Repeater{
            id: linksRepeater

            function loadLinks(skipCheck){

                var links = localBase.getAimLinks(singleAimWindow.aimId)
                if (skipCheck !== true) {
                    if (linksRepeater.model<links.length)
                        linksRepeater.model = links.length
                }
                else
                    linksRepeater.model = links.length

                for (var i =0 ; i < links.length; ++i) {
                    linksRepeater.itemAt(i).valueOfLink = links[i][2].toString()
                    linksRepeater.itemAt(i).nameOfLink = links[i][3].toString()
                }
                flickLinks.reloadModel()
            }

            function saveSingleLink(aim, link, name) {
                if (localBase.isThereAimLink(aim,link))
                    localBase.changeAimLink(aim,link,link,name)
                else
                    localBase.addAimLink(aim,link,name)

                if (aim === singleAimWindow.aimId)
                    linksRepeater.loadLinks()
            }

            model: 1
            Item {
                y: index * elementHeight
                x: screenGlobal.adaptXSize( 5 )
                width:  flickLinks.width - screenGlobal.adaptYSize( 20 )
                height: elementHeight
                property bool inView:  (y+elementHeight) > (flickLinks.contentY) && (y+elementHeight) < (flickLinks.span + elementHeight)
                visible: inView

                id: linkItem

                property string valueOfLink : ""
                property string nameOfLink : ""
                property int indexValue: index

                RowLayout{
                    anchors.fill: parent
                    id:layout

                    TextField {
                        placeholderText: "link"
                        Layout.fillWidth: true
                        id: linkValue
                        text: valueOfLink
                    }
                    TextField {
                        placeholderText: "link name"
                        Layout.fillWidth: true
                        id: linkName
                        text: nameOfLink
                    }
                    RoundButton {
                       id: linkButton
                       onPressed: { linkMenu.y = linkButton.y; linkMenu.open() }
                       text: "..."
                       Menu {
                           id: linkMenu
                           MenuItem {
                               text: "Open"
                               //font.pixelSize:
                               onTriggered: Qt.openUrlExternally(linkValue.text)
                           }
                           MenuItem {
                               text: "Copy to clipboard"
                               onTriggered:clipboard.copyText(linkValue.text)
                           }
                           MenuItem {
                               text: "Save only this"
                               //font.pixelSize:
                               onTriggered: linksRepeater.saveSingleLink(singleAimWindow.aimId, linkValue.text,linkName.text)
                           }
                           MenuItem {
                               text: "Save all" //later get rid of it, by autoediting, when finished editing in 3 seconds - send to sql
                               //font.pixelSize:
                               onTriggered:;
                           }
                           MenuItem {
                               text: "Copy to other aim"
                               onTriggered:{
                                   aimSelectionPopup.linkIndex = linkItem.indexValue
                                   aimSelectionPopup.operationType = 1
                                   aimSelectLoader.setSource("SelectAim.qml") //{}
                                   aimSelectionPopup.open()
                               }
                           }
                           MenuItem {
                               text: "Move to other aim"
                               onTriggered:{
                                   aimSelectionPopup.linkIndex = linkItem.indexValue
                                   aimSelectionPopup.operationType = 2
                                   aimSelectLoader.setSource("SelectAim.qml") //{}
                                   aimSelectionPopup.open()
                               }
                           }
                           MenuItem {
                               text: "Delete"
                               onTriggered:{
                                   deletedLinks.append({"linkValue":linkValue.text,
                                                           "linkName":linkName.text});

                                   localBase.delAimLink(singleAimWindow.aimId,linkValue.text)

                                   ++deletedLinks.counter;

                                   restoreLinksButton.enabled = true
                                   linksRepeater.loadLinks(true)
                               }
                           }
                       }
                    }
                }
            }
        }
    }

    Popup {
        id: aimSelectionPopup
        x: 0
        y: borderBeforeProgress.y
        width:  parent.width
        height: parent.height-y
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        property int operationType: 0
        property int linkIndex: 0

        Loader {
            anchors.fill: parent
            id: aimSelectLoader
        }
        Connections{
            target: aimSelectLoader.item
            onSelectionCanceled:{ aimSelectionPopup.close(); aimSelectionPopup.operationType = 0}
            onAimSelected: {
                aimSelectionPopup.close()

                var linkValue = linksRepeater.itemAt(aimSelectionPopup.linkIndex).valueOfLink
                var linkName = linksRepeater.itemAt(aimSelectionPopup.linkIndex).nameOfLink

                 if (aimSelectionPopup.operationType == 1){
                    linksRepeater.saveSingleLink(selectedAimId,linkValue,linkName)
                 }
                 else if (aimSelectionPopup.operationType == 2){

                     linksRepeater.saveSingleLink(selectedAimId,linkValue,linkName)

                     deletedLinks.append({"linkValue":linkValue,
                                             "linkName":linkName});

                     localBase.delAimLink(singleAimWindow.aimId,linkValue)
                     ++deletedLinks.counter;
                     restoreLinksButton.enabled = true
                     linksRepeater.loadLinks(true)
                 }
                 else
                     console.log("Something got strange operation type on select aim is " + aimSelectionPopup.operationType)


                aimSelectionPopup.operationType = 0
            }
        }
    }

    ListModel{
        id:deletedLinks
        property int counter: 0
    }

    RoundButton{
        text: "+"
        y: parent.height-height-screenGlobal.adaptYSize(15)
        x: (parent.width-width)/2
        onPressed:{
            if (linksRepeater.itemAt(linksRepeater.model-1).valueOfLink == "")
                ;
            else{
            //DONT ADD WHEN LAST IS EMPTY
                linksRepeater.model++;
                linksRepeater.loadLinks() //true means add new
            }

        }
    }
    RoundButton{
        id:restoreLinksButton
        text: "Restore"
        y: parent.height-height-screenGlobal.adaptYSize(15)
        x: screenGlobal.adaptXSize(10)
        onPressed: {
            linksRepeater.model++;
            linksRepeater.loadLinks()

            linksRepeater.itemAt(linksRepeater.model - 1).valueOfLink = deletedLinks.get(0).linkValue;
            linksRepeater.itemAt(linksRepeater.model - 1).nameOfLink = deletedLinks.get(0).linkName;
            flickLinks.reloadModel()

            deletedLinks.remove(0)

            console.log("Deleted links " + deletedLinks.counter)

            --deletedLinks.counter;

            if (deletedLinks.count == 0)
                restoreLinksButton.enabled = false
        }

        enabled: false
    }

    FileDialog {
        id: exportFileDialog
        title: "Select aim export file name"
        //folder:
        nameFilters: [ "Aim files (*.aim)", "All files (*)" ]
        selectMultiple: false
        selectExisting: false
        onAccepted: {
            localBase.exportAim(singleAimWindow.aimId,exportFileDialog.fileUrl)
        }
    }

    Button {
        y: parent.height-height-screenGlobal.adaptYSize(15)
        //y: elementHeight
        x: parent.width - width - screenGlobal.adaptXSize(10)
        font.pixelSize: smallFontSize
        text : "Export aim"
        onClicked: exportFileDialog.open()
        id: exportButton
    }

    Button {
        y: elementHeight
        x: parent.width - width - screenGlobal.adaptXSize(10)
        font.pixelSize: smallFontSize
        text : "?"
        onClicked: singleAimWindow.helpMode = !singleAimWindow.helpMode
    }

    ToolTip {
        parent: aimMoment
        visible: singleAimWindow.helpMode
        text: qsTr("Aim main params: name, parent, moment & tags")
    }
    ToolTip {
        parent: childList
        visible: singleAimWindow.helpMode
        text: qsTr("Child aims of a current one")
    }
    ToolTip {
          parent: progressSlider.handle
          visible: progressSlider.pressed
          text: progressSlider.value.toFixed(1) + "%"
    }
    ToolTip {
        parent: stopAndReportButton
        visible: singleAimWindow.helpMode
        text: qsTr("Button stops running aim and helps input report")
    }
    ToolTip {

        parent: exportButton
        visible: singleAimWindow.helpMode
        text: qsTr("Button used to save aim in file")
    }
    ToolTip {
        parent: flickLinks
        visible: singleAimWindow.helpMode
        text: qsTr("Area used for links connected to aim")
        Component.onCompleted: y += screenGlobal.adaptYSize(50)
    }

}
