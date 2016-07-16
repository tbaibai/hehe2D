#pragma once
#include "typedefs.h"
#include <windows.h>
#include "EGL/egl.h"
#include "GLES2/gl2.h"
#include "macros.h"
#include "Node.h"
NS_HEHE2D_BEGIN
class Renderer
{
public:
	Renderer(void);
	~Renderer(void);

	static Renderer* instance();
	void init(uint32 w, uint32 h);
	void run();
	Node* rootNode();

private:
	void _initNativeWindow(uint32 w, uint32 h);
	void _initOpenGLES();
	static LRESULT WINAPI _windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void _drawRoot();

private:
	EGLNativeWindowType hWnd_;
	EGLDisplay display_;
	EGLSurface surface_;
	Node* rootNode_;
	float lastFrameTime_;
};
NS_HEHE2D_END
