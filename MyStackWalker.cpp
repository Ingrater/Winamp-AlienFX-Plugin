#include "MyStackWalker.h"
#include <stdio.h>
#include "main.h"
#include "XmlConfig.h"
#include "debug.h"

void MyStackWalker::OnOutput(LPCSTR szText){
	StackWalker::OnOutput(szText);
	std::string text = szText;
	dprintf(text.c_str());

  std::string pFilename = "%APPDATA%\\Winamp\\Plugins\\alienfx_vis_crashlog.txt";
  ReplaceEnvironmentVars(pFilename);
	FILE *datei = fopen(pFilename.c_str(),"a");
	if(!datei)
		return;
	fprintf(datei,szText);
}