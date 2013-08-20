#include <oauth2/TokenException.h>

namespace OAuth2
{
    TokenException::TokenException(std::string error, std::string description, std::string uri) :
        std::runtime_error("Token error: " + error),
        error(error)
        ,description(description)
        ,uri(uri)
    {
    }

    const std::string &TokenException::getDescription() const
    {
        return description;
    }

    const std::string &TokenException::getError() const
    {
        return error;
    }

    const std::string &TokenException::getUri() const
    {
        return uri;
    }
}
