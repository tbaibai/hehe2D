#include "EsHelper.h"
#include "stdio.h"
#include "macros.h"
#include "malloc.h"
#include "math.h"

NS_HEHE2D_BEGIN

const float EsHelper::PI = 3.1415926535897932384626433832795f;

void EsHelper::esLogMessage ( const char *formatStr, ... )
{
    va_list params;
    char buf[1024];

    va_start ( params, formatStr );
    vsprintf_s ( buf, sizeof(buf),  formatStr, params );

    printf ( "%s", buf );

    va_end ( params );
}

GLuint EsHelper::esLoadShader ( GLenum type, const char *shaderSrc )
{
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader ( type );

    if ( shader == 0 )
        return 0;

    // Load the shader source
    glShaderSource ( shader, 1, &shaderSrc, NULL );

    // Compile the shader
    glCompileShader ( shader );

    // Check the compile status
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

    if ( !compiled ) 
    {
        GLint infoLen = 0;

        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

        if ( infoLen > 1 )
        {
            char* infoLog = (char* )malloc (sizeof(char) * infoLen );

            glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
            esLogMessage ( "Error compiling shader:\n%s\n", infoLog );            

            free ( infoLog );
        }

        glDeleteShader ( shader );
        return 0;
    }

    return shader;

}

GLuint EsHelper::esLoadProgram ( const char *vertShaderSrc, const char *fragShaderSrc )
{
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;

    // Load the vertex/fragment shaders
    vertexShader = esLoadShader ( GL_VERTEX_SHADER, vertShaderSrc );
    if ( vertexShader == 0 )
        return 0;

    fragmentShader = esLoadShader ( GL_FRAGMENT_SHADER, fragShaderSrc );
    if ( fragmentShader == 0 )
    {
        glDeleteShader( vertexShader );
        return 0;
    }

    // Create the program object
    programObject = glCreateProgram ( );

    if ( programObject == 0 )
        return 0;

    glAttachShader ( programObject, vertexShader );
    glAttachShader ( programObject, fragmentShader );

    // Link the program
    glLinkProgram ( programObject );

    // Check the link status
    glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

    if ( !linked ) 
    {
        GLint infoLen = 0;

        glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

        if ( infoLen > 1 )
        {
            char* infoLog =(char* ) malloc (sizeof(char) * infoLen );

            glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
            esLogMessage ( "Error linking program:\n%s\n", infoLog );            

            free ( infoLog );
        }

        glDeleteProgram ( programObject );
        return 0;
    }

    // Free up no longer needed shader resources
    glDeleteShader ( vertexShader );
    glDeleteShader ( fragmentShader );

    return programObject;
}

void EsHelper::matrixLoadIdentity( Matrix& mat )
{
    memset(&mat, 0, sizeof(Matrix));
    mat.m[0][0] = 1.0f;
    mat.m[1][1] = 1.0f;
    mat.m[2][2] = 1.0f;
    mat.m[3][3] = 1.0f;
}

void EsHelper::matrixLoadPerspective( Matrix& mat, float fov, float aspect, float nearZ, float farZ )
{

}

//应用层，执行的是左乘转置矩阵
void EsHelper::translate( Matrix& mat, GLfloat tx, GLfloat ty, GLfloat tz )
{
    
    mat.m[3][0] += (mat.m[0][0] * tx + mat.m[1][0] * ty + mat.m[2][0] * tz);
    mat.m[3][1] += (mat.m[0][1] * tx + mat.m[1][1] * ty + mat.m[2][1] * tz);
    mat.m[3][2] += (mat.m[0][2] * tx + mat.m[1][2] * ty + mat.m[2][2] * tz);
    mat.m[3][3] += (mat.m[0][3] * tx + mat.m[1][3] * ty + mat.m[2][3] * tz);
}

void EsHelper::scale(Matrix &result, GLfloat sx, GLfloat sy, GLfloat sz)
{
    result.m[0][0] *= sx;
    result.m[0][1] *= sx;
    result.m[0][2] *= sx;
    result.m[0][3] *= sx;

    result.m[1][0] *= sy;
    result.m[1][1] *= sy;
    result.m[1][2] *= sy;
    result.m[1][3] *= sy;

    result.m[2][0] *= sz;
    result.m[2][1] *= sz;
    result.m[2][2] *= sz;
    result.m[2][3] *= sz;
}

void EsHelper::rotate( Matrix& result, GLfloat angle)
{
    Matrix rotMat;
    float cos_angle = cos(angle * PI / 180.f);
    float sin_angle = sin(angle * PI / 180.f);
    rotMat.m[0][0] = rotMat.m[1][1] = cos_angle;
    rotMat.m[0][1] = -sin_angle;
    rotMat.m[1][0] = sin_angle;
    rotMat.m[2][2] = rotMat.m[3][3] = 1.f;
    matrixMultiply(result, rotMat, result);
}

void EsHelper::matrixMultiply(Matrix& result, Matrix& matA, Matrix& matB)
{
    Matrix tmp;
    for (int i = 0; i < 4; ++i)
    {
        tmp.m[i][0] =	(matA.m[i][0] * matB.m[0][0]) +
            (matA.m[i][1] * matB.m[1][0]) +
            (matA.m[i][2] * matB.m[2][0]) +
            (matA.m[i][3] * matB.m[3][0]) ;

        tmp.m[i][1] =	(matA.m[i][0] * matB.m[0][1]) + 
            (matA.m[i][1] * matB.m[1][1]) +
            (matA.m[i][2] * matB.m[2][1]) +
            (matA.m[i][3] * matB.m[3][1]) ;

        tmp.m[i][2] =	(matA.m[i][0] * matB.m[0][2]) + 
            (matA.m[i][1] * matB.m[1][2]) +
            (matA.m[i][2] * matB.m[2][2]) +
            (matA.m[i][3] * matB.m[3][2]) ;

        tmp.m[i][3] =	(matA.m[i][0] * matB.m[0][3]) + 
            (matA.m[i][1] * matB.m[1][3]) +
            (matA.m[i][2] * matB.m[2][3]) +
            (matA.m[i][3] * matB.m[3][3]) ;
    }
    
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            result.m[i][j] = tmp.m[i][j];
        }
    }
    
}

NS_HEHE2D_END
