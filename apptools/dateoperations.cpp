#include "dateoperations.h"

QStringList DateOperations::holydaysList; //OMG how long I didn't use them

QDate DateOperations::addWorkDays(QDate date,int days)
{
    QDate startDate = date;
    QDate endDate = date.addDays(days);

    if (startDate > endDate)
    {
        //qDebug() << "Issue in work "<<date<<" "<<days;
        return startDate;
    }

    for (QDate dateIteration = startDate; dateIteration <= endDate; dateIteration = dateIteration.addDays(1))
    {
        int weekDay = dateIteration.dayOfWeek();

        if (weekDay >= 6)
            endDate = endDate.addDays(1);

        if (isHoliday(dateIteration))
            endDate = endDate.addDays(1);
    }

    return endDate;
}


QDate DateOperations::shiftToWorkDay(QDate date)
{
    QDate startDate = date;
    while (1)
    {
        int weekDay = startDate.dayOfWeek();
        if (isHoliday(startDate) || (weekDay >= 6))
            startDate = startDate.addDays(1);
        else
            break;
    }
    return startDate;
}

void DateOperations::setHolydays(QStringList newList)
{
    holydaysList = newList;
}


bool DateOperations::isHoliday(QDate date)
{
    //hardcoded version first, then maybe talk, maybe just leave them
    //to figure it out by themselfs :)

    QString dateString = date.toString("yyyy-MM-dd");

    //will wipe out soon
    if (dateString=="2017-02-23")
            return true;
    if (dateString=="2017-02-24")
            return true;
    if (dateString=="2017-03-08")
            return true;
    if (dateString=="2017-05-01")
            return true;
    if (dateString=="2017-05-08")
            return true;
    if (dateString=="2017-05-09")
            return true;
    if (dateString=="2017-06-12")
            return true;
    if (dateString=="2017-11-06")
            return true;

    for (int i = 0; i < holydaysList.size(); ++i)
    {
        if (dateString == holydaysList[i])
            return true;
    }

    //should never store here sundays or saturdays

    return false;
}

QString DateOperations::daysText(int days)
{
    QString response = QString::number(days);

    if (days >= 5 && days <= 20)
        response += " дней";
    else if (days < 5)
    {
        if (days == 1)
            response += " день";
        else if (days== 0)
            response += " дней";
        else
            response += " дня";
    }
    else
    {
        int daysPart = days % 10;

        if (daysPart==1)
            response += " день";
        else if (daysPart >= 2 && daysPart <= 4)
            response += " дня";
        else
            response += " дней";

    }

    return response;
}
