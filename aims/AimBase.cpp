#include "AimBase.h"

#include <QDebug>
#include <QSqlError>
#include <QtSql>

#include <QRegExp>

#include <QDateTime>
#include <QDate>

#include "ListOperations.h"

using namespace aim;


AbstractSqlBase::AbstractSqlBase(QObject *parent) : QObject(parent)
{
}


void AbstractSqlBase::notifySqlError(QSqlQuery& query) {
    qDebug() << "SQL error: "<<query.lastError();
    qDebug() << "On request: "<<lastRequestBody;
}


QVariantList AbstractSqlBase::fillList(QSqlQuery &query, const int rowsCount) //maybe olso return as link&
{
    QVariantList filledList;
    while (query.next()) {
        QStringList oneLine;
        for (int i = 0; i < rowsCount; ++i)
            oneLine << query.value(i).toString();
        filledList << oneLine;
    }
    return filledList;
}



LocalSqlBase::LocalSqlBase(QObject *parent): AbstractSqlBase(parent)
{}


QSqlQuery LocalSqlBase::executeRequest(const QString requestBody) {
    lastRequestBody = requestBody;
    QSqlQuery request;
    if (request.exec(requestBody)==false)
        notifySqlError(request);
    return request;
}


QString LocalSqlBase::addAim(const QString aimName, const QString timeAndDate, const QString comment, const QString tag,
                         const QString assignTo, /*delayed*/ const QString priority,  const QString parent, [[maybe_unused]] const QString progress,
                        const QString repeatable, [[maybe_unsued]] const  QString privacy)
{

    QString datePart = timeAndDate.mid(0,timeAndDate.indexOf("T"));
    QString timePart;

    if (timeAndDate.indexOf("T") > 0)
        timePart = timeAndDate.mid(timeAndDate.indexOf("T")+1);

    QString requestBody("INSERT INTO aims (aimName,timePart,datePart,comment,tag," //OUTPUT INSERTED.aimId
                        "assignTo,priority,parentAim,repeatable) VALUES('" + aimName + "','" +
                        timePart + "','" + datePart  + "','" + comment + "','" + tag + "','" + assignTo + "','" + priority  +
                         "','" + parent +  "','" + repeatable + "');");

    QSqlQuery request = executeRequest(requestBody);
    return getLastAimId();
}



QString LocalSqlBase::getLastAimId() { //last added actually
    QString requestBody = "SELECT last_insert_rowid() FROM aims";
    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimIdList = fillList(request,1);
    QStringList aimId = aimIdList[0].toStringList();
    return aimId[0];
}


bool LocalSqlBase::editAim(const QString aimId, const QString aimName, const QString timeAndDate, const QString comment, const  QString tag,
                           const QString assignTo, /*delayed*/ const QString priority, const QString parent, [[maybe_unused]] const QString progress,
                         const QString repeatable, [[maybe_unused]] const QString privacy)
{
    QString datePart = timeAndDate.mid(0,timeAndDate.indexOf("T"));
    QString timePart;

    if (timeAndDate.indexOf("T") > 0)
        timePart = timeAndDate.mid(timeAndDate.indexOf("T")+1);

    QString requestBody = QString("UPDATE aims SET aimName='%1',timePart='%2',datePart='%3',comment='%4',tag='%5',"
                        "assignTo='%6',priority='%7',parentAim='%8',repeatable='%9'")
            .arg(aimName).arg(timePart).arg(datePart).arg(comment).arg(tag).arg(assignTo).arg(priority).arg(parent).arg(repeatable)
            +  QString(" WHERE aimId='%1';").arg(aimId);

    QSqlQuery request = executeRequest(requestBody);
    if (request.next())
        return request.value(0).toInt();
    return false;
}


bool LocalSqlBase::updateAimProgress(const QString aimId, const QString progress, const QString progressText)
{
    QString requestBody = QString("UPDATE aims SET progress='%1',progressText='%2'")
            .arg(progress).arg(progressText)
            +  QString(" WHERE aimId='%1';").arg(aimId);

    QSqlQuery request = executeRequest(requestBody);
    QDateTime now = QDateTime::currentDateTime();
    QString currentMoment = now.toString("yyyy-MM-ddTHH:mm:ss");

    QString logRecord = QString("INSERT INTO progress (aimId,progress,progressText,moment) VALUES('%1','%2','%3','%4');")
            .arg(aimId).arg(progress).arg(progressText).arg(currentMoment);

    if (request.next())
        return request.value(0).toInt();
    return false;
}


bool LocalSqlBase::editTreeAims(apptools::TreeModel *aims)
{
    QVariantList treeAims =  aims->getFullList();
    QVariantList allAims = getAims();
    QVariantList toInsert,toEdit,toDelete;
    aim::compareLists(allAims, treeAims,toInsert,toEdit,toDelete);

    for (int i = 0 ; i < toInsert.size(); ++i) {
        const QStringList aimLine = toInsert[i].toStringList();

        //const QString aimId = aimLine[0];
        const QString aimName = aimLine[1];
        const QString timePart = aimLine[2];
        const QString datePart = aimLine[3];
        const QString comment = aimLine[4];
        const QString tag = aimLine[5];
        const QString assignTo = aimLine[6];
        const QString priority = aimLine[7];
        const QString progress = aimLine[8];
        //const QString progressText = aimLine[9];
        const QString parentAim = aimLine[10];

        QString timeAndDate = datePart;
        if (timePart.isEmpty()==false)
            timeAndDate += "T" + timePart;

        addAim(aimName, timeAndDate, comment, tag, assignTo, priority, parentAim, progress);
    }

    for (int i = 0; i < toEdit.size(); ++i) {
        const QStringList aimLine = toEdit[i].toStringList();

        const QString aimId = aimLine[0];
        const QString aimName = aimLine[1];
        const QString timePart = aimLine[2];
        const QString datePart = aimLine[3];
        const QString comment = aimLine[4];
        const QString tag = aimLine[5];
        const QString assignTo = aimLine[6];
        const QString priority = aimLine[7];
        const QString progress = aimLine[8];
        const QString progressText = aimLine[9];
        const QString parentAim = aimLine[10];

        QString timeAndDate = datePart;
        if (timePart.isEmpty()==false)
            timeAndDate += "T" + timePart;

        editAim(aimId, aimName, timeAndDate, comment, tag, assignTo, priority, parentAim, progress);
    }

    for (int i = 0; i < toDelete.size(); ++i) {
        QStringList aimLine = toDelete[i].toStringList();
        const QString aimId = aimLine[0];
        deleteAim(aimId);
    }
    return true;
}



inline uint qHash(const QVariant& varString)
{
    const QString stringValue = varString.toString();
    return qHash(stringValue, 0xa03f);
}


QVariantList addTagPairs(QString tag)
{
    QVariantList result;
    if (tag.isEmpty())
        return result;

    int lastDot = tag.lastIndexOf(".");
    while (lastDot != -1)  {
        QStringList tagsPair;
        const QString parentTag = tag.mid(0,lastDot); //or no -1. hahaha?
        tagsPair << tag << parentTag;
        result << tagsPair;
        tag = parentTag;
        lastDot = tag.lastIndexOf(".");
    }

    QStringList tagsPair;
    tagsPair << tag << QString();
    result << tagsPair;
    return result;
}


QVariantList addTagPairs(QStringList aimLine)
{
    const QString tag = aimLine[5];
    QStringList tagParts = tag.split(" ");

    QVariantList result;
    for (int i = 0; i < tagParts.size(); ++i)
        result += addTagPairs(tagParts[i]);

    return result;
}


QVariantList LocalSqlBase::getAllTags()
{
    QVariantList allAims = getAims();
    QVariantList allTags;
    for (int i = 0; i < allAims.size(); ++i){
        QVariantList thisTag = addTagPairs(allAims[i].toStringList());
        allTags += thisTag;
    }
    QSet<QVariant> norepeats = allTags.toSet();
    QVariantList norepeatsList = norepeats.toList();
    return norepeatsList;
}


bool LocalSqlBase::deleteAim(const QString aimId)
{
    const QString requestBody("DELETE FROM aims WHERE aimId='"+
                        aimId + "';");

    QSqlQuery request = executeRequest(requestBody);
    return request.next();
}


QStringList LocalSqlBase::getAimsNames()
{
    const QVariantList aimsList = getAims();
    QStringList result;
    result << QString();
    result += aim::createListByField(aimsList, 1);
    return result;
}


QStringList LocalSqlBase::getAimsNamesBackwards()
{
    const QVariantList aimsList = getAims();
    QStringList result;
    result << QString();
    result += createListByFieldBackwards(aimsList, 1);
    return result;
}


QVariantList LocalSqlBase::getCurrentMomementAims()
{
    const QDateTime now(QDateTime::currentDateTime());
    const QString dateAndTime = now.toString("yyyy-MM-ddTHH:mm");

    const QString timePart = dateAndTime.mid(dateAndTime.indexOf("T")+1);
    const QString datePart = dateAndTime.mid(0,dateAndTime.indexOf("T"));

    QString requestBody = QString("SELECT * FROM aims WHERE timePart='%1' AND datePart='%2';")
            .arg(timePart).arg(datePart);

    QSqlQuery request = executeRequest(requestBody);
    return fillList(request, 13);;
}


QStringList LocalSqlBase::getCurrentMomementAimsNames() {
   return createListByField(getCurrentMomementAims(),1);
}


bool sortingCondition(const QVariant &v1, const QVariant &v2){
    const QStringList line1 = v1.toStringList();
    const QStringList line2 = v2.toStringList();
    const QString dateString1 = line1[3];
    const QString dateString2 = line2[3];
    const QDate date1 = QDate::fromString(dateString1,"yyyy-MM-dd");
    const QDate date2 = QDate::fromString(dateString2,"yyyy-MM-dd");
    return date1 < date2;
}


QVariantList LocalSqlBase::getFutureAims()
{
    const QDate today(QDate::currentDate());
    const QString requestBody = "SELECT * FROM aims WHERE datePart NOT NULL;";
    QSqlQuery request = executeRequest(requestBody);
    const QVariantList aimsList = fillList(request,13); //11 is fields amount

    QVariantList futureAims;
    for (int i = 0; i < aimsList.size(); ++i){
        const QStringList aimLine = aimsList[i].toStringList();
        const QString dateValue = aimLine[3];
        const QDate date = QDate::fromString(dateValue,"yyyy-MM-dd");
        if (date > today)
            futureAims << aimLine;
    }
    qSort(futureAims.begin(),futureAims.end(), sortingCondition);
    return futureAims;
}


QVariantList LocalSqlBase::getDelayedAims()
{
    const QDate today(QDate::currentDate());
    const QString requestBody = "SELECT * FROM aims WHERE datePart NOT NULL;";
    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimsList = fillList(request,13); //11 is fields amount

    QVariantList pastAims;
    for (int i = 0; i < aimsList.size(); ++i) {
        const QStringList aimLine = aimsList[i].toStringList();
        const QString dateValue = aimLine[3];
        const QDate date = QDate::fromString(dateValue,"yyyy-MM-dd");
        if (date < today)
            pastAims << aimLine;
    }
    return pastAims;
}


QStringList LocalSqlBase::getFutureAimsNames() {
    return aim::createListByField(getFutureAims(),1);
}


QStringList LocalSqlBase::getDelayedAimsNames() {
    return aim::createListByField(getDelayedAims(),1);
}


QVariantList LocalSqlBase::getAims() {
    QString requestBody = "SELECT * FROM aims";
    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimsList = fillList(request, 13);
    return aimsList;
}


QVariantList LocalSqlBase::getAimsBackwards() {
    QVariantList result;
    QVariantList aims = getAims();
    for (auto i = aims.size()-1; i >= 0; --i)
        result << aims[i];
    return result;
}


QVariantList LocalSqlBase::getChildAims(const QString parentAimId)
{
    const QString requestBody =
            QString("SELECT * FROM aims WHERE parentAim='%1';")
            .arg(parentAimId);
    QSqlQuery request = executeRequest(requestBody);
    return fillList(request, 13);;
}


QStringList LocalSqlBase::getChildAimsNames(const QString parentAimId)
{
    QVariantList aimsList = getChildAims(parentAimId);
    return createListByField(aimsList, 1);
}


QVariantList LocalSqlBase::getAimsByDate(const QString date)
{
    QVariantList aimsList = getAimsByDateOnly(date);
            aimsList += getPeriodHitAimsByDate(date);
    return aimsList;
}


QVariantList LocalSqlBase::getAimsByDateOnly(QString date)
{
    if (date.indexOf("T") != -1)
        date = date.mid(0,date.indexOf("T"));
    const QString requestBody = "SELECT * FROM aims WHERE datePart='" + date + "';";
    QSqlQuery request = executeRequest(requestBody);
    return fillList(request, 13);;
}


quint64 LocalSqlBase::getDoneActionsLength(QString date) {
    if (date.indexOf("T") != -1)
        date = date.mid(0,date.indexOf("T"));

    const QString requestBody = "SELECT totalLength FROM actions WHERE moment LIKE '" + date + "%';";
    QSqlQuery request = executeRequest(requestBody);
    QVariantList lengthList = fillList(request,1);
    quint64 totalSecondsSpent=0;
    for (auto i = 0; i < lengthList.size(); ++i){
        const QStringList l = lengthList[i].toStringList();
        const QString secondsSpent = l[0];
        totalSecondsSpent += secondsSpent.toUInt();
    }
    return totalSecondsSpent;
}


QString LocalSqlBase::secondsTranslate(const quint64 seconds)
{
    if (seconds == 0)
        return QString("is not finished");

    const quint64 minutes = seconds / 60;
    const quint64 leftSec = seconds % 60;
    const quint64 hours = minutes / 60;
    const quint64 leftMinutes = minutes % 60;

    return QString::number(hours) + QString("h ") + QString::number(leftMinutes) + QString("m ") +
            QString::number(leftSec) + QString("s");
}


QStringList LocalSqlBase::getDoneActionsList(QString date) {
    if (date.indexOf("T") != -1)
        date = date.mid(0,date.indexOf("T"));

    const QString requestBody = "SELECT * FROM actions WHERE moment LIKE '" + date + "%';";

    QSqlQuery request = executeRequest(requestBody);
    const QVariantList allActionsList = fillList(request, 6);

    QMap<QString,quint32> counts;
    for (auto i = 0; i < allActionsList.size(); ++i){
        QStringList actionLine = allActionsList[i].toStringList();
        if (counts.contains(actionLine[2]))
            counts[actionLine[2]] += actionLine[5].toUInt();
        else
            counts.insert(actionLine[2],actionLine[5].toUInt());
    }

    QStringList allStringLines;
    for (auto i = 0; i < counts.size(); ++i){
       const QString anotherKey =  (counts.begin()+i).key();
       const QStringList aim = getSingleAim(anotherKey);
       const QString fullLine = QString("<style>a:link { color:#11FF33; }</style>") +
               QString("<br>On <b><big>") + QString("  <a color='11FF22' href=\"") + anotherKey+
               QString("\">") + aim[1] + QString("</a>") + QString("</big></b>       ")
               + secondsTranslate(counts[anotherKey]);
       allStringLines << fullLine;
    }
    return allStringLines;
}


QVariantList LocalSqlBase::getAllDoneActionsList(QString date){
    if (date.indexOf("T") != -1)
        date = date.mid(0,date.indexOf("T"));
    const QString requestBody = "SELECT * FROM actions WHERE moment LIKE '" + date + "%';";
    QSqlQuery request = executeRequest(requestBody);
    QVariantList allActionsList = fillList(request,6); //6 is fields amount
    return allActionsList;
}


bool canDateHitPeriod(const QString originDate, QString period, const QString searchDate)
{
    const QString daysPeriod = period.mid(0,period.indexOf("d"));
    const qint64 daysAmount = daysPeriod.toInt();
    if (daysAmount == 0)
        return false;

    const QDate originDateValue = QDate::fromString(originDate,"yyyy-MM-dd");
    const QDate searchDateValue = QDate::fromString(searchDate,"yyyy-MM-dd");

    if (searchDateValue <= originDateValue)
        return false;

    const qint64 daysBetween = originDateValue.daysTo(searchDateValue);
    if (daysBetween % daysAmount==0)
        return true;

    return false;
}


QVariantList LocalSqlBase::getPeriodHitAimsByDate(QString date)
{
    if (date.indexOf("T") != -1)
        date = date.mid(0,date.indexOf("T"));

    const QString requestBody = "SELECT * FROM aims WHERE datePart NOT NULL AND repeatable NOT NULL;";
    QSqlQuery request = executeRequest(requestBody);

    const QVariantList aimsList = fillList(request,13);
    QVariantList hittedPeriodAims;

    for (int i = 0; i < aimsList.size(); ++i) {
        QStringList aimLine = aimsList[i].toStringList();
        QString originDate = aimLine[3];
        QString period = aimLine[11];
        if (canDateHitPeriod(originDate,period,date))
            hittedPeriodAims << aimLine;
    }
    return hittedPeriodAims;
}


QStringList LocalSqlBase::getSingleAim(const QString aimId) {
    const QString requestBody = "SELECT * FROM aims WHERE aimId='" + aimId + "';";
    QSqlQuery request = executeRequest(requestBody);
    const QVariantList allLines = fillList(request, 13);

    QStringList result;
    if (allLines.size() > 0)
        result = allLines[0].toStringList(); //taking only first line
    return result;
}


QVariantList LocalSqlBase::searchAimsByName(const QString searchText)
{
    const QString requestBody = "SELECT * FROM aims";
    QSqlQuery request = executeRequest(requestBody);

    QVariantList searchResult;
    const QVariantList aimsList = fillList(request,12);

    QRegExp pattern("*" + searchText + "*");
    pattern.setPatternSyntax(QRegExp::Wildcard);

    for (int i = 0; i < aimsList.size(); ++i) {
        QStringList aimLine= aimsList[i].toStringList();
        QString aimName = aimLine[1];
        if (pattern.exactMatch(aimName))
            searchResult.append(aimLine);
    }
    return searchResult;
}


void writeStringToFile(QFile &file, const QString& string){
    const quint32 stringLen = string.size();
    file.write((char*)&stringLen,sizeof (stringLen));
    file.write(string.toLocal8Bit());
}


QString readStringFromFile(QFile &file){
    quint32 stringLen = 0;
    file.read((char*)&stringLen,sizeof(stringLen));
    const QByteArray bytes = file.read(stringLen);
    return QString(bytes);
}


int LocalSqlBase::checkThereIsSameAim(const QString aimName, const QString timeAndDate, const QString comment, const QString tag,
                                         const QString assignTo, const QString priority, [[maybe_unused]] const QString parent,
                                        [[maybe_unused]] QString progress, QString repeatable)
{
    const QString datePart = timeAndDate.mid(0, timeAndDate.indexOf("T"));
    QString timePart;

    if (timeAndDate.indexOf("T") > 0)
        timePart = timeAndDate.mid(timeAndDate.indexOf("T")+1);

    const QString requestBody = QString("SELECT * FROM aims WHERE aimName='%1' AND timePart='%2' AND datePart='%3' AND comment='%4' AND tag='%5'"
                        "AND assignTo='%6' AND priority='%7' AND repeatable='%8';") //AND parentAim='%8'
            .arg(aimName).arg(timePart).arg(datePart).arg(comment).arg(tag).arg(assignTo).arg(priority).arg(repeatable); //.arg(parent)

    QSqlQuery request = executeRequest(requestBody);
    const QVariantList allLines = fillList(request,13);

    if (allLines.size() == 0)
        return 0;
    if (allLines.size() == 1)
        return 1;
    if (allLines.size() > 1)
        return 2;
    return -1;
}


int LocalSqlBase::checkThereIsSameAim(const QStringList aimLine) {
    QString fulldate = aimLine[3];
    if (aimLine[2].isEmpty() == false)
        fulldate += QString("T") + aimLine[2];

    return checkThereIsSameAim(aimLine[1], fulldate, aimLine[4], aimLine[5], aimLine[6],
            aimLine[7], "", aimLine[8], aimLine[11]);

}


int LocalSqlBase::checkThereIsSameImportAim(QString filename) {

    if (filename.indexOf("file://")!=-1)
        filename.replace("file://","");

    QFile importFile(filename);
    importFile.open(QIODevice::ReadOnly);
    if (importFile.isOpen()==false)
        return false;

    quint32 fieldsMask = 0;
    importFile.read((char*)&fieldsMask,sizeof (fieldsMask));

    const int totalFields = 13;
    QStringList loadedAim;

    for (auto i = 0; i < totalFields; ++i) {
        const quint32 localMask = 1 << i;
        if (fieldsMask & localMask)
            loadedAim << readStringFromFile(importFile);
        else
            loadedAim << QString();
    }

    return checkThereIsSameAim(loadedAim);
}


bool LocalSqlBase::exportAim(const QString aimId, QString filename){
    QStringList oneAim = getSingleAim(aimId);
    quint32 fieldsMask = 0;

    for (auto i = 0; i < oneAim.size(); ++i){
        if (oneAim[i].isEmpty()==false)
            fieldsMask |= 1 << i;
    }

    if (filename.indexOf("file://")!=-1)
        filename.replace("file://","");

    QFile exportFile(filename);
    exportFile.open(QIODevice::WriteOnly);
    if (exportFile.isOpen()==false)
        return false;

    exportFile.write((char*)&fieldsMask,sizeof (fieldsMask));

    for (auto i = 0; i < oneAim.size(); ++i)
       if (oneAim[i].isEmpty( )== false)
           writeStringToFile(exportFile, oneAim[i]);

    QVariantList links = getAimLinks(oneAim[0]);
    const quint32 linksSize = links.size();
    exportFile.write((char*)&linksSize,sizeof(linksSize));

    for (auto i = 0; i < links.size(); ++i){
        const QStringList linkLine = links[i].toStringList();
        const QString linkValue = linkLine[2];
        const QString linkName = linkLine[3];
        writeStringToFile(exportFile,linkValue);
        writeStringToFile(exportFile,linkName);
    }
    exportFile.close();
    return true;
}


bool LocalSqlBase::importAim(QString filename){

    if (filename.indexOf("file://")!=-1)
        filename.replace("file://","");

    QFile importFile(filename);
    importFile.open(QIODevice::ReadOnly);
    if (importFile.isOpen()==false)
        return false;

    quint32 fieldsMask = 0;
    importFile.read((char*)&fieldsMask,sizeof (fieldsMask));

    const int totalFields = 13;
    QStringList loadedAim;
    for (auto i = 0; i < totalFields; ++i){
        const quint32 localMask = 1 << i;
        if (fieldsMask & localMask)
            loadedAim << readStringFromFile(importFile);
        else
            loadedAim << QString();
    }

    const QString aimId = addAim(loadedAim[1], loadedAim[3]+QString("T")+loadedAim[2], loadedAim[4] ,loadedAim[5],
            loadedAim[6], loadedAim[7], loadedAim[10], loadedAim[8], loadedAim[11], loadedAim[12]);

    if (aimId != "0" && aimId != "")
        updateAimProgress(aimId,loadedAim[8],loadedAim[9]);

    quint32 linksSize = 0;
    importFile.read((char*)&linksSize,sizeof(linksSize));

    for (quint32 i = 0; i < linksSize; ++i){
        const QString linkValue = readStringFromFile(importFile);
        const QString linkName = readStringFromFile(importFile);
        addAimLink(aimId,linkValue,linkName);
    }
    return true;
}



QVariantList LocalSqlBase::getAimLinks(const QString aimId) {
    const QString requestBody("SELECT * FROM links WHERE aimId='" + aimId + "';");
    QSqlQuery request = executeRequest(requestBody);
    return fillList(request, 4);;
}


bool LocalSqlBase::addAimLink(const QString aimId, const QString link, const QString linkName){

    const QString requestBody("INSERT INTO links (aimId,link,name) VALUES('"
                        + aimId  + "','" + link + "','" + linkName + "');");

    QSqlQuery request = executeRequest(requestBody);
    if (request.next())
        return request.value(0).toInt();
    return false;
}


bool LocalSqlBase::isThereAimLink(const QString aimId, const QString link)
{
    const QString requestBody("SELECT * FROM links WHERE aimId='" + aimId + "' AND link='"+link +"';");
    QSqlQuery request = executeRequest(requestBody);
    QVariantList aimLink = fillList(request,4);
    return aimLink.size();
}


bool LocalSqlBase::delAimLink(const QString aimId, const QString link){
    const QString requestBody("DELETE FROM links WHERE aimId='"+
                        aimId + "' AND link='"+ link +"';");
    QSqlQuery request = executeRequest(requestBody);
    return request.next();
}


bool LocalSqlBase::changeAimLink(const QString aimId, const QString link, const QString newLink, const QString newName)
{
    const QString requestBody = QString("UPDATE links SET link='%1',name='%2'").arg(newLink).arg(newName)
            +  QString(" WHERE aimId='%1' AND link='%2';").arg(aimId).arg(link);

    QSqlQuery request = executeRequest(requestBody);
    if (request.next())
        return request.value(0).toInt();
    return false;
}


QSqlError LocalSqlBase::initDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("local.sqlite");

    if (!db.open())
        return db.lastError();

    const QStringList tables = db.tables();
    for (int i = 0; i < tables.size(); ++i)
        qDebug() << tables;

    if (tables.contains("aims", Qt::CaseInsensitive)
        && tables.contains("categories", Qt::CaseInsensitive))
        return QSqlError();

    return QSqlError();
}



bool LocalSqlBase::addActivity(const QString aimId, const QString aimName,
                 const QString operation, const QString totalLength)
{
    const QDateTime now = QDateTime::currentDateTime();
    const QString currentMoment = now.toString("yyyy-MM-ddTHH:mm:ss");
    const QString requestBody("INSERT INTO actions (aimName,aimId,operation,moment,totalLength) VALUES('"
                        +aimName + "','" + aimId  + "','" + operation + "','" + currentMoment + "','" + totalLength + "');");

    QSqlQuery request = executeRequest(requestBody);
    if (request.next())
        return request.value(0).toInt();

    return false;
}


QVariantList LocalSqlBase::getActivityLog(const QString aimId)
{
    const QString requestBody("SELECT * FROM actions WHERE aimId='" + aimId + "';");
    QSqlQuery request = executeRequest(requestBody);
    QVariantList activityLogResult = fillList(request, 6);
    return activityLogResult;
}


QVariantList LocalSqlBase::getLastActivities() {
    const QString requestBody("SELECT * FROM actions ORDER BY actId DESC LIMIT 100;");
    QSqlQuery request = executeRequest(requestBody);
    QVariantList activityLogResult = fillList(request, 6);
    return activityLogResult;
}


QVariantList LocalSqlBase::getLastActsAims(){
    const QVariantList lastActs = getLastActivities();
    QSet<QString> aimsId;

    for (auto i = 0; i < lastActs.size(); ++i){
        QStringList act = lastActs[i].toStringList();
        aimsId.insert(act[2]);
    }
    QVariantList collectedAims;
    for (auto it = aimsId.begin(); it != aimsId.end(); it++){
        QString oneAimId = (*it);
        QStringList singleAim = getSingleAim(oneAimId);
        collectedAims << singleAim;
    }
    return collectedAims;
}


QString LocalSqlBase::getActivitySummary(const QString aimId) {
    QVariantList actLog = getActivityLog(aimId);
    int countStarts = 0, countStops = 0;
    int totalSeconds = 0;

    for (int i = 0; i < actLog.size(); ++i) {
       const QStringList actionLine = actLog[i].toStringList();

       if (actionLine[3] == "start")
           ++countStarts;
       if (actionLine[3] == "stop") {
           ++countStops;
           QString actionLength = actionLine[5];
           int secondsAmount = actionLength.toInt();
           totalSeconds += secondsAmount;
       }
    }

    const QString result = "Summary: " + QString::number(countStarts)
            + " starts\n" + QString::number(countStops) + " stops\n"
            + "Total seconds spent: " + QString::number(totalSeconds);
    return result;
}


bool LocalSqlBase::createTablesIfNeeded()
{
    qDebug() << initDatabase();

    const QString aimTableCreate("CREATE TABLE IF NOT EXISTS aims (" //
                           "aimId integer primary key autoincrement NOT NULL,"
                           "aimName text NOT NULL,"
                           "timePart text,"//"timeAndDate text,"
                           "datePart text,"
                           "comment text,"
                           "tag text,"
                           "assignTo text NOT NULL,"

                           "priority text,"
                           "progress text,"
                           "progressText text,"
                           "parentAim text,"
                           "repeatable text,"
                           "privacy text,"
                           "duration text"

                         ");");

    QSqlQuery request = executeRequest(aimTableCreate);

    const QString activityTableCreate("CREATE TABLE IF NOT EXISTS actions (" //
                           "actId integer primary key autoincrement NOT NULL,"
                           "aimName text NOT NULL,"
                           "aimId text NOT NULL,"
                           "operation text,"
                           "moment text,"
                           "totalLength text" //helper for stops
                         ");");

    QSqlQuery request2 = executeRequest(activityTableCreate);

    const QString progressTableCreate("CREATE TABLE IF NOT EXISTS progress (" //
                           "recordId integer primary key autoincrement NOT NULL,"
                           "aimId text NOT NULL,"
                           "progress text,"
                           "progressText text"
                           "moment text"
                         ");");

    QSqlQuery request3 = executeRequest(progressTableCreate);

    const QString linksTableCreate("CREATE TABLE IF NOT EXISTS links (" //
                           "linkId integer primary key autoincrement NOT NULL,"
                           "aimId text NOT NULL,"
                           "link text,"
                           "name text"
                         ");");

    QSqlQuery request4 = executeRequest(linksTableCreate);

    return request.isValid() && request2.isValid()
            && request3.isValid() && request4.isValid();
}


bool LocalSqlBase::fillTreeModelWithAims()
{
    aimsTree.fillWithAimList(getAims());
    return false;
}


bool LocalSqlBase::fillTreeModelWithTags()
{
    tagsTree.fillWithTagList(getAllTags());
    return false;
}
