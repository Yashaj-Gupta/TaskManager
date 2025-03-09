#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <QObject>
#include <QTimer>


namespace apptools {

    class Notifications : public QObject {
        Q_OBJECT

    public:
        Notifications(QObject *parent = nullptr) : QObject(parent) {}
        ~Notifications() = default;

        void startWatchDog(int setIntervalSec = 60 * 5) {
            int intervalMs = 1000 * setIntervalSec;
            connect(&_watchDogTimer, SIGNAL(timeout()), this, SLOT(watchDogWoughf()));
            _watchDogTimer.start(intervalMs);
        }

    signals:
        void requestViewNotifications();

    public slots:
        virtual void watchDogWoughf()=0;

    protected:
        QTimer _watchDogTimer;
    };
}


#endif // NOTIFICATIONS_H
