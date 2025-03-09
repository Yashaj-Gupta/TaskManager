import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.1
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.4 as QMLOld
import QtQuick.Controls.Styles.Flat 1.0 as Flat


Item {

    anchors.fill: parent

    property int widthOffset : screenGlobal.adaptXSize( 50 )
    property int microOffset : screenGlobal.adaptXSize( 10 )

    signal requestOpenAddAim()
    signal requestOpenEditAim(var aimId)
    signal requestOpenSingleAim(var aimId)

    QMLOld.Calendar
    {
        y: 0
        x: widthOffset
        width: parent.width - widthOffset * 2
        height: parent.width - widthOffset * 2
        id: calendar
        onSelectedDateChanged: {
            chosenDate.text = calendar.selectedDate
            aimList.loadModelByDate(chosenDate.text)
        }
    }

    Text {
        y: calendar.height
        id: chosenDate
        x: widthOffset
    }

    AimList {
        y: calendar.height + calendar.y + microOffset
        height: parent.height - (calendar.height + calendar.y + microOffset * 2)
        width: parent.width
        id:aimList
    }
}


