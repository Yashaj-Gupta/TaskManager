import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2 //avoid it too
import QtQuick.Extras 1.4  //avoid it please
import QtQuick.Layouts 1.3 //isn't there 2?

Item {

    id: aimViewWindow
    width: 480
    height: 800

    property int elementHeight: 45
    property int widthOffset: 70
    property int yOffset: 50 //be aware of low screens try everywhere


    SwipeView {
        id: swipeView

        y: yOffset  + yOffset/2 //somthing wrong here
        x: 0

        width: parent.width
        height: parent.height

        currentIndex: 1
        //anchors.fill: parent

        Item {
            id: firstPage
            Loader {
                y: 0
                anchors.centerIn: parent
                anchors.fill:  parent
                id: addLoader
                source: "aimAdd.qml"
            }
        }
        Item {
            id: secondPage
            Loader {
                y: 0
                anchors.centerIn: parent
                anchors.fill:  parent
                id: viewLoader
                source: "aimView.qml" //GOOD to scroll qml to add, if there are no aims
            }
        }
        Item {
            id: thirdPage
            Loader {
                y: 0
                anchors.centerIn: parent
                anchors.fill:  parent
                id: scheduleLoader
                source: "schedule.qml"
            }
        }
    }

    RowLayout
    {
        y: yOffset/2
        //anchors.bottom: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

    RoundButton
    {
        text:"<"
        onClicked: swipeView.decrementCurrentIndex()
    } //HEY show the buttons only on desktop app

    PageIndicator {
        id: pageIndicator
        interactive: false
        count: swipeView.count
        currentIndex: swipeView.currentIndex

    }

    RoundButton
    {
        text:">"
        onClicked: swipeView.incrementCurrentIndex()
    } //HEY show the buttons only on desktop app

    //Could make settings over here also - and hidable toolbar then

    }

    Connections
    {
        target: addLoader.item
        onRequestOpenViewAims: swipeView.setCurrentIndex(1); //also had to forse load
    }

}
