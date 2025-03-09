#include "connstringgenerator.h"

#include <QByteArray>

#include <QDebug>


ConnStringGenerator::ConnStringGenerator(QObject *pa):QObject(pa)
{

}


QString ConnStringGenerator::generate(QString ip, QString port, QString user, QString password)
{
        QString multiString = ip + "\n" + port +  "\n" + user +  "\n" + password;

        QByteArray bytes = multiString.toLocal8Bit();
        QByteArray compressed = qCompress(bytes,9);
        QByteArray reversed;

        for (int i = compressed.size()-1; i >=0 ; --i)
            reversed.push_back(compressed[i]);

        QByteArray base64Array = reversed.toBase64();

        qDebug() << "Gen "<<base64Array.size()<<" comp "<<compressed.size();

        QString codedString = base64Array;

        return codedString;
}

QStringList ConnStringGenerator::decrypt(QString generatedString)
{
        QByteArray sourceArray = generatedString.toLocal8Bit();
        QByteArray decodedArray = QByteArray::fromBase64(sourceArray);

        QByteArray backReversed;

        for (int i = decodedArray.size()-1; i >=0 ; --i)
            backReversed.push_back(decodedArray[i]);

        QByteArray decompressed = qUncompress(backReversed);

        QString decodedString = QString(decompressed);
        QStringList elements = decodedString.split("\n");

        return elements;
}
