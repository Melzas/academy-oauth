#include <oauth2/ServiceBuilder.h>

#include <stdexcept>

#include "DefaultSendRequest.h"

namespace OAuth2
{
    ServiceBuilder::ServiceBuilder()
    {
        grantType = AUTH_CODE_GRANT;
        sendRequestFunction = defaultSendRequest;
    }

    ServiceBuilder &ServiceBuilder::setGrantType(GrantType grantType)
    {
        this->grantType = grantType;
        return *this;
    }

    ServiceBuilder &ServiceBuilder::setAuthEndpoint(const std::string &authEndpoint)
    {
        this->authEndpoint = authEndpoint;
        return *this;
    }

    ServiceBuilder &ServiceBuilder::setTokenEndpoint(const std::string &tokenEndpoint)
    {
        this->tokenEndpoint = tokenEndpoint;
        return *this;
    }

    ServiceBuilder &ServiceBuilder::setClientId(const std::string &clientId)
    {
        this->clientId = clientId;
        return *this;
    }

    ServiceBuilder &ServiceBuilder::setRedirectUri(const std::string &redirectUri)
    {
        this->redirectUri = redirectUri;
        return *this;
    }

    ServiceBuilder &ServiceBuilder::setScope(const std::string &scope)
    {
        this->scope = scope;
        return *this;
    }

    ServiceBuilder &ServiceBuilder::setUsername(const std::string &username)
    {
        this->username = username;
        return *this;
    }

    ServiceBuilder &ServiceBuilder::setPassword(const std::string &password)
    {
        this->password = password;
        return *this;
    }

    ServiceBuilder &ServiceBuilder::setSendRequest(const send_request_t &sendRequestFunction)
    {
        this->sendRequestFunction = sendRequestFunction;
        return *this;
    }

    Service ServiceBuilder::build()
    {
        checkParameters();
        ServiceConfiguration configuration(grantType, authEndpoint, tokenEndpoint,
                clientId, redirectUri, scope, username, password);
        Service service(configuration, sendRequestFunction);
        return service;
    }

    void ServiceBuilder::checkParameters()
    {
        if (tokenEndpoint.empty()) {
            throw std::invalid_argument("Access token endpoint is not set");
        }

        switch (grantType) {
        case AUTH_CODE_GRANT:
            checkAuthCodeGrant();
            break;
        case IMPLICIT_GRANT:
            checkImplicitGrant();
            break;
        case OWNER_CREDENTIALS_GRANT:
            checkOwnerCredentialsGrant();
            break;
        case CLIENT_CREDENTIALS_GRANT:
            break;
        }
    }

    void ServiceBuilder::checkAuthCodeGrant()
    {
        if (authEndpoint.empty()) {
            throw std::invalid_argument("Authentication code endpoint is not set");
        }
        if (clientId.empty()) {
            throw std::invalid_argument("Client id is not set");
        }
    }

    void ServiceBuilder::checkImplicitGrant()
    {
        if (clientId.empty()) {
            throw std::invalid_argument("Client id is not set");
        }
    }

    void ServiceBuilder::checkOwnerCredentialsGrant()
    {
        if (username.empty()) {
            throw std::invalid_argument("Username is not set");
        }
        if (password.empty()) {
            throw std::invalid_argument("Password is not set");
        }
    }
}
