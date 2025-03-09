#ifndef AIMBASE_H
#define AIMBASE_H

#include <QObject>
#include <QVariantList>
#include <QSqlQuery>

#include "apptools/TreeModel.h"

namespace aim {

    class AbstractSqlBase : public QObject
    {
        Q_OBJECT
    public:
        explicit AbstractSqlBase(QObject *parent = nullptr);

    protected:
        virtual QSqlQuery executeRequest(const QString requestBody)=0;

        void notifySqlError(QSqlQuery &query);
        QString lastRequestBody;

        QVariantList fillList(QSqlQuery &query, const int rowsCount);

    };


    class LocalSqlBase : public AbstractSqlBase
    {
            Q_OBJECT
    public:
        explicit LocalSqlBase(QObject *parent = nullptr);
        ~LocalSqlBase() = default;


        Q_INVOKABLE QString addAim(const QString aimName, const QString timeAndDate, const QString comment, const QString tag,
                               const QString assignTo, const QString priority="",  const QString parent="", const QString progress="",
                               const QString repeatable="", const QString privacy="");

        Q_INVOKABLE bool editAim(const QString aimId, const QString aimName, const QString timeAndDate, const QString comment, const QString tag,
                                 const QString assignTo, const QString priority="", const QString parent="", const QString progress="",
                                    const QString repeatable="", const QString privacy=""); //maybe some params as links should be accesable alone

        Q_INVOKABLE bool deleteAim(const QString aimId);


        //=====================================

        Q_INVOKABLE QString getLastAimId();

        Q_INVOKABLE QVariantList getAims();
        Q_INVOKABLE QVariantList getAimsBackwards();

        Q_INVOKABLE QStringList getAimsNames();
        Q_INVOKABLE QStringList getAimsNamesBackwards(); //and with parent

        Q_INVOKABLE QStringList getSingleAim(const QString aimId);

        Q_INVOKABLE QVariantList getChildAims(const QString parentAimId);
        Q_INVOKABLE QStringList getChildAimsNames(const QString parentAimId);

        //Serialization
        Q_INVOKABLE bool exportAim(const QString aimId, QString filename);
        Q_INVOKABLE bool importAim(QString filename);


        Q_INVOKABLE int checkThereIsSameAim(const QString aimName, const QString timeAndDate, const QString comment, const QString tag,
                                            const QString assignTo, const QString priority="",  const QString parent="", const QString progress="",
                                            const QString repeatable="");

        Q_INVOKABLE int checkThereIsSameAim(const QStringList aimLine);

        Q_INVOKABLE int checkThereIsSameImportAim(QString filename);

        //=======================================

        Q_INVOKABLE QString secondsTranslate(const quint64 seconds);

        Q_INVOKABLE quint64 getDoneActionsLength(QString date);
        Q_INVOKABLE QStringList getDoneActionsList(QString date);
        Q_INVOKABLE QVariantList getAllDoneActionsList(QString date);

        Q_INVOKABLE QVariantList getAimsByDate(const QString date);
        Q_INVOKABLE QVariantList getAimsByDateOnly(QString date);
        Q_INVOKABLE QVariantList getPeriodHitAimsByDate(QString date);

        Q_INVOKABLE QVariantList getCurrentMomementAims();
        Q_INVOKABLE QStringList getCurrentMomementAimsNames();

        Q_INVOKABLE QVariantList getFutureAims();
        Q_INVOKABLE QVariantList getDelayedAims();

        Q_INVOKABLE QStringList getFutureAimsNames();
        Q_INVOKABLE QStringList getDelayedAimsNames();

        //=====================================

        Q_INVOKABLE QVariantList getAllTags();
        Q_INVOKABLE bool editTreeAims(apptools::TreeModel *aims);
        Q_INVOKABLE bool fillTreeModelWithAims();
        Q_INVOKABLE  bool fillTreeModelWithTags();

         //==================================

        Q_INVOKABLE bool addActivity(const QString aimId, const QString aimName,
                         const QString operation, const QString totalLength="");

        Q_INVOKABLE QVariantList getActivityLog(const QString aimId); //all records
        Q_INVOKABLE QString getActivitySummary(const QString aimId);

        Q_INVOKABLE bool updateAimProgress(const QString aimId, const QString progress, const QString progressText);

        Q_INVOKABLE QVariantList getLastActivities(); //returns last 100 acts of all aims
        Q_INVOKABLE QVariantList getLastActsAims();

        //=========================

        Q_INVOKABLE QVariantList getAimLinks(const QString aimId);
        Q_INVOKABLE bool addAimLink(const QString aimId, const QString link, const QString linkName);
        Q_INVOKABLE bool delAimLink(const QString aimId, const QString link);
        Q_INVOKABLE bool changeAimLink(const QString aimId, const QString link, const QString newLink, const QString newName);
        Q_INVOKABLE bool isThereAimLink(const QString aimId, const QString link);

        //===================================

        Q_INVOKABLE QVariantList searchAimsByName(const QString searchText);

        //------------------------------------------------------------------------

        bool createTablesIfNeeded();

        apptools::TreeModel aimsTree;
        apptools::TreeModel tagsTree;

    protected:
        QSqlQuery executeRequest(const QString requestBody);

        QSqlError initDatabase();
    };

}


#endif // AIMBASE_H
