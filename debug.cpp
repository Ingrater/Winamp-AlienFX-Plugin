#include "debug.h"
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include "XmlConfig.h"
using namespace std;


HANDLE __hStdOut = NULL;
FILE* g_logFile = NULL;


void dprintf(const char* fmt, ...){
	if(__hStdOut == NULL || fmt == NULL)
		return;
	char buffer[4096];
	size_t len = 0;
	va_list	ap;						
	va_start(ap, fmt);
	len = vsprintf_s(buffer, fmt, ap);							
	va_end(ap);
	len++;
	if(len >= 4095)
		len = 4095;
	buffer[len-1] = '\n';
	buffer[len] = '\0';
	int bytesWritten = 0;
	WriteConsoleA(__hStdOut,buffer,len,(LPDWORD)&bytesWritten,NULL);
}

void OpenDebugConsole(){
	AllocConsole();
	SetConsoleTitleA("Debug Window");
	__hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD co = {80,500};
	SetConsoleScreenBufferSize(__hStdOut, co);

  std::string pFilename = "%APPDATA%\\Winamp\\Plugins\\alienfx_vis.log";
  ReplaceEnvironmentVars(pFilename);
	g_logFile = fopen(pFilename.c_str(),"w");
}

void CloseDebugConsole()
{
	if(g_logFile != NULL)
		fclose(g_logFile);
	g_logFile = NULL;
	__hStdOut = NULL;
}