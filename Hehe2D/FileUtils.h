#pragma once
#include "macros.h"
#include "typedefs.h"
NS_HEHE2D_BEGIN
class FileUtils
{
public:
    static uint8* getBytesData(const char* filePath, size_t& fileSize);
};
NS_HEHE2D_END
