#ifndef _WINDOW_H_
#define _WINDOW_H_
#include <windows.h>

bool CreateOpenGLWindow(HWND phWnd, HINSTANCE phInstance);
bool DestroyOpenGLWindow();
void OpenGLSwapBuffers();
void ShowOpenGLWindow();
LRESULT CALLBACK WindowProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // _WINDOW_H_

