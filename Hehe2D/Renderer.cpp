#include "Renderer.h"
#include "EventDispatcher.h"
#include "EGL/egl.h"
#include "EsHelper.h"
#include "FileUtils.h"

/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1 
/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2 
/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
/// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8

NS_HEHE2D_BEGIN
    Renderer::Renderer(void)
    : hWnd_(0)
    , display_(0)
    , surface_(0)
    , rootNode_(new Node())
    , lastFrameTime_(0.f)
    , winWidth_(0)
    , winHeight_(0)
    , curProgram_(0)
{
}


Renderer::~Renderer(void)
{
    delete(rootNode_);
}

Renderer* Renderer::instance()
{
    static Renderer instance;
    return &instance;
}

void Renderer::init( uint32 w, uint32 h )
{
    _initNativeWindow(w, h);
    _initOpenGLES();
    _initDefaultShaderPrograms();
}

void Renderer::_initNativeWindow( uint32 w, uint32 h )
{
    winWidth_ = w;
    winHeight_ = h;
    //native window
    WNDCLASS wndclass = {0};
    DWORD    wStyle   = 0;
    RECT     windowRect;
    HINSTANCE hInstance = GetModuleHandle(NULL);

    wndclass.style         = CS_OWNDC;
    wndclass.lpfnWndProc   = (WNDPROC)(&Renderer::_windowProc);
    wndclass.hInstance     = hInstance; 
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
    wndclass.lpszClassName = "opengles2.0"; 

    RegisterClass (&wndclass);

    wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

    // Adjust the window rectangle so that the client area has
    // the correct number of pixels
    windowRect.left = 0;
    windowRect.top = 0;
    windowRect.right = w;
    windowRect.bottom = h;

    AdjustWindowRect ( &windowRect, wStyle, FALSE );

    hWnd_ = CreateWindow(
        "opengles2.0",
        "Hehe2D",
        wStyle,
        0,
        0,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL,
        NULL,
        hInstance,
        NULL);
    ShowWindow (hWnd_, TRUE);
}

LRESULT WINAPI Renderer::_windowProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
    LRESULT  lRet = 1; 

    switch (uMsg) 
    { 
    case WM_CREATE:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);             
        break; 
    case WM_CHAR:
        {
            POINT point;
            GetCursorPos( &point );
            EventDispatcher::instance()->dispatchKeyClicked((unsigned char) wParam, (int) point.x, (int) point.y);
        }
        break;
    default:
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 

    return lRet; 
}

void Renderer::_initOpenGLES()
{
    // Get Display
    EGLDisplay display;
    display = eglGetDisplay((EGLNativeDisplayType)hWnd_);

    // Initialize EGL
    EGLint majorVersion;
    EGLint minorVersion;
    eglInitialize(display, &majorVersion, &minorVersion);

    //ES_WINDOW_MULTISAMPLE在公司电脑不支持
    GLuint flags = ES_WINDOW_ALPHA | ES_WINDOW_DEPTH | ES_WINDOW_STENCIL;// | ES_WINDOW_MULTISAMPLE;
    EGLint attribList[] =
    {     
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE,       5,
        EGL_GREEN_SIZE,     6,
        EGL_BLUE_SIZE,      5,
        EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
        EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
        EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
        EGL_NONE
    };
    EGLint numConfigs;
    EGLConfig config;
    eglChooseConfig(display, attribList, &config, 1, &numConfigs);

    // Create a surface
    EGLSurface surface;
    surface = eglCreateWindowSurface(display, config, hWnd_, NULL);

    // Create a GL context
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
    EGLContext context;
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );

    // Make the context current
    eglMakeCurrent(display, surface, surface, context);

    display_ = display;
    surface_ = surface;
}

void Renderer::run()
{
    MSG msg = { 0 };
    int done = 0;
    LARGE_INTEGER nFreq;
    LARGE_INTEGER nLast;
    LARGE_INTEGER nNow;

    QueryPerformanceFrequency(&nFreq);
    QueryPerformanceCounter(&nLast);

    while (!done)
    {
        int gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);

        if ( gotMsg )
        {
            if (msg.message == WM_QUIT)
            {
                done = 1; 
            }
            else
            {
                TranslateMessage(&msg); 
                DispatchMessage(&msg); 
            }
        }

        QueryPerformanceCounter(&nNow);

        if((nNow.QuadPart - nLast.QuadPart) / (nFreq.QuadPart + 0.f) < 0.01667) //控制帧率不超过60
        {
            printf("s = %f\n", (float)(nNow.QuadPart - nLast.QuadPart)/ (nFreq.QuadPart + 0.f));
            timeBeginPeriod(1);
            Sleep(1);
            timeEndPeriod(1);
        }
        else
        {
            lastFrameTime_ = (nNow.QuadPart - nLast.QuadPart) / (nFreq.QuadPart + 0.f);
            printf("FPS = %d\n", (int)(1 / lastFrameTime_));
            nLast.QuadPart = nNow.QuadPart;
            _drawRoot();
        }
    }
}

void Renderer::_initDefaultShaderPrograms()
{
    GLbyte vShaderStr[] =  
        "attribute vec4 a_position;           \n"
        "attribute vec2 a_texCoord;           \n"
        "varying vec2 v_texCoord;             \n"
        "uniform mat4 u_mvp;                  \n"
        "void main()                          \n"
        "{                                    \n"
        "   gl_Position = u_mvp * a_position; \n"
        "   v_texCoord = a_texCoord;          \n"
        "}                                    \n";

    GLbyte fShaderStr[] =  
        "precision mediump float;                            \n"
        "varying vec2 v_texCoord;                            \n"
        "uniform sampler2D s_texture;                        \n"
        "void main()                                         \n"
        "{                                                   \n"
        "  gl_FragColor = texture2D( s_texture, v_texCoord );\n"
        "}                                                   \n";

    // Load the shaders and get a linked program object
    GLuint obj = EsHelper::esLoadProgram ( (const char*)vShaderStr, (const char*)fShaderStr );
    shaderPrograms_["defaultSprite"] = obj;
}

GLuint Renderer::addShaderProgram(const char* name, const char* vsFilePath, const char* fsFilePath)
{
    std::map<std::string, GLuint>::iterator it = shaderPrograms_.find(name);
    if(it == shaderPrograms_.end())
    {
        size_t fileSize;
        uint8* vs = FileUtils::getBytesData(vsFilePath, fileSize, true);
        uint8* fs = FileUtils::getBytesData(fsFilePath, fileSize, true);
        GLuint obj = EsHelper::esLoadProgram((const char*)vs, (const char*)fs);
        delete[] vs;
        delete[] fs;
        shaderPrograms_[name] = obj;
    }
    
    return shaderPrograms_[name];
}

GLuint Renderer::getShaderProgram( const char* name )
{
    std::string key(name);
    std::map<std::string, GLuint>::iterator it = shaderPrograms_.find(key);
    if(it != shaderPrograms_.end())
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}

GLuint Renderer::useShaderProgram(const char* name)
{
    GLuint obj = getShaderProgram(name);
    if(obj != curProgram_ && obj != 0)
    {
        glUseProgram(obj);
    }
    return obj;
}

void Renderer::_drawRoot()
{
    glViewport ( 0, 0, winWidth_, winHeight_); //其实也不用每帧都设置
    glClear ( GL_COLOR_BUFFER_BIT );
    rootNode_->visit();
    eglSwapBuffers(display_, surface_);
}

NS_HEHE2D_END