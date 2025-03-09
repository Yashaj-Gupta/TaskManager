#ifndef CONNSTRINGGENERATOR_H
#define CONNSTRINGGENERATOR_H

#include <QObject>

class ConnStringGenerator : public QObject
{
    Q_OBJECT
public:

    Q_INVOKABLE QString generate(QString ip, QString port, QString user, QString password);
    Q_INVOKABLE QStringList decrypt(QString generatedString);

    ConnStringGenerator(QObject *pa=0);
};

#endif // CONNSTRINGGENERATOR_H
