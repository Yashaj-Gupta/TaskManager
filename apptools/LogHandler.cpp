#include "LogHandler.h"

using namespace apptools;

void newLogMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString typeLetter=" ";

    if (type == QtDebugMsg)
        typeLetter = "D";
    else if (type == QtInfoMsg)
        typeLetter = "I";
    else if (type == QtWarningMsg)
        typeLetter = "W";
    else if (type == QtCriticalMsg)
        typeLetter = "C";
    else if (type == QtFatalMsg)
        typeLetter = "F";

    QString logLine =  QString("[%1][%2:%3:%4] %5")
            .arg(typeLetter, context.file, context.function).arg(context.line).arg(msg);

    LogHandler::getInst().addLine(logLine);
    LogHandler::getInst().getOldHandler()(type, context, logLine); //msg
}


LogHandler::LogHandler(QObject *parent) : QObject(parent) {
    _oldHandler = qInstallMessageHandler(newLogMessageHandler);
}


void LogHandler::addLine(const QString anotherLine)
{
    if (_logFileName.isEmpty() == false) {

        QString closedLine = anotherLine + QString("\n\n");
        QFile logFile(_logFileName);
        logFile.open(QIODevice::Append);
        logFile.write(closedLine.toLocal8Bit());
        logFile.close();
    }
    _logLines << anotherLine;
    if (_logLines.size() > 200)
        _logLines.removeAt(0);
}



void ConsoleLogQML::paint(QPainter* painter){
    QStringList log = LogHandler::getInst().getLines();
    int counter = 0;
    for (int i = log.size()-1; i >= 0; i--) {
        ++counter;
        if (counter > 100) break;
        QString line =  log[i];
        painter->drawText(20, counter*10, line);
    }
}
