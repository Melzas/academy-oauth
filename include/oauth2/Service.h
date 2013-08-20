#ifndef O2SERVICE_H_
#define O2SERVICE_H_

#include <string>
#include <functional>
#include <future>

#include "ServiceConfiguration.h"
#include "ParameterList.h"
#include "TokenException.h"

namespace OAuth
{
    class HttpRequest;
}

namespace OAuth2
{
    class Token;

    typedef std::function<std::string(const OAuth::HttpRequest&)> send_request_t;

    class Service
    {
        static const std::string RESPONSE_TYPE;
        static const std::string CLIENT_ID;
        static const std::string REDIRECT_URI;
        static const std::string SCOPE;
        static const std::string STATE;
        static const std::string ACCESS_TOKEN;
        static const std::string TOKEN_TYPE;
        static const std::string EXPIRES_IN;
        static const std::string REFRESH_TOKEN;
        static const std::string GRANT_TYPE;
        static const std::string AUTHORIZATION_CODE;
        static const std::string CODE;
        static const std::string PASSWORD;
        static const std::string USERNAME;
        static const std::string CLIENT_CREDENTIALS;

        static const std::string ERROR;
        static const std::string ERROR_DESCRIPTION;
        static const std::string ERROR_URI;

    public:
        /**
         *  Get url, to which user should be redirected.
         *  @param state State string, that will be added to url.
         *  If state is empty, it will not be added.
         */
        std::string getAuthorizeUrl(const std::string &state = "") const;

        /**
         *  Get access token using way, specified by configuration.
         *  @param url Url, at which user was redirected back (if applicable).
         */
        std::future<Token> getAccessToken(const std::string &url = "") const;

        /*
         *  Sign HttpRequest using token.
         *  @param request Request to sign.
         *  @param token Token, used to sign request.
         */
        void signRequest(OAuth::HttpRequest &request, const Token &token) const;

        /**
         *  Refresh access token.
         *  @param expiredToken Old token.
         */
        std::future<Token> refreshAccessToken(const Token &expiredToken) const;

    private:
        friend class ServiceBuilder;
        /**
         *  Create Service with specified configuration.
         *  @param configuration Configuration, that will be used.
         *  @param sendRequest used to make requests over network.
         *  Must send provided request and return response. May throw exceptions.
         */
        Service(const ServiceConfiguration &configuration, const send_request_t &sendRequest);

        ServiceConfiguration configuration;
        send_request_t sendRequest;

        std::future<Token> getAccessTokenAuthCodeGrant(const std::string &url) const;
        std::future<Token> getAccessTokenImplicitGrant(const std::string &url) const;
        std::future<Token> getAccessTokenOwnerCredentialsGrant() const;
        std::future<Token> getAccessTokenClientCredentialsGrant() const;

        std::string makeRedirectUrlParameters(const std::string &state, const std::string &responseType) const;

        TokenException makeTokenError(const OAuth::ParameterList &parameters) const;
        Token parseTokenResponse(const OAuth::ParameterList &parameters) const;
    };
}

#endif
