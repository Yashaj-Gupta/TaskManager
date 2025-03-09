#ifndef TABELSERIALIZE_H
#define TABELSERIALIZE_H

#include <QVector>
#include <QStringList>

namespace apptools {

    class TableSerialize
    {
    public:

        void addRow(const QStringList oneRow);
        QStringList getRow(int index) const;

        int size() const;

        void fromString(const QString source);
        void fromRawString(QString source);

        QString toString() const;
        QString toRawString() const;

    protected:
        QVector<QStringList> _tableRows;

    };

}


#endif // TABELSERIALIZE_H
