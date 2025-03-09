#ifndef AUTOUPDATE_H
#define AUTOUPDATE_H

#include <QObject>

class AutoUpdate : public QObject
{
    Q_OBJECT
public:
    explicit AutoUpdate(QObject *parent = 0);

    Q_INVOKABLE bool saveBinnaryFile(QString fileName, QString fileData);
    Q_INVOKABLE QString loadBinnaryFile(QString fileName);

    Q_INVOKABLE bool installNewVersion(QString fileName);

    Q_INVOKABLE QString getLastLoadedFilename() { return lastLoadedFile; }

    void setExecutableName(QString appName) { executableName = appName; }

signals:

public slots:

protected:

    QString executableName;
    QString lastLoadedFile;
};


#endif // AUTOUPDATE_H
