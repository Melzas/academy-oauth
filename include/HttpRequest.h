#ifndef HTTPREQUEST_H_
#define HTTPREQUEST_H_

#include <string>
#include <vector>
#include <map>

#include "ParameterList.h"

namespace OAuth
{
    class ParameterList;

    typedef std::map<std::string, std::string> header_t;
    enum HttpRequestType {GET, POST};

    extern const std::string FORM_URLENCODED;
    extern const std::string HEADER_CONTENT_TYPE;
    extern const std::string HEADER_AUTHORIZATION;

    /**
     *  Class for working with HTTP requests.
     */
    class HttpRequest
    {
    public:
        HttpRequest(HttpRequestType httpRequestType, const std::string &url);

        /**
         *  Creates string representation of request type.
         *  @returns Request type string e.g. "POST" for HttpRequestType::POST.
         */
        std::string getRequestTypeAsString() const;

        HttpRequestType getRequestType() const;

        /**
         * Set the request URL with resource and optional query parameters
         * @param url HTTP request URL
         */
        void setUrl(const std::string &url);

        /**
         * Get the complete URL with resource and query string parameters
         * @return complete URL
         */
        const std::string getUrl() const;

        /**
         * Get OAuth Base String URI from the URL
         * @returns URL without query parameters
         * e.g. "http://example.com/resource.php", if URL is "http://example.com/resource.php?q=1"
         */
        const std::string getBaseStringUri() const;

        /**
         * Add custom header to request
         * @param name Name of a header
         * @param value Value of a header
         */
        void addHeader(const std::string &name, const std::string &value);

        /**
         * Get all request headers as std::map
         * @returns Request headers as std::map
         */
        const header_t &getHeaders() const;

        /**
         * Get body content of request
         * @returns Body content
         */
        const std::string &getBody() const;

        /**
         * Set body content of request
         * @param body New value of request body
         */
        void setBody(const std::string &body);

        /**
         * Add a query string parameter
         * @param name Name of a parameter
         * @param value Value of a parameter
         */
        void addQueryParameter(const std::string &name, const std::string &value);

        /**
         * Get all query string parameters
         * @returns ParameterList with all query string parameters
         */
        const ParameterList &getQueryParameters() const;

    private:
        HttpRequestType requestType;
        std::string url;
        header_t headers;
        std::string body;
        ParameterList queryParameters;
    };

}

#endif
