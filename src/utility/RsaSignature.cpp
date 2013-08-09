#include "RsaSignature.h"

#include <stdexcept>
#include <cstdio>

#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "crypto/rsa/sha1.h"
#include "crypto/base64.h"

extern "C" {
#include <openssl/applink.c>
}

namespace OAuth
{
    namespace Utility
    {
        const std::string rsaSha1Signature(const std::string &baseString,
            const std::string &rsaKeyPath, const std::string &)
        {
            typedef unsigned char BYTE;
            const size_t DIGEST_SIZE = 20;
            BYTE messageDigest[DIGEST_SIZE];
            sha1::calc(baseString.c_str(), baseString.length(), messageDigest);

            EVP_PKEY *signingKey = NULL;
            FILE *keyFile = fopen(rsaKeyPath.c_str(), "r");
            signingKey = PEM_read_PrivateKey(keyFile, NULL, NULL, NULL);
            fclose(keyFile);
            if(!signingKey) {
                throw std::invalid_argument("Invalid path or signing key");
            }

            EVP_PKEY_CTX *ctx;
            ctx = EVP_PKEY_CTX_new(signingKey, NULL);
            if (!ctx) {
                throw std::runtime_error("Error occured1");
            }
            if (EVP_PKEY_sign_init(ctx) <= 0) {
                throw std::runtime_error("Error occured2");
            }
            if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING) <= 0) {
                throw std::runtime_error("Error occured3");
            }
            if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha1()) <= 0) {
                throw std::runtime_error("Error occured4");
            }

            size_t signatureSize = EVP_PKEY_size(signingKey);
            BYTE *rawSignature = new BYTE[signatureSize];
            if (EVP_PKEY_sign(ctx, NULL, &signatureSize, messageDigest, DIGEST_SIZE) <= 0) {
                throw std::runtime_error("Error occured5");
            }
            if (EVP_PKEY_sign(ctx, rawSignature, &signatureSize, messageDigest,
                    DIGEST_SIZE) <= 0) {
                throw std::runtime_error("Error occured6");
            }

            std::string signature = base64_encode(rawSignature, signatureSize);
            return signature;
        }
    }
}
