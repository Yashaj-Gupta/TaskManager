#include "RunningAims.h"

#include <QDebug>

#include "ListOperations.h"


using namespace aim;


RunningAims::RunningAims(LocalSqlBase &base, QObject *parent) : QObject(parent), _localBase(base)
{
}


QStringList RunningAims::getRunningAimsNames() const {
    return createListByField(getRunningAims(), 1);
}


QStringList RunningAims::getStoppedAimsNames() const {
    return createListByField(getStoppedAims(), 1);
}


void RunningAims::start(const QString aimId) {
    const QStringList aimLine = _localBase.getSingleAim(aimId);
    _active << aimLine;
    _activeRuns << QDateTime::currentDateTime();
    _localBase.addActivity(aimLine[0], aimLine[1],"start");
}


bool RunningAims::isRunning(const QString aimId) const {
    for (int i = 0; i < _active.size(); ++i) {
        const QStringList aimLine = _active[i].toStringList();
        if (aimLine[0] == aimId)
            return true;
    }
    return false;
}


int RunningAims::getSecondsPassed(QString aimId) const {
    for (int i = 0; i < _active.size(); ++i) {
        QStringList aimLine = _active[i].toStringList();
        if (aimLine[0]==aimId) {
            const QDateTime startMoment = _activeRuns[i];
            const QDateTime currentMoment = QDateTime::currentDateTime();
            return startMoment.secsTo(currentMoment);
        }
    }
    return 0;
}


void RunningAims::stop(const QString aimId)
{
    const QStringList aimLine = _localBase.getSingleAim(aimId);
    for (int i = 0; i < _active.size(); ++i) {
        const QStringList activeLine = _active[i].toStringList();
        if (activeLine[0] == aimId) {
            _active.removeAt(i);
            const QDateTime startMoment = _activeRuns[i];
            _activeRuns.removeAt(i);
            const QDateTime endMoment = QDateTime::currentDateTime();
            _stopped << aimLine; //or active line and don't touch base again
            const int totalSeconds = startMoment.secsTo(endMoment);
            const QString secondsString = QString::number(totalSeconds);
            _localBase.addActivity(aimLine[0], aimLine[1], "stop", secondsString);
            break;
        }
    }
}

