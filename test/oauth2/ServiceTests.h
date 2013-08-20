#include <gtest/gtest.h>

#include <future>

#include <oauth2/Service.h>
#include <oauth2/ServiceBuilder.h>
#include <oauth2/Token.h>
#include <oauth2/TokenException.h>
#include <HttpRequest.h>

namespace OAuth2
{
    class ServiceTests2: public testing::Test
    {
    public:
        ServiceBuilder authCodeGrantBuilder;
        ServiceBuilder implicitGrantBuilder;
        ServiceBuilder ownerCredentialsGrantBuilder;
        ServiceBuilder clientCredentialsGrantBuilder;

        std::string succesfulTokenResponse;

        ServiceTests2() :
            succesfulTokenResponse("{"
                "    \"access_token\":\"2YotnFZFEjr1zCsicMWpAA\",\r\n"
                "    \"token_type\":\"example\",\r\n"
                "    \"expires_in\":3600,\r\n"
                "    \"refresh_token\":\"tGzv3JOkF0XG5Qx2TlKWIA\",\r\n"
                "    \"example_parameter\":\"example_value\"\r\n"
                "}")
        {
            authCodeGrantBuilder.setGrantType(AUTH_CODE_GRANT)
                    .setAuthEndpoint("https://server.example.com/authorize")
                    .setTokenEndpoint("https://server.example.com/token")
                    .setClientId("s6BhdRkqt3")
                    .setRedirectUri("https://client.example.com/cb");
            implicitGrantBuilder.setGrantType(IMPLICIT_GRANT)
                    .setAuthEndpoint("https://server.example.com/authorize")
                    .setTokenEndpoint("https://server.example.com/token")
                    .setClientId("s6BhdRkqt3")
                    .setRedirectUri("https://client.example.com/cb");
            ownerCredentialsGrantBuilder.setGrantType(OWNER_CREDENTIALS_GRANT)
                    .setAuthEndpoint("https://server.example.com/authorize")
                    .setTokenEndpoint("https://server.example.com/token")
                    .setClientId("s6BhdRkqt3")
                    .setUsername("johndoe")
                    .setPassword("A3ddj3w");
            clientCredentialsGrantBuilder.setGrantType(CLIENT_CREDENTIALS_GRANT)
                    .setAuthEndpoint("https://server.example.com/authorize")
                    .setTokenEndpoint("https://server.example.com/token")
                    .setClientId("s6BhdRkqt3");
        }
    };

    TEST_F(ServiceTests2, get_authorize_url_no_parameters_auth_code_grant)
    {
        Service service = authCodeGrantBuilder.build();
        ASSERT_EQ(
                "https://server.example.com/authorize?response_type=code&client_id=s6BhdRkqt3"
                "&redirect_uri=https%3A%2F%2Fclient.example.com%2Fcb&state=xyz",
                service.getAuthorizeUrl("xyz"));
    }

    TEST_F(ServiceTests2, get_authorize_url_no_parameters_implicit_grant)
    {
        Service service = implicitGrantBuilder.build();
        ASSERT_EQ(
            "https://server.example.com/authorize?response_type=token&client_id=s6BhdRkqt3"
            "&redirect_uri=https%3A%2F%2Fclient.example.com%2Fcb&state=xyz",
            service.getAuthorizeUrl("xyz"));
    }

    TEST_F(ServiceTests2, get_access_token_auth_code_ok)
    {
        std::string sentRequestBody;
        std::string sentRequestContentType;

        Service service = authCodeGrantBuilder.setSendRequest(
                [&] (const OAuth::HttpRequest &request) -> std::string {
                    sentRequestBody = request.getBody();
                    sentRequestContentType = request.getHeaders()
                        .find(OAuth::HEADER_CONTENT_TYPE)->second;
                    return succesfulTokenResponse;
                }
            ).build();

        Token token = service.getAccessToken("https://client.example.com/cb?code="
            "SplxlOBeZQQYbYS6WxSbIA&state=xyz").get();

        ASSERT_EQ("grant_type=authorization_code&code=SplxlOBeZQQYbYS6WxSbIA"
            "&redirect_uri=https%3A%2F%2Fclient.example.com%2Fcb&client_id=s6BhdRkqt3",
            sentRequestBody);
        ASSERT_EQ(OAuth::FORM_URLENCODED, sentRequestContentType);

        ASSERT_EQ("2YotnFZFEjr1zCsicMWpAA", token.getAccessToken());
        ASSERT_EQ("example", token.getTokenType());
        ASSERT_EQ("tGzv3JOkF0XG5Qx2TlKWIA", token.getRefreshToken());
    }

    TEST_F(ServiceTests2, get_access_token_implicit_grant_ok)
    {
        Service service = implicitGrantBuilder.build();
        Token token = service.getAccessToken("http://example.com/cb?access_token=2YotnFZFEjr1zCsicMWpAA"
            "&state=xyz&token_type=example&expires_in=3600").get();
        ASSERT_EQ("2YotnFZFEjr1zCsicMWpAA", token.getAccessToken());
        ASSERT_EQ("example", token.getTokenType());
        ASSERT_EQ(Token::REFRESH_UNDEFINED, token.getRefreshToken());
    }

    TEST_F(ServiceTests2, get_access_token_owner_credentials_grant_ok)
    {
        std::string sentRequestBody;
        std::string sentRequestContentType;

        Service service = ownerCredentialsGrantBuilder.setSendRequest(
                [&] (const OAuth::HttpRequest &request) -> std::string {
                    sentRequestBody = request.getBody();
                    sentRequestContentType = request.getHeaders()
                        .find(OAuth::HEADER_CONTENT_TYPE)->second;

                    return succesfulTokenResponse;
                }
            ).build();

        Token token = service.getAccessToken().get();

        ASSERT_EQ("grant_type=password&username=johndoe&password=A3ddj3w",
            sentRequestBody);
        ASSERT_EQ(OAuth::FORM_URLENCODED, sentRequestContentType);

        ASSERT_EQ("2YotnFZFEjr1zCsicMWpAA", token.getAccessToken());
        ASSERT_EQ("example", token.getTokenType());
        ASSERT_EQ("tGzv3JOkF0XG5Qx2TlKWIA", token.getRefreshToken());
    }

    TEST_F(ServiceTests2, get_access_token_client_credentials_grant_ok)
    {
        std::string sentRequestBody;
        std::string sentRequestContentType;

        Service service = clientCredentialsGrantBuilder.setSendRequest(
                [&] (const OAuth::HttpRequest &request) -> std::string {
                    sentRequestBody = request.getBody();
                    sentRequestContentType = request.getHeaders()
                        .find(OAuth::HEADER_CONTENT_TYPE)->second;

                    return succesfulTokenResponse;
                }
            ).build();

        Token token = service.getAccessToken().get();

        ASSERT_EQ("grant_type=client_credentials", sentRequestBody);
        ASSERT_EQ(OAuth::FORM_URLENCODED, sentRequestContentType);

        ASSERT_EQ("2YotnFZFEjr1zCsicMWpAA", token.getAccessToken());
        ASSERT_EQ("example", token.getTokenType());
        ASSERT_EQ("tGzv3JOkF0XG5Qx2TlKWIA", token.getRefreshToken());
    }

    TEST_F(ServiceTests2, get_access_token_auth_code_error)
    {
        Service service = authCodeGrantBuilder.setSendRequest(
                [&] (const OAuth::HttpRequest &request) -> std::string {
                    return "{\"error\":\"invalid_request\"}";
                }
            ).build();

        ASSERT_THROW(service.getAccessToken("https://client.example.com/cb?code="
            "SplxlOBeZQQYbYS6WxSbIA&state=xyz").get(), OAuth2::TokenException);
    }

    TEST_F(ServiceTests2, get_access_token_implicit_grant_error)
    {
        Service service = implicitGrantBuilder.build();
        ASSERT_THROW(
            service.getAccessToken("https://client.example.com/cb?error=access_denied&state=xyz").get(),
            OAuth2::TokenException);

    }

    TEST_F(ServiceTests2, get_access_token_owner_credentials_grant_error)
    {
        Service service = ownerCredentialsGrantBuilder.setSendRequest(
                [&] (const OAuth::HttpRequest &request) -> std::string {
                    return "{\"error\":\"invalid_request\"}";
                }
            ).build();
        ASSERT_THROW(service.getAccessToken().get(), OAuth2::TokenException);
    }

    TEST_F(ServiceTests2, get_access_token_client_credentials_grant_error)
    {
        Service service = clientCredentialsGrantBuilder.setSendRequest(
                [&] (const OAuth::HttpRequest &request) -> std::string {
                    return "{\"error\":\"invalid_request\"}";
                }
            ).build();
        ASSERT_THROW(service.getAccessToken().get(), OAuth2::TokenException);
    }

    TEST_F(ServiceTests2, sign_request)
    {
        Service service = implicitGrantBuilder.build();
        Token token("mF_9.B5f-4.1JqM");
        OAuth::HttpRequest httpRequest(OAuth::POST, "https://data.example.com/");
        service.signRequest(httpRequest, token);
        ASSERT_EQ("Bearer mF_9.B5f-4.1JqM", httpRequest.getHeaders().find("Authorization")->second);
    }
}
