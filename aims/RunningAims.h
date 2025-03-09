#ifndef RUNNINGAIMS_H
#define RUNNINGAIMS_H

#include <QObject>
#include <QVariantList>
#include <QMap>
#include <QDateTime>

#include "AimBase.h"

namespace aim {

    class RunningAims : public QObject
    {
        Q_OBJECT
    public:

        explicit RunningAims(LocalSqlBase& base, QObject *parent = nullptr);

        Q_INVOKABLE QVariantList getRunningAims() const { return _active; }
        Q_INVOKABLE QVariantList getStoppedAims() const { return _stopped; }

        Q_INVOKABLE void start(const QString aimId);
        Q_INVOKABLE void stop(const QString aimId);

        Q_INVOKABLE int getSecondsPassed(QString aimId) const;

        Q_INVOKABLE bool isRunning(const QString aimId) const;

        Q_INVOKABLE QStringList getRunningAimsNames() const;
        Q_INVOKABLE QStringList getStoppedAimsNames() const;

    protected:

        QVariantList _active;
        QVariantList _stopped;
        LocalSqlBase& _localBase;
        QList<QDateTime> _activeRuns;

    };

}

#endif // RUNNINGAIMS_H
