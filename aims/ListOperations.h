#ifndef LISTOPERATIONS_H
#define LISTOPERATIONS_H

#include <QStringList>
#include <QVariantList>

namespace aim {

    QStringList createListByField(const QVariantList source, const int fieldIndex);

    QStringList createListByFieldBackwards(const QVariantList source, const int fieldIndex, const int secondField=-1);

    void compareLists(const QVariantList &firstList, const QVariantList &secondList,
                      QVariantList &toInsert, QVariantList &toEdit, QVariantList &toDelete);
}


#endif // LISTOPERATIONS_H
