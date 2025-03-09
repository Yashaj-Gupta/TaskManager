#include "ListOperations.h"
#include <QDebug>


namespace aim {

QStringList createListByField(const QVariantList source, const int fieldIndex)
{
    QStringList result;
    for (int i = 0; i < source.size(); ++i) {
        QStringList fields = source[i].toStringList();
        QString exactField = fields[fieldIndex];
        result << exactField;
    }
    return result;
}


QStringList createListByFieldBackwards(const QVariantList source, const int fieldIndex, const int secondField)
{
    QStringList result;
    for (int i = source.size()-1; i >= 0; --i) {
        const QStringList fields = source[i].toStringList();
        QString totalRecord = fields[fieldIndex];
        if (secondField != -1) {
            if (fields[secondField].isEmpty() == false) {
                const QStringList paFields = source[ fields[secondField].toUInt() ].toStringList();
                totalRecord += QString(" [") + paFields[fieldIndex] + QString("]") + fields[secondField] ;
            }
        }
        result << totalRecord;
    }
    return result;
}


bool areLinesSame(const QStringList &firstLine, const QStringList &secondLine)
{
    for (int i = 0; i < firstLine.size(); ++i)  {
        const QString firstString = firstLine[i].trimmed();
        const QString secondString = secondLine[i].trimmed();
        if (firstString != secondString) {
            qDebug() << "Lines don't fit "<<firstString<<" "<<secondString;
            return false;
        }
    }
    return true;
}


void compareLists(const QVariantList &firstList, const QVariantList &secondList,
                  QVariantList &toInsert, QVariantList &toEdit, QVariantList &toDelete)
{

    for (int i =0; i < firstList.size(); ++i) {
        bool wasFound = false;
        const QStringList firstLine = firstList[i].toStringList();
        const QString firstCode = firstLine[0];

        for (int j = 0; j < secondList.size(); ++j) {
            const QStringList secondLine = secondList[j].toStringList();
            const QString secondCode = secondLine[0];

            if (firstCode == secondCode) {
                wasFound = true;
                if (areLinesSame(firstLine, secondLine) == false) {
                    toEdit << secondLine;
                    break;
                }
            }
        }
        if (wasFound == false)
            toDelete << firstLine;
    }

     for (int i = 0; i < secondList.size(); ++i) {
         bool wasFound = false;
         const QStringList secondLine = secondList[i].toStringList();
         const QString secondCode = secondLine[0];

         for (int j = 0; j < firstList.size(); ++j) {
                QStringList firstLine = firstList[j].toStringList();
                QString firstCode = firstLine[0];

                if (firstCode == secondCode) {
                    wasFound = true;
                    break;
                }
         }
         if (wasFound == false)
             toInsert << secondLine;
     }
}


}
