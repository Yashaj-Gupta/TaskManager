#ifndef AIMNOTIFICATIONS_H
#define AIMNOTIFICATIONS_H

#include <QObject>
#include <QTimer>

#include "AimBase.h"
#include "apptools/Popup.h"


namespace aim {


    class NotificationClient : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString notification READ notification WRITE setNotification NOTIFY notificationChanged)

    public:
        explicit NotificationClient(QObject *parent = nullptr);

        Q_INVOKABLE void setNotification(const QString &notification);
        QString notification() const;

    signals:
        void notificationChanged();

    private slots:
        void updateAndroidNotification();

    private:
        QString _notification;
    };



    class AimNotifications : QObject
    {
        Q_OBJECT

    public:
        AimNotifications(LocalSqlBase& base, apptools::PopUp& popUp, QObject *parent = nullptr);
        ~AimNotifications() = default;

        void startWatchDog(int setInterval = 60);

    signals:
        void requestViewNotifications();

    public slots:
        void watchDogWoughf();

    protected:
        QTimer _watchDogTimer;
        LocalSqlBase& _localBase;
        apptools::PopUp& _popUp;

     public:
        NotificationClient androidNotification;
    };


}


#endif // AIMNOTIFICATIONS_H
