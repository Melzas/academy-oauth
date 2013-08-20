#include <ParameterList.h>

#include <sstream>
#include <set>
#include <algorithm>
#include <stdexcept>

#include "utility/Url.h"
#include "utility/Extractor.h"

namespace OAuth
{
    const char ParameterList::QUERY_SEPARATOR = '?';
    const char ParameterList::PARAMETER_SEPARATOR = '&';
    const char ParameterList::PAIR_SEPARATOR = '=';

    ParameterList::ParameterList()
    {
    }

    ParameterList::ParameterList(const std::string& queryString)
    {
        this->addQueryString(queryString);
    }

    void ParameterList::add(const std::string &name, const std::string &value)
    {
        std::string decodedName = Utility::urlDecode(name);
        std::string decodedValue = Utility::urlDecode(value);
        parameters.push_back(std::make_pair(decodedName, decodedValue));
    }

    void ParameterList::addRaw(const std::string &name, const std::string &value)
    {
        parameters.push_back(std::make_pair(name, value));
    }

    void ParameterList::add(const ParameterList &other)
    {
        this->addQueryString(other.asQueryString());
    }

    void ParameterList::addQueryString(const std::string &queryString)
    {
        if(queryString.empty()) {
            return;
        }
        std::string query = (queryString[0] == QUERY_SEPARATOR)
                ? queryString.substr(1) : queryString;
        std::stringstream queryStream(query);
        std::string pairString;
        while (std::getline(queryStream, pairString, PARAMETER_SEPARATOR)) {
            std::stringstream pairStream(pairString);
            std::string name;
            std::string value;
            std::getline(pairStream, name, PAIR_SEPARATOR);
            std::getline(pairStream, value, PAIR_SEPARATOR);
            this->add(name, value);
        }
    }

    const std::string ParameterList::asQueryString() const
    {
        std::string queryString(1, QUERY_SEPARATOR);
        if(parameters.empty()) {
            return queryString;
        }

        for (auto pair : parameters) {
            StringPair encodedPair = Utility::urlEncodedPair(pair.first,pair.second);
            queryString += encodedPair.first + PAIR_SEPARATOR + encodedPair.second;
            queryString += PARAMETER_SEPARATOR;
        }
        return queryString.substr(0, queryString.length() - 1);
    }

    const parameters_t &ParameterList::getParameters() const
    {
        return parameters;
    }

    const std::string &ParameterList::getFirst(const std::string &name) const
    {
        const auto iterator =
                std::find_if(parameters.begin(), parameters.end(), [name] (const StringPair &pair) {
                    return pair.first == name;
                }
            );
        if (iterator == parameters.end())
            throw std::invalid_argument("ParameterList does not contain element with name \""+ name +"\"");
        return  iterator -> second;
    }

    bool ParameterList::contain(const std::string &name) const
    {
        return std::find_if(parameters.begin(), parameters.end(), [name] (const StringPair &pair) {
                    return pair.first == name;
                }
            ) != parameters.end();
    }
}
