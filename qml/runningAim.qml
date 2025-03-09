import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.1
//import QtQuick.Extras 1.4
import QtQuick.Layouts 1.15
import QtQuick.Extras 1.4 as QmlExtra
import QtQuick.Controls 1.4 as QmlOld
import QtQuick.Controls.Styles.Flat 1.0 as Flat


Item {
    id: runningAimWindow
    width: screenGlobal.getScreenWidth()//480
    height: screenGlobal.getScreenHeight()//800

    signal requestOpenSingleAim(var aimId)

    property int elementHeight: screenGlobal.adaptYSize(45) //apply to every one
    property int widthOffset: screenGlobal.adaptXSize(20) //70
    property int yOffset: screenGlobal.adaptYSize(50) //be aware of low screens try everywhere
    property int veryBigFontSize : screenGlobal.adaptYSize(50)
    property int fontNormalSize : screenGlobal.adaptYSize(15)
    property string currentAimId : ""

    ComboBox {
        id: listTypeSelect
        y: yOffset
        width: parent.width-widthOffset*2
        x: widthOffset
        font.pixelSize: fontNormalSize

        model : ["All aims","This moment(+5min?)","Delayed","In future","Running","Last stopped"]

        onCurrentIndexChanged: {
            //reload aim select
            if (currentIndex==0)
                aimSelect.model = localBase.getAimsNames()
            else if (currentIndex == 1)
                aimSelect.model = localBase.getCurrentMomementAimsNames()
            else if (currentIndex == 2)
                aimSelect.model = localBase.getDelayedAimsNames()
            else if (currentIndex == 3)
                aimSelect.model = localBase.getFutureAimsNames()
            else if (currentIndex == 4)
                aimSelect.model = runningAims.getRunningAimsNames()
            else if (currentIndex == 5)
                aimSelect.model = runningAims.getStoppedAimsNames()
        }
    }

    ComboBox{
        id: aimSelect

        y: yOffset+elementHeight*2
        width: parent.width-widthOffset*2
        x: widthOffset
        font.pixelSize: fontNormalSize
        model: localBase.getAimsNames()

        function loadSelectedAim() {
            if (currentAimId.length > 0) {
                if (runningAims.isRunning(currentAimId)) {
                    stopButton.enabled = true
                    startButton.enabled = false
                }
                else {
                    startButton.enabled = true
                    stopButton.enabled = false
                }

                secondsCount.text = runningAims.getSecondsPassed(currentAimId)
                summaryText.text = localBase.getActivitySummary(currentAimId)
                var actLog = localBase.getActivityLog(currentAimId)
                var fullLogText = ""

                for (var i = actLog.length-1; i >=0; --i) {
                    var actLine = actLog[i]
                    var operation = actLine[3]
                    var moment = actLine[4]
                    var totalLength = actLine[5]
                    var fullLine = operation + " " + moment;
                    if (totalLength.length > 0)
                        fullLine += " duration " + totalLength;
                    fullLine += "\n";
                    fullLogText += fullLine
                }
                logOfActivity.text =   fullLogText
            }
        }

        onCurrentIndexChanged: {
            var aimTable

            if (listTypeSelect.currentIndex == 0) //ALL
                if (currentIndex >=1)
                    aimTable = localBase.getAims()
            if (listTypeSelect.currentIndex == 1) //CurrentMoment
                    aimTable = localBase.getCurrentMomementAims()
            if (listTypeSelect.currentIndex == 2) //Delayed
                    aimTable = localBase.getDelayedAims()
            if (listTypeSelect.currentIndex == 3) //Future
                aimTable = localBase.getFutureAims()

            if (listTypeSelect.currentIndex == 4) //Running
                aimTable = runningAims.getRunningAims()
            if (listTypeSelect.currentIndex == 5) //Stopped
                    aimTable = runningAims.getStoppedAims()

            if (aimTable !== undefined && aimTable.length >= currentIndex) {
                var localMiniShift = 0
                if (listTypeSelect.currentIndex == 0)
                    localMiniShift = 1

                var aimLine = aimTable[currentIndex-localMiniShift]
                var aimId = aimLine[0]
                currentAimId = aimId;
            }

            loadSelectedAim()
        }
    }

    Button {
        id:startButton
        y :yOffset+elementHeight*4
        x : widthOffset
        text : "Start"
        font.pixelSize: fontNormalSize

        onClicked: {
            stopButton.enabled = true
            startButton.enabled = false

            if (currentAimId.length > 0)
                runningAims.start(currentAimId)
        }
    }

    Button {
        id:showButton
        y: yOffset+elementHeight*4
        x : (parent.width - widthOffset - width)/2
        enabled: currentAimId != ""
        text: "Open view"
        font.pixelSize: fontNormalSize
        onClicked: {
            console.log("Opening " + currentAimId)
            runningAimWindow.requestOpenSingleAim(currentAimId)
        }
    }

    Button {
        id:stopButton
        y: yOffset+elementHeight*4
        x : parent.width - widthOffset - width
        text: "Stop"
        font.pixelSize: fontNormalSize
        enabled: false

        onClicked: {
            stopButton.enabled = false
            startButton.enabled = true
            if (currentAimId.length > 0)
                runningAims.stop(currentAimId)
            aimSelect.loadSelectedAim()
        }
    }

    Text {
        id:secondsCount
        x: widthOffset + (parent.width-width-widthOffset)/2 //WOULD LIKE to see it in center :)
        y: yOffset+elementHeight*6
        font.pointSize: veryBigFontSize
        text: "0:0"
    }

    Timer {
        id: refreshTimer
        running: true; interval: 500; repeat: true
        onTriggered: {
            if (currentAimId.length > 0)
                secondsCount.text = runningAims.getSecondsPassed(currentAimId)
        }
    }

    TextArea {
        id: logOfActivity
        y: yOffset+elementHeight * 9
        width: parent.width - widthOffset * 2
        height: elementHeight * 3
        x: widthOffset
        font.pixelSize: fontNormalSize
        placeholderText: "Simple log of activity,\ncan be replaced with ListView"
    }

    TextArea {
        id: summaryText
        y: yOffset+elementHeight*13
        width: parent.width-widthOffset*2
        height: elementHeight*3
        x: widthOffset
        font.pixelSize: fontNormalSize
        placeholderText: "Some summory about this aim, like total hours, times runned\nand in the end - time line!"
    }
}
