import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.1
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.4 as QMLOld
import QtQuick.Controls.Styles.Flat 1.0 as Flat

Item {
    id:actionsItem
    anchors.fill: parent

    property int fontSizeMiddle : screenGlobal.adaptYSize(15)
    property int widthOffset : screenGlobal.adaptXSize( 50 )
    property int microOffset : screenGlobal.adaptXSize( 10 )

    signal requestOpenAddAim()
    signal requestOpenEditAim(var aimId)
    signal requestOpenSingleAim(var aimId)

    QMLOld.Calendar{
        y: 0
        x: widthOffset
        width: parent.width - widthOffset*2
        height: parent.width - widthOffset*2

        id: calendar

        onSelectedDateChanged:{
            chosenDate.text = calendar.selectedDate
            var seconds = localBase.getDoneActionsLength(chosenDate.text)
            summaryText.text = "Total spent " + localBase.secondsTranslate(seconds); // seconds + " : "
            var lists = localBase.getDoneActionsList(chosenDate.text)
            for (var i = 0; i < lists.length; ++i){
                var singleLine = lists[i]
                summaryText.text += "\n" + singleLine;
            }
            summaryText.height = screenGlobal.adaptYSize( parent.width * (lists.length+4) / 16 );
            flick.contentHeight = screenGlobal.adaptYSize(  parent.width *  (lists.length+4) / 16 );
        }

        Component.onCompleted: {
            calendar.selectedDateChanged()
        }
    }

    Text{
        y: calendar.height
        id: chosenDate
        x: widthOffset
    }

    Flickable {
        id: flick

        x: screenGlobal.adaptYSize( 5 )
        y: calendar.height + screenGlobal.adaptYSize( 30 )

        width: parent.width - widthOffset/2
        height: parent.height - y - screenGlobal.adaptYSize( 30 )
        contentWidth: parent.width * 1.2
        contentHeight: screenGlobal.adaptYSize(parent.height - y)

        ScrollBar.horizontal: ScrollBar { id: hbar; active: vbar.active }
        ScrollBar.vertical: ScrollBar { id: vbar; active: hbar.active }

        TextArea {
            textFormat: "RichText"
            id: summaryText
            width: parent.width * 2
            height: screenGlobal.adaptYSize( actionsItem.height )
            readOnly:  true
            font.pixelSize: fontSizeMiddle
            onLinkActivated:{
                console.log("Aim link:" + link)
                actionsItem.requestOpenSingleAim(link)
            }
        }
    }
}


