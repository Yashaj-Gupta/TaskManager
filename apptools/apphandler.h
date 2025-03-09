#ifndef APPHANDLER_H
#define APPHANDLER_H

#include <QObject>
#include <QString>
#include <QStringList>

class AppHandler : public QObject
{
    Q_OBJECT
public:
    AppHandler(QObject *parent=0) : QObject(parent), appNeedQuit(false) {}

    bool needQuit() { return appNeedQuit; }

    //as we don't use any polymorph things yet
    //virtual void checkArguments(int argc, char *argv[]) = 0;

      void defaultHandle(int argc, char *argv[]);

protected:


    bool appNeedQuit;
};

bool versionIsGreater(QString first, QString second);

//YET unsolved designed, how to connect it to other applications

#endif // APPHANDLER_H
