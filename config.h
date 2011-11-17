#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <windows.h>
#include <boost/function.hpp>
#include <map>
#include "callback.h"

#ifdef CONFIG_MAIN_FILE
HWND ConfigWindow = NULL;
#else
extern HWND ConfigWindow;
#endif

bool CreateConfigWindow(HWND phWnd, HINSTANCE phInstance);
void DestroyConfigWindow();
void ShowConfigWindow();
void ConfigDraw(int i, float pEnergy, float pVariance);

#endif // _CONFIG_H_

