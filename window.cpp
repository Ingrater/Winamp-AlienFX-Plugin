#include "window.h"
#include <GL/gl.h>

size_t _Width = 640;
size_t _Height = 480;
HWND _hWnd = NULL;
HDC _hDC;
HGLRC _hRC;
HINSTANCE _hInstance;
bool RenderWindowVisible = false;

bool CreateOpenGLWindow(HWND phWnd, HINSTANCE phInstance){
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;
	_hInstance = phInstance;

	// Register A Window Class
	WNDCLASSEX windowClass;												// Window Class
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));						// Make Sure Memory Is Cleared
	windowClass.cbSize			= sizeof (WNDCLASSEX);					// Size Of The windowClass Structure
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraws The Window For Any Movement / Resizing
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				// WindowProc Handles Messages
	windowClass.hInstance		= phInstance;				            // Set The Instance
	windowClass.hIcon			= LoadIcon(NULL, IDI_WINLOGO);          // Load default Icon
	windowClass.hbrBackground	= NULL;			                        // No Background required
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	windowClass.lpszClassName	= L"VisAlienfxRenderer";				            // Sets The Applications Classname
	if (RegisterClassEx (&windowClass) == 0)							// Did Registering The Class Fail?
	{
		MessageBox(phWnd,L"Registering Window Class failed",L"Error",MB_OK);
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof (PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24, //Color Depth
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24, //Depth Buffer
		0, //Stencil Buffer
		0, //Auxiliary Buffer
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	GLuint PixelFormat;

	RECT windowRect = {0, 0, _Width, _Height};

  AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);

	_hWnd = CreateWindowEx (windowExtendedStyle,					// Extended Style
								   L"VisAlienfxRenderer",	                        // Class Name
								   L"AlienFX Beat Detection",					            // Window Title
								   windowStyle,							// Window Style
								   0, 50,								// Window X,Y Position
								   windowRect.right - windowRect.left,	// Window Width
								   windowRect.bottom - windowRect.top,	// Window Height
								   phWnd,						// Desktop Is Window's Parent
								   0,									// No Menu
								   phInstance,
								   NULL);

	if (_hWnd == 0){
	  MessageBox(phWnd,L"CreateWindowEx failed",L"Error", MB_OK);
	  return false;
	}

	_hDC = GetDC (_hWnd);
	if (_hDC == 0)
	{
	  MessageBox(phWnd,L"Querying Drawing Context failed",L"Error", MB_OK);
		DestroyWindow (_hWnd);
		_hWnd = 0;
		return false;
	}

  //Wenn kein Multisamplecontext erstellet wurde
  //eignenen Context erstellen

  PixelFormat = ChoosePixelFormat (_hDC, &pfd);				        // Find A Compatible Pixel Format
  if (PixelFormat == 0)
    {
    ReleaseDC (_hWnd, _hDC);							                // Release Our Device Context
    _hDC = 0;												// Zero The Device Context
    DestroyWindow (_hWnd);									        // Destroy The Window
    _hWnd = 0;												// Zero The Window Handle
    MessageBox(phWnd,L"Choosing Pixel format failed",L"Error", MB_OK);
    return false;
  }

	if (SetPixelFormat (_hDC, PixelFormat, &pfd) == FALSE)		        // Try To Set The Pixel Format
	{
		ReleaseDC (_hWnd, _hDC);							                // Release Our Device Context
		_hDC = 0;												        // Zero The Device Context
		DestroyWindow (_hWnd);									        // Destroy The Window
		_hWnd = 0;												        // Zero The Window Handle
		MessageBox(phWnd,L"Setting Pixel format failed",L"Error", MB_OK);
		return false;
	}

	_hRC = wglCreateContext (_hDC);						        // Try To Get A Rendering Context
	if (_hRC == 0)
	{
		ReleaseDC (_hWnd, _hDC);							                // Release Our Device Context
		_hDC = 0;												        // Zero The Device Context
		DestroyWindow (_hWnd);									        // Destroy The Window
		_hWnd = 0;												        // Zero The Window Handle
		MessageBox(phWnd,L"Creating OpenGL context failed",L"Error", MB_OK);
		return false;
	}

	// Make The Rendering Context Our Current Rendering Context
	if (wglMakeCurrent (_hDC, _hRC) == FALSE)
	{
		wglDeleteContext (_hRC);									        // Delete The Rendering Context
		_hRC = 0;												// Zero The Rendering Context
		ReleaseDC (_hWnd, _hDC);							                // Release Our Device Context
		_hDC = 0;												// Zero The Device Context
		DestroyWindow (_hWnd);									        // Destroy The Window
		_hWnd = 0;												// Zero The Window Handle
		MessageBox(phWnd,L"Activating OpenGL Context failed",L"Error", MB_OK);
		return false;
	}

	//ShowWindow (_hWnd, SW_NORMAL);
	return true;
}

void ShowOpenGLWindow(){
	ShowWindow(_hWnd, SW_NORMAL);
	RenderWindowVisible = true;
}

bool DestroyOpenGLWindow(){
  if(_hWnd == 0){
    return false;
  }

	if (_hWnd != 0)												        // Does The Window Have A Handle?
	{
		if (_hDC != 0)											        // Does The Window Have A Device Context?
		{
			wglMakeCurrent (_hDC, 0);							        // Set The Current Active Rendering Context To Zero
			if (_hRC != 0)										        // Does The Window Have A Rendering Context?
			{
				wglDeleteContext (_hRC);							        // Release The Rendering Context
				_hRC = 0;										        // Zero The Rendering Context

			}
			ReleaseDC (_hWnd,_hDC);						                // Release The Device Context
			_hDC = 0;											        // Zero The Device Context
		}
		DestroyWindow (_hWnd);									        // Destroy The Window
		_hWnd = 0;												        // Zero The Window Handle
	}

	UnregisterClass(L"VisAlienfxRenderer",_hInstance);

  return true;
}

LRESULT CALLBACK WindowProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
      case WM_CLOSE:
          ShowWindow(_hWnd,SW_HIDE);
					RenderWindowVisible = false;
      break;

      case WM_DESTROY:
          return 0;

      case WM_SYSCOMMAND:												// Intercept System Commands
        switch (wParam)												// Check System Calls
        {
          case SC_SCREENSAVE:										// Screensaver Trying To Start?
          case SC_MONITORPOWER:									// Monitor Trying To Enter Powersave?
          return 0;												// Prevent From Happening
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;														// Exit

      default:
          return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  return 0;
}

void OpenGLSwapBuffers(){
  SwapBuffers(_hDC);
}
