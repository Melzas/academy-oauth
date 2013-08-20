#include "SingleLevelJSON.h"

#include <exception>
#include <string>
#include <sstream>

namespace Utility
{
    std::string trimBracesIfExist(const std::string &data)
    {
        int openingBracePosition = 0;
        int closingBracePosition = data.length() - 1;

        while (openingBracePosition < static_cast<int>(data.length()) &&
                isspace(data[openingBracePosition]))
            ++openingBracePosition;
        if (openingBracePosition == static_cast<int>(data.length()))
            return data;
        while (isspace(data[closingBracePosition]))
            --closingBracePosition;

        int remainingLength = closingBracePosition - openingBracePosition - 1;

        if (data[openingBracePosition] != '{' && data[closingBracePosition] != '}')
            return data;
        if (data[openingBracePosition] == '{' && data[closingBracePosition] == '}')
            return data.substr(openingBracePosition + 1, remainingLength);
        throw std::invalid_argument("JSON parsing failed, braces do not match");
    }

    char decodeEscaped(std::istringstream &dataStream)
    {
        char currentCharacter;
        dataStream>>currentCharacter;

        switch (currentCharacter) {
        case '"':
        case '\\':
        case '/':
            return currentCharacter;
        case 'b':
            return '\b';
        case 'f':
            return '\f';
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        case 'u':
            throw std::invalid_argument("JSON parsing failed, \\uXXXX format is not supported");
        }

        throw std::invalid_argument("JSON parsing failed, unknown escaped character");
    }

    void skipWhitespaces(std::istringstream &dataStream)
    {
        while (isspace(dataStream.peek()))
            dataStream.ignore(1);
    }

    bool isValidUnquotedCharacter(char character)
    {
        std::string allowed = "0123456789eE+-.";
        return allowed.find(character) != std::string::npos;
    }

    std::string readUnquoted(std::istringstream &dataStream) {
        char character;
        std::ostringstream value;

        while (dataStream>>character && isValidUnquotedCharacter(character))
            value<<character;
        dataStream.unget();

        if (value.str() == "" || dataStream.eof())
            throw std::invalid_argument("JSON parsing failed");
        return value.str();
    }

    std::string readQuoted(std::istringstream &dataStream) {
        char character;
        std::ostringstream value;

        dataStream.ignore(1, '"');
        while (dataStream>>character && character != '"') {
            if (character == '\\')
                value<<decodeEscaped(dataStream);
            else
                value<<character;
        }

        if (dataStream.eof())
            throw std::invalid_argument("JSON parsing failed");
        return value.str();
    }

    std::string readPairElement(std::istringstream &dataStream) {
        if (dataStream.peek() == '"')
            return readQuoted(dataStream);
        return readUnquoted(dataStream);
    }

    OAuth::ParameterList parseSingleLevelJSON(std::string data)
    {
        OAuth::ParameterList parsedData;

        data = trimBracesIfExist(data) + ",";
        std::istringstream dataStream(data);

        skipWhitespaces(dataStream);
        while (!dataStream.eof()) {
            std::string name = readPairElement(dataStream);
            if (name.empty())
                throw std::invalid_argument("JSON parsing failed");

            skipWhitespaces(dataStream);
            char character;
            dataStream>>character;
            if (character!=':')
                throw std::invalid_argument("JSON parsing failed");

            skipWhitespaces(dataStream);
            parsedData.addRaw(name, readPairElement(dataStream));

            skipWhitespaces(dataStream);
            dataStream>>character;
            if (character!=',')
                throw std::invalid_argument("JSON parsing failed");

            skipWhitespaces(dataStream);
        }
        return parsedData;
    }
}
