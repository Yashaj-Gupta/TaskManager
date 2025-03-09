#ifndef HEADER_JSON
#define HEADER_JSON

#include <string>
#include <vector>
#include <map>

class TinyJSON
{
protected:
    std::map<std::string,std::string> inside;

	void checkStringQuotes(std::string &checkString);

public:
    void addField(std::string fieldName, std::string fieldValue);
    void addField(std::string fieldName, int fieldValue);

    std::string toString();
    void clear();

    bool parse(const std::string &jsonString, int startFrom=0);
    std::string getValue(std::string fieldName);
    int getIntValue(std::string fieldName);

    int getLastParsedPosition() { return lastParsedPosition; }

    int size() { return inside.size(); }

    void joinWith(TinyJSON &another)
    {
        for (std::map<std::string,std::string>::iterator itIn = another.inside.begin();
             itIn != another.inside.end(); ++itIn)
        {
            addField(itIn->first,itIn->second);
        }
    }

protected:

    int lastParsedPosition;

    std::string extractName(const std::string &jsonString, int &position);
    std::string extractValue(const std::string &jsonString, int &position);
};



#endif
