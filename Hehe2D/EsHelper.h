#pragma once
#include "macros.h"
#include "EGL/egl.h"
#include "GLES2/gl2.h"

NS_HEHE2D_BEGIN
struct Matrix
{
    GLfloat   m[4][4];
};

class EsHelper
{
public:
    static const float PI;
    static GLuint esLoadProgram ( const char *vertShaderSrc, const char *fragShaderSrc );
    static GLuint esLoadShader ( GLenum type, const char *shaderSrc );
    static void esLogMessage ( const char *formatStr, ... );

    static void matrixLoadIdentity(Matrix& mat);
    static void matrixLoadPerspective(Matrix& mat, float fov, float aspect, float nearZ, float farZ);
    static void translate(Matrix& mat, GLfloat x, GLfloat y, GLfloat z);
    static void rotate(Matrix& mat, GLfloat angel, GLfloat x, GLfloat y, GLfloat z);
    static void matrixMultiply(Matrix& result, Matrix& matA, Matrix& matB);
};
NS_HEHE2D_END