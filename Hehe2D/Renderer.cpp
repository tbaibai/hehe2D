#include "Renderer.h"
#include "EventDispatcher.h"
#include "EGL/egl.h"

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
	, rootNode_(new Node())
	, lastFrameTime_(0.f)
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
}

void Renderer::_initNativeWindow( uint32 w, uint32 h )
{
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

	GLuint flags = ES_WINDOW_ALPHA | ES_WINDOW_DEPTH | ES_WINDOW_STENCIL | ES_WINDOW_MULTISAMPLE;
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

	while (!done)
	{
		DWORD t0 = GetTickCount();
		
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

		_drawRoot();

		DWORD t1 = GetTickCount();
		if(t1 - t0 < 167)
		{
			Sleep(167 - t0 + t1);
		}

		lastFrameTime_ = (GetTickCount() - t0) / 1000.f;
	}
}

void Renderer::_drawRoot()
{
	rootNode_->visit();
	eglSwapBuffers(display_, surface_);
}

NS_HEHE2D_END