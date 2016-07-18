#include "Sprite.h"
#include "assert.h"
#include "Renderer.h"
#include "FileUtils.h"

NS_HEHE2D_BEGIN
Sprite::Sprite(void)
    : image_(NULL)
    , imageHeight_(0)
    , imageWidth_(0)
    , texObj_(0)
{
}

Sprite::~Sprite(void)
{
    delete[] image_;
}

Sprite::Sprite( const char* filePath )//暂只支持png格式的散图
    : image_(NULL)
{
    size_t fileSize;
    uint8* buffer =  FileUtils::getBytesData(filePath, fileSize);
    bool ret = _initWithPngData(buffer, fileSize);
    assert(ret);

    //目前只支持rgba32位的png
    if(imageWidth_ % 2 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
    }
    else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }

    glGenTextures(1, &texObj_);
    //set data and params for texObj_

    // bind texObj_ to "opt-slot" (GL_TEXTURE_2D)
    glBindTexture(GL_TEXTURE_2D, texObj_);//可以考虑作cache

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)imageWidth_, (GLsizei)imageHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_);

    delete[] buffer;
}

void Sprite::draw()
{
    //set program obj
    GLuint progObj = Renderer::instance()->useShaderProgram("defaultSprite");

    //set uniform
    GLuint mvp =  glGetUniformLocation ( progObj, "u_mvp" );
    computeTranform();
    glUniformMatrix4fv(mvp, 1, GL_FALSE, (GLfloat*)&transform_.m[0][0]);

    //set attributes
    Size winSize = Renderer::instance()->getWinSize();
    float x = imageWidth_ / winSize.w;
    float y = imageHeight_ / winSize.h;
    GLfloat posAndTexcoord[] = { -x , y, 0.0f,  // Position 0
        0.0f,  0.0f,        // TexCoord 0 
        -x, -y, 0.0f,  // Position 1
        0.0f,  1.0f,        // TexCoord 1
        x, -y, 0.0f,  // Position 2
        1.0f,  1.0f,        // TexCoord 2
        x,  y, 0.0f,  // Position 3
        1.0f,  0.0f         // TexCoord 3
    };
    GLuint pos = glGetAttribLocation ( progObj, "a_position" );
    GLuint texCoord = glGetAttribLocation ( progObj, "a_texCoord" );
    glVertexAttribPointer ( pos, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), posAndTexcoord );
    // Load the texture coordinate
    glVertexAttribPointer ( texCoord, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), &posAndTexcoord[3] );
    glEnableVertexAttribArray ( pos );//默认是const 属性，数组型的属性需要额外开启！
    glEnableVertexAttribArray ( texCoord );

    //set sample2d(texture)
    GLuint texture = glGetUniformLocation ( progObj, "s_texture" );
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texObj_);
    glUniform1i(texture, 0);

    //make drawcall
    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
    glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
}

bool Sprite::_initWithPngData(uint8 * pData, int nDatalen)
{
    // length of bytes to check if it is a valid png file
    const int PNGSIGSIZE = 8;
    bool bRet = false;
    png_byte        header[PNGSIGSIZE]   = {0}; 
    png_structp     png_ptr     =   0;
    png_infop       info_ptr    = 0;

    do 
    {
        // png header len is 8 bytes
        HEHE_BREAK_IF(nDatalen < PNGSIGSIZE);

        // check the data is png or not
        memcpy(header, pData, PNGSIGSIZE);
        HEHE_BREAK_IF(png_sig_cmp(header, 0, PNGSIGSIZE));

        // init png_struct
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        HEHE_BREAK_IF(! png_ptr);

        // init png_info
        info_ptr = png_create_info_struct(png_ptr);
        HEHE_BREAK_IF(!info_ptr);

        HEHE_BREAK_IF(setjmp(png_jmpbuf(png_ptr)));

        // set the read call back function
        tImageSource imageSource;
        imageSource.data    = pData;
        imageSource.size    = nDatalen;
        imageSource.offset  = 0;
        png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

        // read png file info
        png_read_info(png_ptr, info_ptr);

        imageHeight_ = png_get_image_width(png_ptr, info_ptr);
        imageWidth_ = png_get_image_height(png_ptr, info_ptr);
        png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

        //CCLOG("color type %u", color_type);

        // force palette images to be expanded to 24-bit RGB
        // it may include alpha channel
        if (color_type == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_palette_to_rgb(png_ptr);
        }

        // expand any tRNS chunk data into a full alpha channel
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        {
            png_set_tRNS_to_alpha(png_ptr);
        }
        // expand grayscale images to RGB
        if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        {
            png_set_gray_to_rgb(png_ptr);
        }

        // read png data
        // bitsPerComponent will always be 8
        png_uint_32 rowbytes;
        png_bytep* row_pointers = (png_bytep*)malloc( sizeof(png_bytep) * imageHeight_ );

        png_read_update_info(png_ptr, info_ptr);

        rowbytes = png_get_rowbytes(png_ptr, info_ptr);

        image_ = new unsigned char[rowbytes * imageHeight_];
        HEHE_BREAK_IF(!image_);

        for (unsigned short i = 0; i < imageHeight_; ++i)
        {
            row_pointers[i] = image_ + i*rowbytes;
        }
        png_read_image(png_ptr, row_pointers);

        png_read_end(png_ptr, NULL);

        png_uint_32 channel = rowbytes / imageHeight_;
        if (channel == 4)
        {
            unsigned int *tmp = (unsigned int *)image_;
            for(unsigned short i = 0; i < imageHeight_; i++)
            {
                for(unsigned int j = 0; j < rowbytes; j += 4)
                {
                    uint32 r = row_pointers[i][j];
                    uint32 g = row_pointers[i][j + 1];
                    uint32 b = row_pointers[i][j + 2];
                    uint32 a = row_pointers[i][j + 3];
                    *tmp++ = r + (g << 8) + (b << 16) + (a << 24);
                }
            }
        }
        else
        {
            assert(0);
        }

        free(row_pointers);

        bRet = true;
    } while (0);

    if (png_ptr)
    {
        png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
    }

    return bRet;
}

NS_HEHE2D_END