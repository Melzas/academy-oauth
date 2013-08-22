#ifndef __URL_H
#define __URL_H

#include <string>
#include <sstream>

namespace OAuth
{
    namespace Utility
    {
        /**
         *  Performs percent encoding of string
         *  @param data string to encode
         *  @returns encoded string
         */
        std::string urlEncode(const std::string &data);
        /**
         *  Decodes percent encoded string
         *  @param data string to decode
         *  @returns decoded string
         */
        std::string urlDecode(const std::string &data);

        /**
         * Extracts host from url
         * @param url url, e.g. http://www.example.com/test
         * @returns host, e.g. www.example.com
         */
        std::string hostFromUrl(const std::string &url);
        /**
         * Extracts resource from url
         * @param url url, e.g. http://www.example.com/test
         * @returns resource, e.g. /test
         */
        std::string resourceFromUrl(const std::string &url);

        /**
         * Extracts parameters from url after a separator
         * @param url Url with query string or fragment
         * @param separator Parameters separator
         * @returns Parameters, e.g. q1=f
         */
        std::string extractParameters(const std::string &url, const char separator);

        /**
         * Normalizes url
         * e.g http://www.example.com, www.example.com/, www.example.com
         * will result in http://www.example.com/
         * @param url url to normalize
         * @returns normalized url
         */
        std::string normalizeUrl(std::string url);
        /**
         * Converts long long to string. Needed because of gcc bug:
         * Bug 52015 - std::to_string does not work under MinGW
         */
        template<typename T> std::string toString(T x)
        {
            std::stringstream stringStream;
            stringStream<<x;
            return stringStream.str();
        }
    }
}

#endif
