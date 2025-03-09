#ifndef UNSAVEDCHECK_H
#define UNSAVEDCHECK_H

#include <QObject>

class UnsavedCheck : public QObject
{
    Q_OBJECT
public:

   UnsavedCheck(QObject *parent) : QObject(parent), saved(true) {}

   Q_INVOKABLE void setUnsaved() { saved  = false; }
   Q_INVOKABLE void setSaved() { saved = true; }

   Q_INVOKABLE bool isSaved() { return saved; }

    //FORCE saved on moment when we had a tool push but its still unsaved

   Q_INVOKABLE bool forceSaved();

protected:

    bool saved;
};

//Plain and etc looking unfinished way to hold unsaved status for qml in different components
///MAYBE once good to connect with "work-log"

#endif // UNSAVEDCHECK_H
