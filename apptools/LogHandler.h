#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include <QObject>

#include <QQuickPaintedItem>
#include <QPainter>


namespace apptools {


    class LogHandler : public QObject {
        Q_OBJECT

    public:

        explicit LogHandler(QObject *parent = nullptr);

        static LogHandler& getInst() {
            static LogHandler logHandler;
            return logHandler;
        }

        void addLine(const QString anotherLine);
        QStringList getLines() const { return _logLines; }
        void setFilename(const QString filename) { _logFileName = filename; }
        QtMessageHandler getOldHandler() { return _oldHandler; }

    private:
        QStringList _logLines;
        QString _logFileName;
        QtMessageHandler _oldHandler;
    };


    class ConsoleLogQML : public QQuickPaintedItem {
        Q_OBJECT

      public:
        ConsoleLogQML() = default;
        ~ConsoleLogQML() = default;

        void paint(QPainter* painter);
    };
}


#endif // LOGHANDLER_H
