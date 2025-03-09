#ifndef APPCRASHREPORT_H
#define APPCRASHREPORT_H

#include <QString>
#include <QList>

class AppCrashReport
{

public:

    AppCrashReport(QString appName);

protected:

    void setSignalHandlers();
    QList<int> waitingSignals;

    void addAwaitingSignal(int signal);

    QString appName;

};

#endif // APPCRASHREPORT_H
