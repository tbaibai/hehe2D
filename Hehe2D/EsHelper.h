#pragma once
#include "macros.h"
#include "EGL/egl.h"
#include "GLES2/gl2.h"

NS_HEHE2D_BEGIN
class EsHelper
{
public:
    static GLuint esLoadProgram ( const char *vertShaderSrc, const char *fragShaderSrc );
    static GLuint esLoadShader ( GLenum type, const char *shaderSrc );
    static void esLogMessage ( const char *formatStr, ... );
};
NS_HEHE2D_END