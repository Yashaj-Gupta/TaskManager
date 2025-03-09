#include "UserToken.h"

#include <QNetworkInterface>
#include <QCryptographicHash>


#ifdef ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#endif


using namespace aim;

UserToken::UserToken(QObject *parent) : QObject(parent) {
    prepareToken();
}


void UserToken::prepareToken()
{
    QString result;
    QCryptographicHash crypt(QCryptographicHash::Md5);
    bool oneExtra = false;
    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
        QString macAddress = interface.hardwareAddress();
        if (macAddress == "00:00:00:00:00:00:00:E0")
            continue;
        if (macAddress.size() == 0) {
    #ifdef ANDROID

        if (oneExtra == false) {
            QAndroidJniObject javaResult = QAndroidJniObject::callStaticObjectMethod<jstring>("at/wavespl/apps/aim/NotificationClient",
                                                                                     "getMacAddr");
            qDebug() << javaResult.toString() <<" from android!";
            macAddress = javaResult.toString();
            oneExtra = true;
        }
        else
            continue;
    #else
            continue;
    #endif
        }

        qDebug() << "Interface:"+macAddress +"\n";
        macAddress.replace(":","");
        QByteArray macBytes = macAddress.toLocal8Bit();

        crypt.reset();
        crypt.addData(macBytes);

        QString MD5Summ = QString(crypt.result().toHex());
        result += MD5Summ;
        qDebug() << MD5Summ << " : md5";
     }
    _token = result;
}
