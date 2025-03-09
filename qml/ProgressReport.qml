import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.15
import QtQuick.Extras 1.4 as QmlExtra
import QtQuick.Controls 1.4 as QmlOld
import QtQuick.Controls.Styles.Flat 1.0 as Flat

Item {
    id: reportProgressWindow

    width: screenGlobal.getScreenWidth()//480
    height: screenGlobal.getScreenHeight()//800

    signal reportCanceled()
    signal reportFinished()

    property string aimId : ""
    property int smallFontSize : screenGlobal.adaptYSize(15)

    Slider{
        id: progressSlider
        y: screenGlobal.adaptYSize(10)
        width: parent.width - screenGlobal.adaptXSize(20)
        from: 1
        to: 100
        ToolTip {
              parent: progressSlider.handle
              visible: progressSlider.pressed
              text: progressSlider.value.toFixed(1) + "% of total aim"
          }
    }

    ScrollView {
         id: progressView
         y: progressSlider.y + progressSlider.height + screenGlobal.adaptYSize(10)
         width: parent.width-screenGlobal.adaptXSize(20)
         height: elementHeight*3
         x: screenGlobal.adaptXSize(10)
        TextArea{
            id: progressText
            placeholderText: "Progress addition"
            font.pixelSize: smallFontSize
        }
    }


    Button{
        id: saveProgressButton
        y: progressView.y + progressView.height + screenGlobal.adaptYSize(10)
        x: parent.width - width - screenGlobal.adaptXSize(10)
        text : "Save progress"
        font.pixelSize: smallFontSize //its normal size verywhere
        onClicked: {
            var aimLine = localBase.getSingleAim(reportProgressWindow.aimId)
            var fullProgress = parseFloat(aimLine[8]) + progressSlider.value
            var fullProgressText = aimLine[9]
            fullProgressText += "\n" + new Date().toLocaleDateString(Qt.locale()) + "\n"
            fullProgressText += progressText.text
            fullProgressText += "\n[" + progressSlider.value +"%]\n"
            localBase.updateAimProgress(reportProgressWindow.aimId,fullProgress,fullProgressText)
            reportProgressWindow.reportFinished()
        }
    }

    Button{
        y: progressView.y + progressView.height + screenGlobal.adaptYSize(10)
        x: screenGlobal.adaptXSize(10)
        text : "Cancel"
        font.pixelSize: smallFontSize
        onPressed: reportProgressWindow.reportCanceled()
    }
}

