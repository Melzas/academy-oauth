#ifndef O2TOKEN_EXCEPTION_H_
#define O2TOKEN_EXCEPTION_H_

#include <stdexcept>

namespace OAuth2
{
    /**
     *  Describes what server returns when something is wrong while acquiring token.
     */
    class TokenException : public std::runtime_error
    {
    public:
        /**
         *  Creates exception with specified parameters.
         *  @param error Error code.
         *  @param description Human-readable text providing additional information.
         *  @param uri A URI identifying a human-readable web page with
         *  information about the error, used to provide the client
         *  developer with additional information about the error.
         */
        TokenException(std::string error, std::string description, std::string uri);

        /**
         *  Returns error code.
         *  @returns Error code.
         */
        const std::string &getError() const;

        /**
         *  Returns human-readable text providing additional information.
         *  @returns Error description.
         */
        const std::string &getDescription() const;

        /**
         *  Returns a URI identifying a human-readable web page with
         *  information about the error, used to provide the client
         *  developer with additional information about the error.
         *  @returns URI.
         */
        const std::string &getUri() const;

    private:
        std::string error;
        std::string description;
        std::string uri;
    };
}

#endif
