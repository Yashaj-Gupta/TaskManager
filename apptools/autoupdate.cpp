#include "autoupdate.h"

#include <QFile>
#include <QFileDialog>

#include <QDebug>

#include <QDateTime>
#include <QTextStream>

#ifdef WIN32
#include <windows.h>
#include <process.h>
//for launcher
#endif

#include "apphandler.h"

AutoUpdate::AutoUpdate(QObject *parent) : QObject(parent)
{

}

//for a while ca use them as static

QString AutoUpdate::loadBinnaryFile(QString fileName)
{
    QFile localFile;

    if (fileName.isEmpty())
       fileName = QFileDialog::getOpenFileName(0,"Загрузка версии программы",""," (*.exe)") ;

    localFile.setFileName(fileName); //fileName);
    bool opened = localFile.open(QIODevice::ReadOnly);

    if (opened ==false)
        return "";

    lastLoadedFile= fileName;

    QByteArray fileData = localFile.readAll();
    QByteArray compressedData = qCompress(fileData,9);


    qDebug() << "Compressing data - size before: "<<fileData.size()
             << "; size after: "<<compressedData.size();

    //SHOULD add there those values
    /// quint32  uncompressedSize = fileData.size();
    //datastream?
   // QByteArray fileSize;
   // fileSize.fromRawData((char*)&uncompressedSize,4); //badbadbad??
    //compressedData = fileSize + compressedData;


    QByteArray basedData = compressedData.toBase64();
    QString response(basedData);

    localFile.close();

    return response;
}

bool AutoUpdate::saveBinnaryFile(QString fileName, QString fileData)
{
    QFile localFile;

    if (fileName.isEmpty())
       fileName = QFileDialog::getSaveFileName(0,"Сохранение версии программы",""," (*.exe)") ;

    localFile.setFileName(fileName);
    bool opened = localFile.open(QIODevice::WriteOnly);

    if (opened == false)
        return false;

    QByteArray unbasedData = QByteArray::fromBase64(fileData.toLocal8Bit());
    QByteArray decompressedData = qUncompress(unbasedData);

    qDebug() << "Data size before decompressing "<<fileData.size();

    qDebug() << "Decompressing.. compressed size: "<<unbasedData.size() <<"; decompressed size: "<<decompressedData.size();

    localFile.write(decompressedData);
    localFile.close();

    return true;
}

bool AutoUpdate::installNewVersion(QString fileName)
{
#ifdef WIN32

    QString currentMoment = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH_mm_ss");
    QString prevFilename = "updater/" + currentMoment + "_old.version";
    //better prev.version repace with moment
    std::string prevName = prevFilename.toStdString();
    std::string nextName = fileName.toStdString();

    QTextStream cout(stdout);

    std::string execName = executableName.toStdString();

    cout << "Move 1 : " << MoveFileExA(execName.c_str(),prevName.c_str(),0);
    cout << "Move 2 : " << MoveFileExA(nextName.c_str(),execName.c_str(),0); //creepy reminder

    cout << "Relaunching app "<<_execl(execName.c_str(),"-updated") <<"\n";     //PLEASE note that yet - default app handler doesnt't react
                                                                                                                                    ///attention should use it
    //after all just re-run
    ///_execl("ProcutionContol.exe","-updated"); //can send argument updated - to notify that thing
    //already done upper

#endif

    return true;
}
