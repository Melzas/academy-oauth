#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "HttpRequestTests.h"
#include "UrlEncodeDecode.h"
#include "NormalizeUrl.h"
#include "HostResourceFromUrl.h"
#include "ParameterListTests.h"
#include "DefaultSendRequestTests.h"
#include "SingleLevelJsonTests.h"

#include "oauth1/SignatureTests.h"
#include "oauth1/RsaSignatureTests.h"
#include "oauth1/ExtractorTests.h"
#include "oauth1/TokenTests.h"
#include "oauth1/ServiceTests.h"

#include "oauth2/ServiceTests.h"
#include "oauth2/ServiceBuilderTests.h"

GTEST_API_ int main(int argc, char **argv)
{
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
