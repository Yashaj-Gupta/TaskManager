#ifndef DATEOPERATIONS_H
#define DATEOPERATIONS_H

#include <QDate>
#include <QStringList>

class DateOperations
{
public:

    static QDate addWorkDays(QDate date, int days);
    static QString daysText(int days);

    static QDate shiftToWorkDay(QDate date);

    static QStringList holydaysList;
    static void setHolydays(QStringList newList);

protected:

    static bool isHoliday(QDate date);
};

#endif // DATEOPERATIONS_H
