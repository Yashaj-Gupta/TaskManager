import QtQuick 2.0
import aim.tasks 1.0

Item {
    anchors.fill: parent
    id:item

    ConsoleLog {
        id:consoleLog
        y: 100
        width: parent.width
        height: parent.height
        MouseArea {
            anchors.fill: parent
            onClicked: {
                consoleLog.update()
            }
        }
    }

    Timer {
        id: refTimer
        interval: 1000
        onTriggered: {
            consoleLog.update()
        }
        repeat: true
        running: false
    }

    Component.onCompleted:{
        refTimer.running = true
    }
}
