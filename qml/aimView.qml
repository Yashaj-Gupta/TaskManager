import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.1
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.15

Item {

    id: aimViewWindow
    width: screenGlobal.getScreenWidth()
    height: screenGlobal.getScreenHeight()

    signal requestOpenAddAim()
    signal requestOpenEditAim(var aimId)
    signal requestOpenSingleAim(var aimId)

    property int elementHeight: screenGlobal.adaptYSize(45)
    property int widthOffset: screenGlobal.adaptXSize(70)
    property int yOffset: screenGlobal.adaptYSize(50)
    property int microOffset : screenGlobal.adaptXSize(10)
    property int roundButtonFontSize : screenGlobal.adaptYSize(20)
    property int aimListMicroOffset : screenGlobal.adaptYSize(25)
    property bool desktopClient: Qt.platform.os != "android" && Qt.platform.os != "ios"

    Item
    {
        y: yOffset
        x: microOffset
        width: parent.width - widthOffset*2
        id: searchBox
        visible: desktopClient

        property int fullHeight: elementHeight*2

        TextField
        {
            id: aimNameSearch
            placeholderText: "Input here search request"
            visible: true
            width: parent.width
            font.pixelSize: roundButtonFontSize
            onTextChanged: {
                searchTimer.restart()
                if (searchByName.checked) {
                    if (text.length > 0)
                        aimList.searchModel(text)
                    else
                        aimList.loadModel()
                }
                if (searchByTag.checked) {
                    if (text.length > 0)
                        aimList.searchModelTag(text)
                    else
                        aimList.loadModel()
                }
            }
        }

        RadioButton
        {
            y: elementHeight
            x: 0
            id: searchByName
            checked: true
            text: "name"
            font.pixelSize: roundButtonFontSize
        }

        RadioButton
        {
            y: elementHeight
            x: parent.width - width
            id: searchByTag
            text: "tag"
            font.pixelSize: roundButtonFontSize
        }
    }

    Item
    {
        y: yOffset - microOffset*2
        x: searchBox.width + widthOffset + microOffset * 2
        width: widthOffset-microOffset
        id: settingsBox
        visible: desktopClient //false

        RoundButton {
            text:  "*" // "\u2699"
            font.pixelSize: roundButtonFontSize
            onClicked: {
                aimList.requestSettingsPopup()
            }
            ToolTip.visible: hovered
            ToolTip.text: qsTr("Open settings")
        }
    }

    Item {
        y: yOffset*2
        x: searchBox.width + widthOffset + microOffset*2
        width: widthOffset-microOffset
        id: addBox
        RoundButton {
            text:  "+"
            font.pixelSize: roundButtonFontSize
            onClicked: {
                aimViewWindow.requestOpenAddAim()
            }
            ToolTip.visible: hovered
            ToolTip.text: qsTr("Add aim")
        }
    }

    Timer{
        id: searchTimer
        running: false
        interval: 10000
        repeat: false
        onTriggered: {
            if (desktopClient == false) {
                searchBox.visible = false
                settingsBox.visible = false
            }
        }
    }

    Rectangle {
         y: searchBox.visible ? searchBox.fullHeight + yOffset +microOffset: yOffset+microOffset
         x: 0
         width: parent.width
         height:microOffset
         border.color: "black"
         color: "gray"
    }

    AimList {
        id: aimList
        width: parent.width
        y: searchBox.visible ? searchBox.fullHeight + yOffset + aimListMicroOffset : yOffset + aimListMicroOffset
        height: parent.height - yOffset * 4
        Component.onCompleted: {
            aimList.loadModel()
        }
    }
}
