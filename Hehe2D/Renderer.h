#pragma once
#include <map>
#include <string>
#include "typedefs.h"
#include <windows.h>
#include "EGL/egl.h"
#include "GLES2/gl2.h"
#include "macros.h"
#include "Node.h"
#include "PointSizeRect.h"
NS_HEHE2D_BEGIN
class Renderer
{
public:
    Renderer(void);
    ~Renderer(void);

    static Renderer* instance();
    void init(uint32 w, uint32 h);
    void run();
    Node* rootNode(){return rootNode_;}

    void addShaderProgram(const char* name, const char* vsFilePath, const char* fsFilePath);
    GLuint getShaderProgram(const char* name);
    GLuint useShaderProgram(const char* name);

    Size getWinSize(){return Size(winWidth_, winHeight_);}

private:
    void _initNativeWindow(uint32 w, uint32 h);
    void _initOpenGLES();
    static LRESULT WINAPI _windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void _drawRoot();
    void _initDefaultShaderPrograms();

private:
    EGLNativeWindowType hWnd_;
    EGLDisplay display_;
    EGLSurface surface_;

    uint32 winWidth_;
    uint32 winHeight_;

    GLuint curProgram_;
    std::map<std::string, GLuint> shaderPrograms_;

    Node* rootNode_;
    float lastFrameTime_;
};
NS_HEHE2D_END
