#include "AimNotifications.h"
#include <QDebug>


#ifdef ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#endif

using namespace aim;


NotificationClient::NotificationClient(QObject *parent)
    : QObject(parent)
{
    connect(this, SIGNAL(notificationChanged()), this, SLOT(updateAndroidNotification()));
}


void NotificationClient::setNotification(const QString &notification)
{
    if (_notification == notification)
        return;

    _notification = notification;
    emit notificationChanged();
}


QString NotificationClient::notification() const {
    return _notification;
}


void NotificationClient::updateAndroidNotification() {
    #ifdef ANDROID

    qDebug() << "Running android notification attempt";

    QAndroidJniObject javaNotification = QAndroidJniObject::fromString(_notification);
    QAndroidJniObject::callStaticMethod<void>("at/wavespl/apps/aim/NotificationClient",
                                       "notify",
                                       "(Ljava/lang/String;)V",
                                       javaNotification.object<jstring>());
    #endif
}


AimNotifications::AimNotifications(LocalSqlBase& base, apptools::PopUp& popUp, QObject *parent)
    : QObject(parent), _localBase(base), _popUp(popUp)
{}


void AimNotifications::startWatchDog(int setInterval)
{
    int interval = 1000*setInterval;
    connect(&_watchDogTimer,SIGNAL(timeout()),this,SLOT(watchDogWoughf()));
    _watchDogTimer.start(interval);
}


void AimNotifications::watchDogWoughf()
{
   QVariantList currentAims = _localBase.getCurrentMomementAims();

   if (currentAims.size()) {
       qDebug() << "There are aims now! "<<currentAims;
       QString messageText;

       if (currentAims.size() == 1) {
           QStringList aimLine = currentAims[0].toStringList();
           messageText = aimLine[1] + " starts now!";
       }
       else
           messageText = QString::number(currentAims.size()) + " aims start now!";

#ifdef ANDROID
        androidNotification.setNotification(messageText);
        //later show popup too, but it should have another shape
#else
       _popUp.setPopupText(messageText,"gray",10);
       _popUp.show();
#endif
   }
}


