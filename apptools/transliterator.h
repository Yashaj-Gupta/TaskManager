#ifndef TRANSLITERATOR_H
#define TRANSLITERATOR_H

#include <QStringList>
#include <QString>

class Transliterator
{
public:
    Transliterator();

    void clear() { russianLetters.clear(); latinLetters.clear(); }
    void addPair(QString russian, QString latin);

    QString getLatinChar(QString russian);
    QString transliterate(QString russianText);

public:
    QStringList russianLetters;
    QStringList latinLetters;
};

#endif // TRANSLITERATOR_H
