#include "TinyXml.h"
#include "debug.h"
#include "XmlConfig.h"
#include "alienfx.h"
#include "Zone.h"
#include <boost/scoped_array.hpp>

struct CleanUpAlienfx
{
	bool bDoCleanup;

	CleanUpAlienfx() : bDoCleanup(true) {}

	~CleanUpAlienfx()
	{
		if(bDoCleanup)
			AlienfxDeinit();
	}
};

bool parseHex(const char* in, int* out){
	if(!in)
		return false;
	int length = (int)strlen(in);
	if(length < 3 || in[0]!='0' || in[1] != 'x')
		return false;
	*out = 0;
	int digitValue = 1;
	for(int i=0;i<length-2;i++){
		int cur = length - i - 1;
		int value = 0;
		if(in[cur] >= '0' && in[cur] <= '9')
			value = in[cur] - '0';
		else if(in[cur] >= 'A' && in[cur] <= 'F')
			value = in[cur] - 'A' + 10;
		else if(in[cur] >= 'a' && in[cur] <= 'f')
			value = in[cur] - 'a' + 10;
		else
			return false;
		*out += value * digitValue;
		digitValue *= 16;
	}
	return true;
}

bool LoadXmlDevices()
{
  std::string pFilename = "%APPDATA%\\Winamp\\Plugins\\alienfx_vis_devices.xml";
  ReplaceEnvironmentVars(pFilename);

  FILE *datei = fopen(pFilename.c_str(),"r");
  if(datei == NULL){
    return false;
  }
  fclose(datei);

  TiXmlDocument XmlFile(pFilename.c_str());
  XmlFile.SetCondenseWhiteSpace(false);
  if(!XmlFile.LoadFile()){
    MessageBoxA(HWND_DESKTOP,XmlFile.ErrorDesc(),"Devices Xml Error", MB_OK);
    XmlFile = NULL;
    return false;
  }

	TiXmlNode* rootNode = XmlFile.FirstChild("devices");
	if(rootNode == NULL)
	{
		MessageBoxA(HWND_DESKTOP,"Couldn't find 'devices' root noed","Devices Xml Error", MB_OK);
		return false;
	}

	for(TiXmlNode* deviceNode = rootNode->FirstChild("device"); deviceNode != NULL; deviceNode = deviceNode->NextSibling("device"))
	{
		TiXmlElement* device = deviceNode->ToElement();
		if(device == NULL)
		{
			MessageBoxA(HWND_DESKTOP,"Device node is a non element","Devices Xml Error", MB_OK);
			return false;
		}
		
		const char *nameString = device->Attribute("name");
    if(nameString == NULL){
			MessageBoxA(HWND_DESKTOP,"Device node does not have a 'name' attribute","Devices Xml Error", MB_OK);
			return false;
		}
    
		int size = MultiByteToWideChar(CP_UTF8,0,nameString,-1,NULL,0);
    boost::scoped_array<wchar_t> nameWString(new wchar_t[size]);
    MultiByteToWideChar(CP_UTF8,0,nameString,-1,nameWString.get(),size);

		int protocolVersion = 1;
		int error = device->QueryIntAttribute("protocolVersion",&protocolVersion);
		if(error == TIXML_NO_ATTRIBUTE)
		{
			MessageBoxA(HWND_DESKTOP,"Device node does not have a 'protocolVersion' attribute","Devices Xml Error", MB_OK);
			return false;
		}
		else if(error == TIXML_WRONG_TYPE)
		{
			MessageBoxA(HWND_DESKTOP,"'protocolVersion' attribute ist not a number","Devices Xml Error", MB_OK);
			return false;
		}
		if(protocolVersion < 1 || protocolVersion > 2)
		{
			MessageBoxA(HWND_DESKTOP, "invalid 'protocolVersion' attribute value", "Devices Xml Error", MB_OK);
			return false;
		}

		int productId = -1;
		const char* productIdString = device->Attribute("productId");
		if(productIdString == NULL)
		{
			MessageBoxA(HWND_DESKTOP, "'device' node is missing 'productId' attribute", "Devices Xml Error", MB_OK);
			return false;
		}
		if(!parseHex(productIdString,&productId))
		{
			MessageBoxA(HWND_DESKTOP, "invalid 'productId' attribute value", "Devices Xml Error", MB_OK);
			return false;
		}

		if(AlienfxInitDevice(productId,protocolVersion))
		{
			CleanUpAlienfx cleanup;

			Zone::PrepareCustomModel();

			for(TiXmlElement* led = device->FirstChildElement("led"); led != NULL; led = led->NextSiblingElement("led"))
			{
				const char* ledNameString = led->Attribute("name");
				if(ledNameString == NULL)
				{
					MessageBoxA(HWND_DESKTOP, "'led' node is missing 'name' attribute", "Devices Xml Error", MB_OK);
					return false;
				}

				size = MultiByteToWideChar(CP_UTF8,0,ledNameString,-1,NULL,0);
				boost::scoped_array<wchar_t> ledNameWString(new wchar_t[size]);
				MultiByteToWideChar(CP_UTF8,0,ledNameString,-1,ledNameWString.get(),size);

				const char* codeString = led->Attribute("code");
				if(codeString == NULL)
				{
					MessageBoxA(HWND_DESKTOP, "'led' node is missing 'code' attribute", "Devices Xml Error", MB_OK);
					return false;
				}

				int code = 0;
				if(!parseHex(codeString,&code))
				{
					MessageBoxA(HWND_DESKTOP, "'led' node has a invalid 'code' attribute value", "Devices Xml Error", MB_OK);
					return false;
				}

				Zone::AddCustomLed(std::wstring(ledNameWString.get()),code);
			}

			cleanup.bDoCleanup = false;
			return true;
		}
	}

	return false;
}