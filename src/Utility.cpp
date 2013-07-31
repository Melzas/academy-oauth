#include <Utility.h>

#include <sstream>
#include <iomanip>
#include <regex>

namespace OAuth
{
    namespace Utility
    {
        bool isUnreservedUrlCharacter(int character)
        {
            std::string otherUnreserved = "-._~";
            return isalnum(character) || otherUnreserved.find(character) != std::string::npos;
        }

        std::string urlEncode(std::string data)
        {
            std::ostringstream encodedStream;

            for (size_t i = 0; i < data.length(); ++i) {
                if (isUnreservedUrlCharacter(data[i]))
                    encodedStream << data[i];
                else
                    encodedStream << "%" << std::uppercase << std::setfill('0') << std::setw(2)
                        << std::hex << static_cast<unsigned int>(data[i]);
            }
            return encodedStream.str();
        }

        std::string urlDecode(std::string data)
        {
            std::ostringstream decodedStream;

            for (size_t i = 0; i < data.length(); ++i) {
                unsigned int currentCharacter = data[i];
                if (currentCharacter == '%') {
                    std::istringstream encodedChar(data.substr(i + 1, 2));
                    encodedChar >> std::hex >> currentCharacter;
                    i += 2;
                }
                if (currentCharacter == '+') {
                    currentCharacter = ' ';
                }
                decodedStream << static_cast<char>(currentCharacter);
            }
            return decodedStream.str();
        }

        std::pair<int, int> getHostPosition(std::string url)
        {
            std::string protocolEnd = "://";
            int hostBeginPosition = url.find(protocolEnd) + protocolEnd.length();
            int hostEndPositon = url.find("/", hostBeginPosition);
            return std::make_pair(hostBeginPosition, hostEndPositon);
        }

        std::string hostFromUrl(std::string url)
        {
            std::pair<int, int> hostPosition = getHostPosition(url);
            int hostNameLength =  hostPosition.second - hostPosition.first;
            return url.substr(hostPosition.first, hostNameLength);
        }

        std::string resourceFromUrl(std::string url)
        {
            std::pair<int, int> hostPosition = getHostPosition(url);
            return url.substr(hostPosition.second);
        }

        std::string normalizeUrl(std::string url)
        {
            std::regex hasProtocol(".*://.*");
            if (!std::regex_match(url, hasProtocol))
                url = "http://" + url;
            std::regex hasSlashAfterHost(".*://.*/.*");
            if (!std::regex_match(url, hasSlashAfterHost))
                url += "/";
            return url;
        }
    }
}
