import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
//import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4 as QmlExtra
import QtQuick.Controls 1.4 as QmlOld
import QtQuick.Controls.Styles.Flat 1.0 as Flat


Item {
    id: aimAddWindow
    width:  screenGlobal.getScreenWidth()//480
    height: screenGlobal.getScreenHeight()//800


    signal requestOpenViewAims()
    //and other signals could be also here


    property int elementHeight: screenGlobal.adaptYSize(45) //y 45/800 - 0.05625
    property int widthOffset: screenGlobal.adaptXSize(20) //x 20/480 - 0.041(6)

    property int buttonWidth : screenGlobal.adaptXSize(50)//x - 0,1041(6)
    property int yOffset: screenGlobal.adaptYSize(50) //y - 0.0625
    property int microOffset : screenGlobal.adaptXSize(5)//x+- 0.01041(6)

    property int periopPopupWidth : screenGlobal.adaptXSize(360) //x - 0.75
    property int periopPopupHeight : screenGlobal.adaptYSize(300) //y    - 0.375          RENAME PROPERTY
    property int calendarLength : screenGlobal.adaptXSize(260) //xy - 0.325

    property int fontNormalSize : screenGlobal.adaptYSize(15)

    property bool editMode : false
    property string editingAimId : ""

    TextField
    {
        id: aimName

        y:yOffset-elementHeight //strange formula because of shift
        width: parent.width-widthOffset*2
        x: widthOffset

        height: elementHeight-microOffset
        font.pixelSize: fontNormalSize

        placeholderText: "Aim name"
    }

    TextField
    {
        id: timeAndDate

        y:yOffset+elementHeight*1
        width: parent.width-widthOffset*2 - widthOffset*3
        x: widthOffset

        placeholderText: "Time / date"
        //format { hh:mm[:ss] dd.MM[.yyyy] } //can have also buttons for dialogs

        height: elementHeight-microOffset
        font.pixelSize: fontNormalSize
    }
    Button
    {
        id: timeAndDatePick
        y:yOffset+elementHeight*1
        x: widthOffset  + timeAndDate.width + microOffset
        width: buttonWidth
        text: "Pick"

        height: elementHeight-microOffset
        font.pixelSize: fontNormalSize

        onClicked:
        {
                dateTimePopup.open()
        }
    }


    TextArea
    {
        id: commentText

        y: yOffset+elementHeight*3
        width: parent.width-widthOffset*2
        height: elementHeight*2

        x: widthOffset
        font.pixelSize: fontNormalSize

        placeholderText: "Place for comment\nMany lines"
    }


    TextField
    {
        id: tags

        y:yOffset+elementHeight*6
        width: parent.width-widthOffset*2
        x: widthOffset

        height: elementHeight-microOffset
        font.pixelSize: fontNormalSize

        placeholderText: "#tags"
    }

    ComboBox
    {
        id: assignTo

        y:yOffset+elementHeight*7
        width: parent.width-widthOffset*2
        x: widthOffset

        height: elementHeight-microOffset
        font.pixelSize: fontNormalSize

        model : ["Assign to:","myself","friend1","friend2"]
    }


    ComboBox
    {
        id: priority

        y:yOffset+elementHeight*9
        width: parent.width-widthOffset*2
        x: widthOffset

        height: elementHeight-microOffset
        font.pixelSize: fontNormalSize

        model : ["Priority:","0-none","25-low","50-mid","75-high","100-critical"]
    }


    ComboBox
    {
        id: parentAimName

        y:yOffset+elementHeight*11
        width: parent.width-widthOffset*2
        x: widthOffset

        height: elementHeight-microOffset
        font.pixelSize: fontNormalSize

        model : localBase.getAimsNamesBackwards()
    }


    TextField
    {
        id: repeatPeriod

        y:yOffset+elementHeight*13
        width: parent.width-widthOffset*2-widthOffset*2//also small offset
        x: widthOffset

        height: elementHeight-microOffset
        font.pixelSize: fontNormalSize

        placeholderText: "Repeat period"
        //format { hh:mm[:ss] dd.MM[.yyyy] } //can have also buttons for dialogs
    }
    Button
    {
        id: repeatPeriodPick
        y:yOffset+elementHeight*13
        x: widthOffset  + timeAndDate.width + microOffset
        width: 50
        text: "Set"

        height: elementHeight-microOffset
        font.pixelSize: fontNormalSize

        onClicked:
        {
                periodPopup.open()
        }
    }

    Button
    {
        id: addAimButton
        text: "Create aim"
        y: yOffset+elementHeight*15 //+ linkRepeater.model * elementHeight
        x: parent.width - width - widthOffset

        height: elementHeight-microOffset
        font.pixelSize: fontNormalSize

        visible: !editMode

        onClicked: {

            var assignToText = assignTo.currentText
            if (assignToText == "Assign to:")
                assignToText = ""

            var priorityText = priority.currentText
            if (priorityText == "Priority:")
                priorityText = ""

            var parentId = ""
            if (parentAimName.currentText.length > 0)
            {
                   var  parentPossibleList = localBase.searchAimsByName(parentAimName.currentText)

                if (parentPossibleList.length > 0)
                {
                    var firstLine = parentPossibleList[0]
                    if (firstLine.length > 1)
                        parentId = firstLine[0]
                }
           }


            var insertedIndex =//missing "" is progress
            localBase.addAim(aimName.text,timeAndDate.text, commentText.text, tags.text,
                                                 assignToText, priorityText,parentId,"",repeatPeriod.text);

            console.log("aim added")

            aimName.text = timeAndDate.text = commentText.text = tags.text = repeatPeriod.text = ""
            assignTo.currentIndex = -1
            priority.currentIndex = -1

            aimAddWindow.requestOpenViewAims()
        }
    }

    function loadAimForEdit(aimId)
    {
        //takes single aim and loads it things
        var aimList = localBase.getSingleAim(aimId)

        var aimNameValue = aimList[1]

        var timePart = aimList[2]
        var datePart = aimList[3]

        var timeAndDateValue = aimList[2]

        timeAndDateValue = datePart;
        if (timePart.length > 0)
            timeAndDateValue += "T" + timePart;


        var commentValue = aimList[4]
        var tagValue = aimList[5]
        var assignToValue = aimList[6]
        var priorityValue = aimList[7]

        //OK must store also aimId somewhere

        aimName.text = aimNameValue
        timeAndDate.text = timeAndDateValue
        commentText.text = commentValue
        tags.text = tagValue

        //assign to + priority skipped for a while..

        //need a nice way for combo boxes, probably function to pass through list and then find index and set in for combo

        editingAimId = aimId

        var progressValue = aimList[8]
        var progressTextValue = aimList[9]
        var parentAimValue = aimList[10]


        var repeatableValue = aimList[11]
        var privacyValue = aimList[12]

         parentAimName.model = localBase.getAimsNamesBackwards()

        editMode = true
    }

    function loadForAddNew()
    {
        aimName.text = timeAndDate.text = commentText.text = tags.text = repeatPeriod.text = ""
        assignTo.currentIndex = -1
        priority.currentIndex = -1

        editMode = false //to prepare for next time

        parentAimName.model = localBase.getAimsNamesBackwards()
    }

    Button
    {
        id: editAimButton
        text: "Edit aim"
        y: yOffset+elementHeight*15 //+ linkRepeater.model * elementHeight
        x: parent.width - width - widthOffset

        height: elementHeight-microOffset
        font.pixelSize: fontNormalSize

        visible: editMode

        onClicked: {

            var assignToText = assignTo.currentText
            if (assignToText == "Assign to:")
                assignToText = ""

            var priorityText = priority.currentText
            if (priorityText == "Priority:")
                priorityText = ""

            var parentId = ""
            if (parentAimName.currentText.length > 0)
            {
                    var parentPossibleList = localBase.searchAimsByName(parentAimName.currentText)

                if (parentPossibleList.length > 0)
                {
                    var firstLine = parentPossibleList[0]
                    if (firstLine.length > 1)
                        parentId = firstLine[0]
                }
           }


            var insertedIndex = //missing "" field is progress
            localBase.editAim(editingAimId ,aimName.text,timeAndDate.text, commentText.text, tags.text,
                                                 assignToText, priorityText,parentId,"",repeatPeriod.text);

            aimName.text = timeAndDate.text = commentText.text = tags.text = repeatPeriod.text = ""
            assignTo.currentIndex = -1
            priority.currentIndex = -1

            editMode = false //to prepare for next time

            aimAddWindow.requestOpenViewAims()
        }
    }


    //TODO:
    ///SO NOW ONLY MISSING CONFIGURATION AND SCROLLABLE IF TOO MANY ELEMENTS

    Dialog {
        id: afterAddDialog
        title: "Request"
        Text
        {
            text:  "Do you wish to add another aim?"
        }

        standardButtons: Dialog.Yes | Dialog.No

        onYes:
        {
            aimName.text=""
            timeAndDate.text=""
            assignTo.currentIndex=0
            parentAimName.currentIndex=0
        }
        onNo:
        {
            aimAddWindow.requestOpenViewAims()
        }
    }


    FontMetrics {
        id: fontMetrics
    }

    Component {
          id: delegateTumbler

          Label {
              text: popupDateItem.formatText(Tumbler.tumbler.count, modelData)
              opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
              horizontalAlignment: Text.AlignHCenter
              verticalAlignment: Text.AlignVCenter
              font.pixelSize: fontMetrics.font.pixelSize * 1.25
          }
      }

    Popup {
        id: periodPopup //make also time popup with tumbler
        x: microOffset*2
        y: yOffset*2
        width: periopPopupWidth
        height: periopPopupHeight
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        Item {
            id: popupPeriodItem

            Row {
                id: periodRow

                y: microOffset
                x: microOffset


                Tumbler {
                  id: daysTumbler
                  model: 31
                  delegate: delegateTumbler

                  width:  widthOffset*6
                }

                //ON CHOOSE TURN ON SWITCH
            }

            Button
            {
                y: yOffset*4 + microOffset*6 //230
                text: "Choose days period"

                x: buttonWidth*5-width

                height: elementHeight-microOffset
                font.pixelSize: fontNormalSize

                onClicked: {
                    console.log("done")

                    var daysAmount = daysTumbler.currentIndex

                    if (daysAmount)
                        repeatPeriod.text =  daysAmount + "d";

                    periodPopup.close()
                }
            }
            Button
            {
                y: yOffset*4 + microOffset*6 //230
                text: "Cancel"

                height: elementHeight-microOffset
                font.pixelSize: fontNormalSize

                x: microOffset*2
                onClicked: {
                    periodPopup.close()
                }
            }
        }
        }


    Popup {
        id: dateTimePopup //make also time popup with tumbler
        x: microOffset*2
        y: yOffset*2
        width: periopPopupWidth
        height: periopPopupHeight*2
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        Item {

            id: popupDateItem

            QmlOld.Calendar
            {
                y: microOffset
                x: microOffset //SHOULD BE REPLACE WITH SOME SCREEN VALUE

                width:  calendarLength
                height: calendarLength //special logic for always vertical Application

                id: calendar
            }

            function formatText(count, modelData) {
                var data = count === 12 ? modelData + 1 : modelData;
                return data.toString().length < 2 ? "0" + data : data;
            }



            Row {
                id: timeTumbler

                y: calendar.height + microOffset*2
                x: microOffset

                Tumbler {
                  id: hoursTumbler
                  model: 24
                  delegate: delegateTumbler

                  width:  widthOffset*6
                }

                Tumbler {
                  id: minutesTumbler
                  model: 60
                  delegate: delegateTumbler

                  width:  widthOffset*6
                }

                //ON CHOOSE TURN ON SWITCH
            }


            Switch
            {
                y: timeTumbler.y + timeTumbler.height + microOffset
                id: useTime
                checked: false

                height: elementHeight-microOffset
            }

            Button
            {
                y: useTime.y + useTime.height
                text: "Choose"

                height: elementHeight-microOffset
                font.pixelSize: fontNormalSize

                x: buttonWidth*5-width

                onClicked: {
                    timeAndDate.text = calendar.selectedDate

                    var datePart = timeAndDate.text.substring(0,10)

                    if (useTime.checked==false)
                        timeAndDate.text = datePart
                    else
                    {
                        var hoursValue = hoursTumbler.currentIndex < 10 ? "0" + hoursTumbler.currentIndex : hoursTumbler.currentIndex;
                        var minutesValue = minutesTumbler.currentIndex < 10 ? "0" + minutesTumbler.currentIndex : minutesTumbler.currentIndex;

                        var timeValue =  hoursValue + ":" + minutesValue;

                        timeAndDate.text = datePart + "T" + timeValue;
                    }

                    dateTimePopup.close()
                }
            }
        }
        }

}

