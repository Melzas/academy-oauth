#ifndef SINGLE_LEVEL_JSON_H_
#define SINGLE_LEVEL_JSON_H_

#include <map>
#include <string>

#include <ParameterList.h>

namespace Utility
{
    /**
     *  Parse single level JSON to ParameterList
     *  @param data JSON string. No arrays, no nested objects.
     *  @returns ParameterList, representing object.
     */
    OAuth::ParameterList parseSingleLevelJSON(std::string data);
}

#endif
