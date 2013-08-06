#ifndef __SERVICE_H
#define __SERVICE_H

#include <string>
#include <functional>
#include <future>

#include "ServiceConfiguration.h"
#include "ParameterList.h"

namespace OAuth
{
    class HttpRequest;
    class Token;

    typedef std::function<std::string(const HttpRequest&)> sendRequest_t;

    class Service
    {
    public:
        static const std::string OAUTH_CONSUMER_KEY;
        static const std::string OAUTH_SIGNATURE_METHOD;
        static const std::string OAUTH_CALLBACK;
        static const std::string OAUTH_SIGNATURE;
        static const std::string OAUTH_TIMESTAMP;
        static const std::string OAUTH_NONCE;
        static const std::string OAUTH_VERSION;
        static const std::string OAUTH_TOKEN;
        static const std::string OAUTH_DEFAULT_VERSION;

    private:
        ServiceConfiguration configuration;
        sendRequest_t sendRequest;

        /**
         * Generates random string
         * @returns random string
         */
        std::string generateNonce();

    public:

        /**
         *  Creates service with specified configuration and networkWorker
         *  @param configuration initial service configuration
         *  @param sendRequest used to make requests over network.
         *  Must send provided request and return response. May throw exceptions.
         */
        Service(const ServiceConfiguration &configuration, const sendRequest_t &sendRequest);

        /**
         *  Request temporary credentials asynchronously
         *  @returns std::future object, that will return token or throw exception
         */
        std::future<Token> getRequestToken();

        void signRequest(HttpRequest &request, const Token &token);
        /**
         *  Generates url to which user should be redirected
         *  @param token temporary credentials
         *  @returns url to which user should be redirected
         */
        std::string getAuthorizeUrl(const Token &token) const;

    private:
        ParameterList generateOAuthParameters();
    };
}
#endif
