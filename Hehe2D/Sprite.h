#pragma once
#include "node.h"
#include "typedefs.h"
#include "GLES2/gl2.h"
#include "libpng/png.h"
NS_HEHE2D_BEGIN
class Sprite :
    public Node
{
public:
    Sprite(void);
    virtual ~Sprite(void);
    void setTextureFile(const char* filePath);
    explicit Sprite(const char* filePath);
    virtual void draw();

private:
    bool _initWithPngData(uint8 * pData, int nDatalen);

private:
    uint8* image_;
    uint32 imageWidth_;
    uint32 imageHeight_;
    GLuint texObj_;
};

struct tImageSource
{
    uint8* data;
    int size;
    int offset;
};

static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
    tImageSource* isource = (tImageSource*)png_get_io_ptr(png_ptr);

    if((int)(isource->offset + length) <= isource->size)
    {
        memcpy(data, isource->data+isource->offset, length);
        isource->offset += length;
    }
    else
    {
        png_error(png_ptr, "pngReaderCallback failed");
    }
}

NS_HEHE2D_END