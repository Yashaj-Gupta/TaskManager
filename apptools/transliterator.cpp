#include "transliterator.h"

Transliterator::Transliterator()
{
    addPair("А","a"); //full cover with that function also
    addPair("Б","b");
    addPair("В","v");
    addPair("Г","g");
    addPair("Д","d");
    addPair("Е","e");
    addPair("Ё","jo");
    addPair("Ж","zh");
    addPair("З","z");
    addPair("И","i");
    addPair("Й","j");
    addPair("К","k");
    addPair("Л","l");
    addPair("М","m");
    addPair("Н","n");
    addPair("О","o");
    addPair("П","p");
    addPair("Р","r");
    addPair("С","s");
    addPair("Т","t");
    addPair("У","u");
    addPair("Ф","f");
    addPair("Х","h");
    addPair("Ц","c");
    addPair("Ч","ch");
    addPair("Ш","sh");
    addPair("Щ","sch");
    addPair("Ы","y");
    addPair("Э","e");
    addPair("Ю","yu");
    addPair("Я","ya");
}


void Transliterator::addPair(QString russian, QString latin)
{
    russianLetters << russian;
    latinLetters << latin;
}

QString Transliterator::getLatinChar(QString russian)
{
    for (int i = 0; i < russianLetters.size(); ++i)
    {
        if (russian == russianLetters[i])
            return latinLetters[i];
    }
    return "";
}

QString Transliterator::transliterate(QString russianText)
{
    QString result;

    for (int i = 0; i < russianText.size(); ++i)
    {
        QString letter = russianText.mid(i,1); //not that fine way..
        result += getLatinChar(letter);
    }

    return result;
}
