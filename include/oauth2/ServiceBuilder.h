#ifndef O2SERVICEBUILDER_H_
#define O2SERVICEBUILDER_H_

#include <string>

#include "ServiceConfiguration.h"
#include "Service.h"

namespace OAuth2
{
    /**
     * Simplifies creation of the Service object
     */
    class ServiceBuilder
    {
    public:
        ServiceBuilder();

        /**
         * Set OAuth 2.0 authorization grant type
         * @param grantType Type of a grant
         * @returns this ServiceBuilder object to use for further configuration
         */
        ServiceBuilder &setGrantType(GrantType grantType);

        /**
         * Set authorization code URL
         * @param authEndpoint URL to obtain an authorization code
         * @returns this ServiceBuilder object to use for further configuration
         */
        ServiceBuilder &setAuthEndpoint(const std::string &authEndpoint);

        /**
         * Set access token URL
         * @param tokenEndpoint URL to obtain an access token
         */
        ServiceBuilder &setTokenEndpoint(const std::string &tokenEndpoint);

        /**
         * Set identifier of the client (given by authorization server)
         * @param clientId Client identifier
         * @returns this ServiceBuilder object to use for further configuration
         */
        ServiceBuilder &setClientId(const std::string &clientId);

        /**
         * Set the URL to which server will redirect the user after access confirmation
         * @param redirectUri URL to redirect the user
         * @returns this ServiceBuilder object to use for further configuration
         */
        ServiceBuilder &setRedirectUri(const std::string &redirectUri);

        /**
         * Set the required scope. Allowable values are defined by authorization server
         * @param scope Required scope
         * @returns this ServiceBuilder object to use for further configuration
         */
        ServiceBuilder &setScope(const std::string &scope);

        /**
         * Set the user's username. Only for Resource Owner Password Credentials Grant
         * @param username Username of user at the authorization server
         * @returns this ServiceBuilder object to use for further configuration
         */
        ServiceBuilder &setUsername(const std::string &username);

        /**
         * Set the user's password. Only for Resource Owner Password Credentials Grant
         * @param password Password of user at the authorization server
         */
        ServiceBuilder &setPassword(const std::string &password);

        /**
         * Set the function to make HTTP requests.
         * Service will use cURL, if this is not set.
         * @param sendRequestFunction Function to make HTTP requests
         * @returns this ServiceBuilder object to use for further configuration
         */
        ServiceBuilder &setSendRequest(const send_request_t &sendRequestFunction);

        /**
         * Create Service object with all specified settings
         * @returns Service object
         * @throws invalid_argument Throws exception, if some of required settings are not set
         */
        Service build();

    private:
        void checkParameters();

        void checkAuthCodeGrant();
        void checkImplicitGrant();
        void checkOwnerCredentialsGrant();

        GrantType grantType;
        std::string authEndpoint;
        std::string tokenEndpoint;
        std::string clientId;
        std::string redirectUri;
        std::string scope;
        std::string username;
        std::string password;
        send_request_t sendRequestFunction;
    };
}
#endif
