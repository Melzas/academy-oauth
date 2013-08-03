#include "crypto/HMAC_SHA1.h"
#include "crypto/base64.h"

#include "Signature.h"
#include "ParameterList.h"
#include "Utility.h"

namespace OAuth
{
    Signature::Signature(SignatureMethod method) :
            method(method)
    {

    }

    const std::string Signature::get(const std::string &baseString,
            const std::string &clientSecret, const std::string &tokenSecret)
    {
        this->baseString = baseString;
        this->clientSecret = clientSecret;
        this->tokenSecret = tokenSecret;

        switch (method) {
        case RSA_SHA1:
            return rsaSha1Signature();
        case PLAINTEXT:
            return plainTextSignature();
        case HMAC_SHA1:
        default:
            return hmacSha1Signature();
        }
    }

    const std::string Signature::hmacSha1Signature()
    {
        const size_t DIGEST_SIZE = 20;
        const std::string key = clientSecret + '&' + tokenSecret;
        BYTE *byteKey = const_cast<BYTE*>(reinterpret_cast<const BYTE*> (key.c_str()));
        BYTE *byteBaseString = const_cast<BYTE*>
                (reinterpret_cast<const BYTE*> (baseString.c_str()));

        BYTE digest[DIGEST_SIZE];
        CHMAC_SHA1 HMAC_SHA1;
        HMAC_SHA1.HMAC_SHA1(byteBaseString, baseString.length(),
                byteKey, key.length(), digest);

        std::string stringDigest = base64_encode(digest, DIGEST_SIZE);
        return Utility::urlEncode(stringDigest);
    }

    const std::string Signature::rsaSha1Signature()
    {
        // TODO: add encryption
        return baseString;
    }

    const std::string Signature::plainTextSignature()
    {
        return clientSecret + '?' + tokenSecret;
    }
}
