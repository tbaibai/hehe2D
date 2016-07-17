#include "FileUtils.h"
#include "stdio.h"

NS_HEHE2D_BEGIN

uint8* FileUtils::getBytesData( const char* filePath, size_t& fileSize )
{
    FILE* fp = fopen(filePath, "rb");
    fseek(fp, 0L, SEEK_END);  
    fileSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    uint8* buffer = new uint8[fileSize];
    fread(buffer, fileSize, 1, fp);
    fclose(fp);
    return buffer;
}

NS_HEHE2D_END