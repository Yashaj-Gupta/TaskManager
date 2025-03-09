#include "JSON.h"
#include "Logs.h"

#include <stdlib.h> //atoi

#include <sstream>

void TinyJSON::addField(std::string fieldName, std::string fieldValue)
{
    checkStringQuotes(fieldValue);
    inside[fieldName] = fieldValue;
}

void TinyJSON::addField(std::string fieldName, int fieldValue)
{
    std::stringstream streambuffer;
    streambuffer << fieldValue;
    std::string fieldString = streambuffer.str();

    inside[fieldName] = fieldString;
}

std::string TinyJSON::toString()
{
	std::string jsonString = "{";

    int elementNumber = 0;
    int lastElement = inside.size()-1;

    for (std::map<std::string,std::string>::iterator it = inside.begin();
         it != inside.end(); ++it)
    {
        std::string fieldName =  it->first;
        checkStringQuotes(fieldName);

        std::string inString = fieldName + std::string (":") + it->second;
        jsonString += inString;

            if (elementNumber++ != lastElement) //std::next isn't everywhere =(
        jsonString += std::string(",\n");
    }

	jsonString += "}";
	
	return jsonString;	
}

void TinyJSON::clear()
{
	inside.clear();
    inside.clear();
}

void TinyJSON::checkStringQuotes(std::string &checkString)
{
	if (checkString.empty())
	{
		checkString ="\"\"";
		return;
	}

	if (checkString[0] != '"')
		checkString = std::string("\"") + checkString;
	if (checkString[checkString.size()-1] != '"')
		checkString += std::string("\"");
}

bool TinyJSON::parse(const std::string &jsonString, int startFrom)
{
        size_t startPosition = jsonString.find("{",startFrom);

        if (startPosition == std::string::npos)
            return false; //not a JSON

        int currentPosition = startPosition;
        int lastNamePosition = 0;

        while (currentPosition != -1)
        {
            std::string fieldName = extractName(jsonString,currentPosition);

            if (fieldName.empty())
                return false;

            lastNamePosition = currentPosition;
            std::string fieldValue = extractValue(jsonString,currentPosition);

            if (currentPosition==-1)
                lastParsedPosition = lastNamePosition + fieldValue.size();

            //debug output:
            //std::cout << "Field: "<<fieldName.c_str()<<" ; "<<fieldValue.c_str()<<"\n";
            inside[fieldName] = fieldValue;
        }

        return true;
}

std::string TinyJSON::getValue(std::string fieldName)
{
    std::map<std::string,std::string>::iterator it = inside.find(fieldName);

    if (it != inside.end())
    {
        std::string foundValue = it->second;

        if (foundValue[foundValue.size()-1]=='"')
            foundValue.erase(foundValue.size()-1);
        if (foundValue[0]=='"')
            foundValue.erase(0,1);

        return foundValue;
    }

    return "";
}

int TinyJSON::getIntValue(std::string fieldName)
{
    std::map<std::string,std::string>::iterator it = inside.find(fieldName);

    if (it != inside.end())
    {
        return atoi(it->second.c_str());
    }

    return -1;
}


std::string TinyJSON::extractName(const std::string &jsonString, int &position)
{
    size_t openBrack = jsonString.find("\"",position);
    size_t closedBrack = jsonString.find("\"",openBrack+1);

    std::string foundName = "";

    if (closedBrack - openBrack > 2)
        foundName = jsonString.substr(openBrack+1,closedBrack-openBrack-1);

    position = closedBrack;

    return foundName;
}

std::string TinyJSON::extractValue(const std::string &jsonString, int &position)
{
    size_t valueBegin = jsonString.find(":",position);

    int localPosition = valueBegin+1;

    bool quotesOpen = false;
    int brackLevel = 0;
    int rectBrackLevel =0;

    int stringLength = jsonString.size();

    while (localPosition < stringLength)
    {
        if (jsonString[localPosition] == '"')
            quotesOpen = !quotesOpen;

        if (jsonString[localPosition] == '{')
                ++brackLevel;
        if (jsonString[localPosition] == '}')
                --brackLevel;

        if (jsonString[localPosition] == '[')
                ++rectBrackLevel;
        if (jsonString[localPosition] == ']')
                --rectBrackLevel;

        if (brackLevel == -1)
        {
            //last value in the json file
            std::string foundValue = jsonString.substr(valueBegin+1, localPosition-valueBegin-1);
            position = -1;
            return foundValue;
        }

        if (jsonString[localPosition] == ',')
        {
            if ((quotesOpen == false) && (brackLevel == 0) && (rectBrackLevel == 0))
            {
                std::string foundValue = jsonString.substr(valueBegin+1, localPosition-valueBegin-1);
                position = localPosition;
                return foundValue;
            }
        }

        ++localPosition;
    }

}
