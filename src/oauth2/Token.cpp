#include <oauth2/Token.h>

namespace OAuth2
{
    const std::string Token::BEARER_TOKEN = "bearer";
    const int Token::EXPIRES_UNDEFINED = -1;
    const std::string Token::REFRESH_UNDEFINED = "";

    Token::Token(const std::string &accessToken,
            int expiresIn,
            const std::string &refreshToken,
            const std::string &tokenType) :
        accessToken(accessToken)
        ,refreshToken(refreshToken)
        ,tokenType(tokenType)
        ,expiresIn(expiresIn)
    {
    }

    const std::string &Token::getTokenType() const
    {
        return tokenType;
    }

    const std::string &Token::getRefreshToken() const
    {
        return refreshToken;
    }

    const std::string &Token::getAccessToken() const
    {
        return accessToken;
    }
}
