#include <string>

namespace OAuth2
{
    /**
     *  Stores OAuth 2.0 access and refresh tokens with relevant data.
     */
    class Token
    {
    public:
        static const std::string BEARER_TOKEN;
        static const int EXPIRES_UNDEFINED;
        static const std::string REFRESH_UNDEFINED;

        /**
         *  Creates OAuth 2.0 Token and refresh token.
         *  @param accessToken Access token string.
         *  @param expiresIn Expiration time in seconds.
         *  @param refreshToken Refresh token string.
         *  @param tokenType Token type string.
         */
        Token(const std::string &accessToken,
                int expiresIn = EXPIRES_UNDEFINED,
                const std::string &refreshToken = REFRESH_UNDEFINED,
                const std::string &tokenType = BEARER_TOKEN);

        /**
         *  Returns token type.
         *  @returns Token type.
         */
        const std::string &getTokenType() const;

        /**
         *  Returns refresh token.
         *  @returns Refresh token.
         */
        const std::string &getRefreshToken() const;

        /**
         *  Returns access token.
         *  @returns Access token.
         */
        const std::string &getAccessToken() const;

    private:
        std::string accessToken;
        std::string refreshToken;
        std::string tokenType;
        int expiresIn;
    };
}
