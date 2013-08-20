#ifndef PARAMETERLIST_H_
#define PARAMETERLIST_H_

#include <vector>
#include <map>
#include <string>

namespace OAuth
{
    typedef std::pair<std::string, std::string> StringPair;
    typedef std::vector<StringPair> parameters_t;

    /**
     *  Class for working with POST/GET parameters
     */
    class ParameterList
    {
    public:
        static const char QUERY_SEPARATOR;
        static const char PARAMETER_SEPARATOR;
        static const char PAIR_SEPARATOR;

    public:
        ParameterList();
        /**
         *  Creates parameter list from query string
         *  @param queryString Query string
         */
        ParameterList(const std::string& queryString);

        /**
         *  Adds percent-encoded parameter to list
         *  @param name Name part of "name=value" pair
         *  @param value Value part of "name=value" pair
         */
        void add(const std::string &name, const std::string &value);
        /**
         *  Adds not encoded parameter to list
         *  @param name Name part of "name=value" pair
         *  @param value Value part of "name=value" pair
         */
        void addRaw(const std::string &name, const std::string &value);

        /**
         * Add all parameters from another ParameterList
         * @param other Another ParameterList
         */
        void add(const ParameterList &other);

        /**
         * Adds parameters to list from query string
         * @param queryString Query string
         */
        void addQueryString(const std::string &queryString);

        /**
         * Constructs query string from parameter list
         * @returns Query string
         */
        const std::string asQueryString() const;

        /**
         * Get parameters as a vector of string pairs
         * @returns Vector with parameters
         */
        const parameters_t &getParameters() const;

        /**
         *  Get value of parameter with specified name.
         *  @param name Parameter name.
         *  @throws std::invalid_argument if parameter does not exist.
         *  @returns Parameter value.
         */
        const std::string &getFirst(const std::string &name) const;

        /**
         *  Check, if list contains parameter with specified name.
         *  @param name Parameter name.
         *  @return True if parameter exists, false otherwise.
         */
        bool contain(const std::string &name) const;

    private:
        parameters_t parameters;
    };

}
#endif
