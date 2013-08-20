#include <oauth2/Service.h>

#include <string>
#include <stdexcept>
#include <functional>
#include <future>
#include <sstream>

#include <oauth2/Token.h>
#include <oauth2/TokenException.h>
#include <utility/Url.h>
#include <utility/SingleLevelJson.h>
#include <DefaultSendRequest.h>
#include <ParameterList.h>

namespace OAuth2
{
    const std::string Service::RESPONSE_TYPE = "response_type";
    const std::string Service::CLIENT_ID = "client_id";
    const std::string Service::REDIRECT_URI = "redirect_uri";
    const std::string Service::SCOPE = "scope";
    const std::string Service::STATE = "state";
    const std::string Service::ACCESS_TOKEN= "access_token";
    const std::string Service::TOKEN_TYPE= "token_type";
    const std::string Service::EXPIRES_IN= "expires_in";
    const std::string Service::REFRESH_TOKEN= "refresh_token";
    const std::string Service::GRANT_TYPE = "grant_type";
    const std::string Service::AUTHORIZATION_CODE = "authorization_code";
    const std::string Service::CODE = "code";
    const std::string Service::PASSWORD = "password";
    const std::string Service::USERNAME = "username";
    const std::string Service::CLIENT_CREDENTIALS = "client_credentials";

    const std::string Service::ERROR = "error";
    const std::string Service::ERROR_DESCRIPTION = "error_description";
    const std::string Service::ERROR_URI = "error_uri";

    Service::Service(const ServiceConfiguration &configuration, const send_request_t &sendRequest) :
        configuration(configuration)
        ,sendRequest(sendRequest)
    {
    }

    std::string Service::getAuthorizeUrl(const std::string &state) const
    {
        std::string authorizeUrl = configuration.getAuthEndpoint();

        if (authorizeUrl.find(OAuth::ParameterList::QUERY_SEPARATOR) == std::string::npos)
            authorizeUrl += OAuth::ParameterList::QUERY_SEPARATOR;
        else
            authorizeUrl += OAuth::ParameterList::PARAMETER_SEPARATOR;

        switch (configuration.getGrantType()) {
        case AUTH_CODE_GRANT:
        case IMPLICIT_GRANT:
            authorizeUrl += makeRedirectUrlParameters(state,  configuration.getResponseType());
            break;
        default:
            throw std::logic_error("getAuthorizeUrl does not support specified grant type");
        }

        return authorizeUrl;
    }

    std::string Service::makeRedirectUrlParameters(const std::string &state,
            const std::string &responseType) const
    {
        OAuth::ParameterList parameters;
        parameters.add(RESPONSE_TYPE, responseType);
        parameters.add(CLIENT_ID, configuration.getClientId());
        if (!configuration.getRedirectUri().empty())
            parameters.add(REDIRECT_URI, configuration.getRedirectUri());
        if (!configuration.getScope().empty())
            parameters.add(SCOPE, configuration.getScope());
        if (!state.empty())
            parameters.add(STATE, state);

        return parameters.asQueryString().substr(1);
    }

    std::future<Token> Service::getAccessToken(const std::string &url) const
    {
        switch (configuration.getGrantType()) {
        case AUTH_CODE_GRANT:
            return getAccessTokenAuthCodeGrant(url);
        case IMPLICIT_GRANT:
            return getAccessTokenImplicitGrant(url);
        case OWNER_CREDENTIALS_GRANT:
            return getAccessTokenOwnerCredentialsGrant();
        case CLIENT_CREDENTIALS_GRANT:
            return getAccessTokenClientCredentialsGrant();
        default:
            throw std::logic_error("getAccessToken does not support specified grant type");
        }
    }

    std::future<Token> Service::getAccessTokenAuthCodeGrant(const std::string &url) const
    {
        const std::string parametersString = OAuth::Utility::extractQueryParameters(url);
        const OAuth::ParameterList parameters(parametersString);

        if (parameters.contain(ERROR)) {
            throw makeTokenError(parameters);
        } else {
            const std::string code = parameters.getFirst(CODE);

            OAuth::HttpRequest request(OAuth::HttpRequestType::POST,
                configuration.getTokenEndpoint());
            request.addHeader(OAuth::HEADER_CONTENT_TYPE, OAuth::FORM_URLENCODED);
            OAuth::ParameterList body;
            body.addRaw(GRANT_TYPE, AUTHORIZATION_CODE);
            body.addRaw(CODE, code);
            if (!configuration.getRedirectUri().empty())
                body.addRaw("redirect_uri", configuration.getRedirectUri());
            if (!configuration.getClientId().empty())
                body.addRaw("client_id", configuration.getClientId());
            request.setBody(body.asQueryString().substr(1));

            return std::async([=] () {
                return parseTokenResponse(Utility::parseSingleLevelJSON(sendRequest(request)));
            });
        }
    }

    std::future<Token> Service::getAccessTokenImplicitGrant(const std::string &url) const
    {
        const std::string parametersString = OAuth::Utility::extractQueryParameters(url);
        const OAuth::ParameterList parameters(parametersString);

        return std::async([=] () {
            return parseTokenResponse(parameters);
        });
    }

    std::future<Token> Service::getAccessTokenOwnerCredentialsGrant() const
    {
        OAuth::HttpRequest request(OAuth::HttpRequestType::POST,
            configuration.getTokenEndpoint());
        request.addHeader(OAuth::HEADER_CONTENT_TYPE, OAuth::FORM_URLENCODED);
        OAuth::ParameterList body;
        body.addRaw(GRANT_TYPE, PASSWORD);
        body.addRaw(USERNAME, configuration.getUsername());
        body.addRaw(PASSWORD, configuration.getPassword());
        request.setBody(body.asQueryString().substr(1));

        return std::async([=] () {
            return parseTokenResponse(Utility::parseSingleLevelJSON(sendRequest(request)));
        });
    }

    std::future<Token> Service::getAccessTokenClientCredentialsGrant() const
    {
        OAuth::HttpRequest request(OAuth::HttpRequestType::POST,
            configuration.getTokenEndpoint());
        request.addHeader(OAuth::HEADER_CONTENT_TYPE, OAuth::FORM_URLENCODED);
        OAuth::ParameterList body;
        body.addRaw(GRANT_TYPE, CLIENT_CREDENTIALS);
        request.setBody(body.asQueryString().substr(1));

        return std::async([=] () {
            return parseTokenResponse(Utility::parseSingleLevelJSON(sendRequest(request)));
        });
    }

    void Service::signRequest(OAuth::HttpRequest &request, const Token &token) const
    {
        if (token.getTokenType() == Token::BEARER_TOKEN)
            request.addHeader(OAuth::HEADER_AUTHORIZATION, "Bearer " + token.getAccessToken());
        else
            throw std::logic_error("Unsupported token type " + token.getTokenType());
    }

    std::future<Token> Service::refreshAccessToken(const Token &expiredToken) const
    {
        OAuth::HttpRequest request(OAuth::HttpRequestType::POST,
            configuration.getTokenEndpoint());
        request.addHeader(OAuth::HEADER_CONTENT_TYPE, OAuth::FORM_URLENCODED);
        OAuth::ParameterList body;
        body.addRaw(GRANT_TYPE, REFRESH_TOKEN);
        body.addRaw(REFRESH_TOKEN, expiredToken.getRefreshToken());
        request.setBody(body.asQueryString().substr(1));

        return std::async([=] () {
            return parseTokenResponse(Utility::parseSingleLevelJSON(sendRequest(request)));
        });
    }

    Token Service::parseTokenResponse(const OAuth::ParameterList &parameters) const
    {
        if (parameters.contain(ERROR)) {
            throw makeTokenError(parameters);
        } else {
            if (!parameters.contain(ACCESS_TOKEN) || !parameters.contain(TOKEN_TYPE))
                throw std::invalid_argument("Token parsing failed. Required parameter not set");
            std::string accessToken = parameters.getFirst(ACCESS_TOKEN);
            std::string tokenType = parameters.getFirst(TOKEN_TYPE);

            int expiresIn = Token::EXPIRES_UNDEFINED;
            if (parameters.contain(EXPIRES_IN)) {
                std::istringstream expiresInStream(parameters.getFirst(EXPIRES_IN));
                expiresInStream>>expiresIn;
            }

            std::string refreshToken = Token::REFRESH_UNDEFINED;
            if (parameters.contain(REFRESH_TOKEN))
                refreshToken = parameters.getFirst(REFRESH_TOKEN);

            return Token(accessToken, expiresIn, refreshToken, tokenType);
        }
    }

    TokenException Service::makeTokenError(const OAuth::ParameterList &parameters) const
    {
        std::string error = parameters.getFirst(ERROR);

        std::string description;
        if (parameters.contain(ERROR_DESCRIPTION))
            description = parameters.getFirst(ERROR_DESCRIPTION);

        std::string uri;
        if (parameters.contain(ERROR_URI))
            uri= parameters.getFirst(ERROR_URI);

        return TokenException(error, description, uri);
    }
}
