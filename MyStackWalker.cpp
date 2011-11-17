#include "MyStackWalker.h"
#include <stdio.h>
#include "main.h"
#include "XmlConfig.h"

void MyStackWalker::OnOutput(LPCSTR szText){
	StackWalker::OnOutput(szText);
	std::string text = szText;
	SendMyMessage(0,std::wstring(text.begin(),text.end()),0);

  std::string pFilename = "%APPDATA%\\Winamp\\Plugins\\alienfx_vis_crashlog.txt";
  ReplaceEnvironmentVars(pFilename);
	FILE *datei = fopen(pFilename.c_str(),"a");
	if(!datei)
		return;
	fprintf(datei,szText);
}