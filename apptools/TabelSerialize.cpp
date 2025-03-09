#include "TabelSerialize.h"

using namespace apptools;


void TableSerialize::fromRawString(QString source) {
    if (source.size())
        if (source[source.size()-1]=='\n')
        source = source.mid(0,source.size()-1);

    QStringList lines = source.split(QString("\n"));
    int number = 0;
    while (number < lines.count()) {

        int position = 0;
        QString lineData = lines[number].mid(position); //there was trimmed
        if (lineData.isEmpty())
            lineData = " "; //fix

        if (!lineData.isEmpty()) {
            QStringList columnStrings = lineData.split("\t",QString::SkipEmptyParts);
            for (int i = 0; i < columnStrings.size(); ++i) {
                QString currentString = columnStrings[i];

                if (currentString.size())
                if (currentString[currentString.size()-1] =='\n')
                    currentString = currentString.mid(0,currentString.size()-1);
                columnStrings[i] = currentString;
            }
            _tableRows << columnStrings;
        }
        ++number;
    }
}


void TableSerialize::fromString(const QString source) {
    _tableRows.clear();
    QByteArray sourceArray = source.toLocal8Bit();
    QByteArray decoded = QByteArray::fromBase64(sourceArray);
    QString decodedString = QString(decoded);
    fromRawString(decodedString);
}


QString TableSerialize:: toString() const
{
    QString response;
    QString sepparator(4,'\t');

    for (int i = 0; i < _tableRows.size(); ++i) {
        QStringList currentRow = _tableRows[i];
        for (int j = 0; j < currentRow.size(); ++j) {
            response += currentRow[j];
            if (j != currentRow.size()-1)
                response += sepparator;
        }
        response += "\n";
    }

    QByteArray array = response.toUtf8();
    QByteArray base64Array = array.toBase64();
    QString codedResponse = base64Array;
    return codedResponse;
}


void TableSerialize::addRow(const QStringList oneRow) {
    _tableRows << oneRow;
}


QStringList TableSerialize::getRow(int index) const {
    return _tableRows[index];
}


int TableSerialize::size() const {
    return _tableRows.size();
}


QString TableSerialize:: toRawString() const {
    QString response;
    QString sepparator(4,'\t');

    for (int i = 0; i < _tableRows.size(); ++i) {
        QStringList currentRow = _tableRows[i];
        for (int j = 0; j < currentRow.size(); ++j) {
            if (currentRow[j].isEmpty() == false)
                response += currentRow[j];
            else
                response += " ";

            if (j != currentRow.size()-1)
                response += sepparator;
        }
        response += "\r\n";
    }
    return response;
}
