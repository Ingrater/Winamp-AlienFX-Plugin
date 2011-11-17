#include <string>
#include <sstream>
#include <windows.h>
#include <stdio.h>
#include "XmlConfig.h"
#include "TinyXml.h"
#include <boost/scoped_array.hpp>
#include "Slider.h"
#include "Zone.h"
#include "main.h"

TiXmlDocument *XmlFile = NULL;
TiXmlNode* MainNode = NULL;

extern Slider *EnergySlider[8];
extern Slider *VarianceSlider[8];
extern Slider *FactorSlider[2];
extern ListBox *ZoneSelect;
extern std::vector<Zone*> Zones;

bool ReplaceEnvironmentVars(std::string& str){
  bool found=true;
  char buffer[2048];
  std::stringstream ErrorMessage;
  char *varname;
  int varsize;
  while(found){
    int i,start=-1;
    for(i=0;i<str.size();i++){
      if(str[i] == '%'){
        if(start == -1)
          start = i;
        else{
          varsize = i-start -1;
          varname = new char[varsize+1];
          for(int j=0;j<varsize;j++)
            varname[j] = str[j+start+1];
          varname[varsize]='\0';
          if(!GetEnvironmentVariableA(varname,buffer,2048)){
            if(GetLastError() == ERROR_ENVVAR_NOT_FOUND){
              ErrorMessage << "Konnte Variable '" << varname << "' nicht finden!";
              MessageBoxA(NULL,ErrorMessage.str().c_str(),"Fehler",MB_OK | MB_ICONERROR);
              return false;
            }
          }
          str.replace(start,i-start+1,buffer,strlen(buffer));
          delete []varname;
          break;
        }
      }
    }
    if(i==str.size())
      found=false;
  }
  return true;
}

void LoadXmlConfigPart1(size_t& pDelay){
  TiXmlNode* Child;
  TiXmlElement *Element;

  std::string pFilename = "%APPDATA%\\Winamp\\Plugins\\alienfx_vis_config.xml";
  ReplaceEnvironmentVars(pFilename);

  FILE *datei = fopen(pFilename.c_str(),"r");
  if(datei == NULL){
    std::stringstream ErrorMessage;
    ErrorMessage << "Couldn't open 'alienfx_vis_config.xml'\n" << pFilename.c_str();
    MessageBoxA(HWND_DESKTOP, ErrorMessage.str().c_str(), "Error", MB_OK);
    return;
  }
  fclose(datei);

  XmlFile = new TiXmlDocument(pFilename.c_str());
  XmlFile->SetCondenseWhiteSpace(false);
  if(!XmlFile->LoadFile()){
    MessageBoxA(HWND_DESKTOP,XmlFile->ErrorDesc(),"Error", MB_OK);
    delete XmlFile;
    XmlFile = NULL;
    return;
  }

  //Nach Main Node suchen
  for(MainNode = XmlFile->FirstChild(); MainNode != 0; MainNode = MainNode->NextSibling()){
    if(MainNode->Type() == TiXmlNode::ELEMENT){
      if(std::string("config") == MainNode->Value())
        break; //MainNode gefunden
    }
  }

  if(MainNode == 0){
    MessageBox(HWND_DESKTOP,L"Couldn't find main node 'config'",L"Error",MB_OK);
    return;
  }

  for(Child = MainNode->FirstChild(); Child != 0; Child = Child->NextSibling()){
    if(Child->Type() == TiXmlNode::ELEMENT){
      Element = Child->ToElement();
      if(std::string("delay") == Child->Value()){
        int delay;
        if(Element->QueryIntAttribute("value",&delay) == TIXML_SUCCESS){
          if(delay >= 0)
            pDelay = delay;
        }
      }
      else if(std::string("energy") == Child->Value()
              || std::string("variance") == Child->Value()
              || std::string("factor") == Child->Value()
              || std::string("model") == Child->Value()){
        //Do nothing
      }
      else {
        std::stringstream ErrorMessage;
        ErrorMessage << "Unkown config value '" << Child->Value() << "'";
        MessageBoxA(HWND_DESKTOP,ErrorMessage.str().c_str(),"XML Error", MB_OK);
      }
    }
  }
}

bool XmlLoadSliders(TiXmlElement* pFather, Slider** pSliders, int pSize){
  TiXmlNode *Child;
  TiXmlElement *Element;
  for(Child = pFather->FirstChild(); Child != 0; Child = Child->NextSibling()){
    if(Child->Type() == TiXmlNode::ELEMENT){
      Element = Child->ToElement();
      if(std::string("slider") == Child->Value()){
        int id = 0;
        float value = 0.0f;
        int err = Element->QueryIntAttribute("id",&id);
        if(err == TIXML_NO_ATTRIBUTE){
          MessageBoxA(HWND_DESKTOP,"Couldn't find id attribute in slider element","XML Error",MB_OK | MB_ICONERROR);
          return false;
        }
        else if(err == TIXML_WRONG_TYPE){
          MessageBoxA(HWND_DESKTOP,"id attribute in slider element has wrong type","XML Error",MB_OK | MB_ICONERROR);
          return false;
        }
        else if(id >= pSize || id < 0){
          MessageBoxA(HWND_DESKTOP,"id attribute in slider element is out of range","XML Error",MB_OK | MB_ICONERROR);
          return false;
        }

        err = Element->QueryFloatAttribute("value",&value);
        if(err == TIXML_NO_ATTRIBUTE){
          MessageBoxA(HWND_DESKTOP,"Couldn't fint value attribute in slider element","XML Error",MB_OK | MB_ICONERROR);
          return false;
        }
        else if(err == TIXML_WRONG_TYPE){
          MessageBoxA(HWND_DESKTOP,"value attribute in slider element has wrong type","XML Error",MB_OK | MB_ICONERROR);
          return false;
        }
        pSliders[id]->SetValue(value);
      }
      else {
        std::stringstream ErrorMessage;
        ErrorMessage << "Warning: Unkown element '" << Child->Value() << "'";
        MessageBoxA(HWND_DESKTOP,ErrorMessage.str().c_str(),"XML Warning", MB_OK);
      }
    }
  }
  return true;
}

bool LoadXmlConfigPart2(){
  if(MainNode == NULL)
    return false;

  TiXmlNode *Child = MainNode->FirstChild("energy");
  if(Child == NULL){
    MessageBoxA(HWND_DESKTOP,"energy element not found","XML Warning",MB_OK);
  }
  else if(Child->Type() != TiXmlNode::ELEMENT){
    MessageBoxA(HWND_DESKTOP,"energy node is not a element","XML Error",MB_OK | MB_ICONERROR);
    return false;
  }
  else XmlLoadSliders(Child->ToElement(),EnergySlider,8);

  Child = MainNode->FirstChild("variance");
  if(Child == NULL){
    MessageBoxA(HWND_DESKTOP,"variance element not found","XML Warning",MB_OK);
  }
  else if(Child->Type() != TiXmlNode::ELEMENT){
    MessageBoxA(HWND_DESKTOP,"variance node ist not an element","XML Error",MB_OK | MB_ICONERROR);
    return false;
  }
  else XmlLoadSliders(Child->ToElement(),VarianceSlider,8);

  Child = MainNode->FirstChild("factor");
  if(Child == NULL){
    MessageBoxA(HWND_DESKTOP,"factor element not found","XML Warning",MB_OK);
  }
  else if(Child->Type() != TiXmlNode::ELEMENT){
    MessageBoxA(HWND_DESKTOP,"factor node ist not an element","XML Error",MB_OK | MB_ICONERROR);
    return false;
  }
  else XmlLoadSliders(Child->ToElement(),FactorSlider,2);

  //Load Model
  /*Child = MainNode->FirstChild("model");
  if(Child == NULL){
    MessageBoxA(HWND_DESKTOP,"model element not found","XML Warning",MB_OK);
    Zone::SetModel(AREA_51_M15X);
  }
  else {
    if(Child->Type() != TiXmlNode::ELEMENT){
      MessageBoxA(HWND_DESKTOP,"model element is not an element","XML Error",MB_OK | MB_ICONERROR);
      return false;
    }
    int value;
    TiXmlElement *Element = Child->ToElement();
    int res = Element->QueryIntAttribute("value",&value);
    if(res == TIXML_NO_ATTRIBUTE){
      MessageBoxA(HWND_DESKTOP,"value attribute of model element not found","XML Error",MB_OK | MB_ICONERROR);
      return false;
    }
    else if(res == TIXML_WRONG_TYPE){
      MessageBoxA(HWND_DESKTOP,"value attribute of model element has wrong type","XML Error",MB_OK | MB_ICONERROR);
      return false;
    }
    else if(value < FIRST_MODEL || value > LAST_MODEL){
      MessageBoxA(HWND_DESKTOP,"unkown model","XML Error",MB_OK | MB_ICONERROR);
      return false;
    }
    Zone::SetModel((SupportedModels)value);
  }*/

  XmlLoadSettings();

  return true;
}

void XmlSaveSliders(TiXmlElement *pFather, Slider **pSliders, int pSize){
  boost::scoped_array<bool> Done(new bool[pSize]);
  for(int i=0;i<pSize;i++)
    Done[i] = false;

  for(TiXmlNode *Child = pFather->FirstChild(); Child != 0; Child = Child->NextSibling()){
    if(Child->Type() == TiXmlNode::ELEMENT){
      if(std::string("slider") == Child->Value()){
        TiXmlElement *Element = Child->ToElement();
        int id = -1;
        Element->QueryIntAttribute("id",&id);
        if(id >= 0 && id < pSize){
          Element->SetDoubleAttribute("value",pSliders[id]->GetValue());
          Done[id] = true;
        }
      }
    }
  }

  for(int i=0;i<pSize;i++){
    if(!Done[i]){
      TiXmlElement *Element = new TiXmlElement("slider");
      Element->SetAttribute("id",i);
      Element->SetDoubleAttribute("value",pSliders[i]->GetValue());
      pFather->LinkEndChild(Element);
    }
  }
}

void XmlSave(){
  if(MainNode != NULL){
    TiXmlNode *Child = MainNode->FirstChild("energy");
    if(Child == NULL){
      TiXmlElement *Element = new TiXmlElement("energy");
      MainNode->LinkEndChild(Element);
      Child = Element;
    }
    else if(Child->Type() != TiXmlNode::ELEMENT){
      MainNode->RemoveChild(Child);
      TiXmlElement *Element = new TiXmlElement("energy");
      MainNode->LinkEndChild(Element);
      Child = Element;
    }
    XmlSaveSliders(Child->ToElement(),EnergySlider,8);

    Child = MainNode->FirstChild("variance");
    if(Child == NULL){
      TiXmlElement *Element = new TiXmlElement("variance");
      MainNode->LinkEndChild(Element);
      Child = Element;
    }
    else if(Child->Type() != TiXmlNode::ELEMENT){
      MainNode->RemoveChild(Child);
      TiXmlElement *Element = new TiXmlElement("variance");
      MainNode->LinkEndChild(Element);
      Child = Element;
    }
    XmlSaveSliders(Child->ToElement(),VarianceSlider,8);

    Child = MainNode->FirstChild("factor");
    if(Child == NULL){
      TiXmlElement *Element = new TiXmlElement("factor");
      MainNode->LinkEndChild(Element);
      Child = Element;
    }
    else if(Child->Type() != TiXmlNode::ELEMENT){
      MainNode->RemoveChild(Child);
      TiXmlElement *Element = new TiXmlElement("factor");
      MainNode->LinkEndChild(Element);
      Child = Element;
    }
    XmlSaveSliders(Child->ToElement(),FactorSlider,2);

    std::string pFilename = "%APPDATA%\\Winamp\\Plugins\\alienfx_vis_config.xml";
    ReplaceEnvironmentVars(pFilename);

    XmlFile->SaveFile(pFilename.c_str());
  }
  XmlSaveSettings();
}

void XmlDeinit(){
  XmlSave();

  delete XmlFile;
  XmlFile = NULL;
}

void XmlLoadSettings(){
  std::string pFilename = "%APPDATA%\\Winamp\\Plugins\\alienfx_vis_settings.xml";
  ReplaceEnvironmentVars(pFilename);

  FILE *datei = fopen(pFilename.c_str(),"r");
  if(datei == NULL){
    std::stringstream ErrorMessage;
    ErrorMessage << "Couldn't open 'alienfx_vis_settings.xml'\n" << pFilename.c_str();
    MessageBoxA(HWND_DESKTOP, ErrorMessage.str().c_str(), "Warning", MB_OK);
    return;
  }
  fclose(datei);

  TiXmlDocument SettingsFile(pFilename.c_str());
  SettingsFile.SetCondenseWhiteSpace(false);
  if(!SettingsFile.LoadFile()){
    MessageBoxA(HWND_DESKTOP,SettingsFile.ErrorDesc(),"Error", MB_OK);
    return;
  }

  TiXmlElement *MainNode = SettingsFile.FirstChildElement("settings");
  if(MainNode){
    TiXmlElement *model = MainNode->FirstChildElement("model");
    if(model){
      int eModel;
      if(model->QueryIntAttribute("value",&eModel) == TIXML_SUCCESS){
        if((SupportedModels)eModel != Zone::GetModel()){
          MessageBoxA(HWND_DESKTOP,"Settings where not loaded because the model of the settings file does not match the current model","Error", MB_OK);
          return;
        }
      }
    }

    for(size_t i=0;i<Zones.size();i++){
      delete Zones[i];
    }
    Zones.resize(0);

    if(ZoneSelect){
      ZoneSelect->RemoveAllEntries();
    }

    TiXmlElement *zones = MainNode->FirstChildElement("zones");
    if(zones){
      Zone::PreXmlLoad();
      int count = 0;
      for(TiXmlElement *zone=zones->FirstChildElement("zone");zone!=0;zone=zone->NextSiblingElement("zone")){
        if(count >= MAX_ZONE_NUM){
          break;
        }
        Zone *ZoneInstance = new Zone();
        ZoneInstance->FromXml(zone);
        Zones.push_back(ZoneInstance);
        ZoneSelect->AppendEntry(ZoneInstance->GetName().c_str(),ZoneInstance->GetId());
        count++;
      }
      Zone::StaticFromXml(zones);
    }
  }
}

void XmlSaveSettings(){
  std::string pFilename = "%APPDATA%\\Winamp\\Plugins\\alienfx_vis_settings.xml";
  ReplaceEnvironmentVars(pFilename);

	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );

	TiXmlElement *MainNode = new TiXmlElement("settings");
	doc.LinkEndChild(MainNode);

	TiXmlElement *Model = new TiXmlElement("model");
	Model->SetAttribute("value",(int)Zone::GetModel());
	MainNode->LinkEndChild(Model);

	TiXmlElement *zones = Zone::StaticToXml();
	for(size_t i=0;i<Zones.size();i++){
    TiXmlElement *zone = Zones[i]->ToXml();
    zones->LinkEndChild(zone);
	}
	MainNode->LinkEndChild(zones);

	doc.SaveFile(pFilename.c_str());
}
