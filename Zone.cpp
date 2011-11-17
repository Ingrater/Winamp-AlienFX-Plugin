#include "Zone.h"
#include "alienfx.h"
#include "resource.h"
#include <boost/scoped_array.hpp>
#include <math.h>

void SendMyMessage(int pId, const std::wstring& pString, int pRGB);

extern std::vector<Zone*> Zones;

SupportedModels Zone::_Model = AREA_51_M15X;
std::vector<Zone::Led> Zone::_Leds;
Zone *Zone::_FreqBandUsage[32];
COLORREF Zone::_CustomColors[16];

Zone::Zone(int pId,const wchar_t *pName, int pColor, bool pRandomColor) :
_Id(pId),
_Color(pColor),
_RandomColor(pRandomColor),
_Name(pName){
}

Zone::~Zone(){
  for(int i=0;i<32;i++){
    if(_FreqBandUsage[i] == this)
      _FreqBandUsage[i] = NULL;
  }
  for(size_t i=0;i<_LedsInUse.size();i++){
    _Leds[_LedsInUse[i]]._Used = false;
  }
}

void Zone::AddCustomLed(const char* name, int ledBit){
	size_t oldSize = _Leds.size();
	_Leds.resize(oldSize + 1);

	std::string sname = name;

	_Leds[oldSize]._Name = std::wstring(sname.begin(),sname.end());
	_Leds[oldSize]._LedBit = ledBit;
	_Leds[oldSize]._Used = false;
}

void Zone::SetModel(SupportedModels pModel){
  for(int i=0;i<32;i++)
    _FreqBandUsage[i] = NULL;

  _Model = pModel;
  _Leds.clear();
  switch(_Model){
    case AREA_51_M15X:
      _Leds.resize(6);

      _Leds[0]._Name = L"Touchpad";
      _Leds[0]._LedBit = ALIENFX_A51_M15X_TOUCHPAD;
      _Leds[0]._Used = false;

      _Leds[1]._Name = L"Lightpipe";
      _Leds[1]._LedBit = ALIENFX_A51_M15X_LIGHTPIPE;
      _Leds[1]._Used = false;

      _Leds[2]._Name = L"Alienware Logo";
      _Leds[2]._LedBit = ALIENFX_A51_M15X_ALIENWARE_LOGO;
      _Leds[2]._Used = false;

      _Leds[3]._Name = L"Alienhead";
      _Leds[3]._LedBit = ALIENFX_A51_M15X_ALIENHEAD;
      _Leds[3]._Used = false;

      _Leds[4]._Name = L"Keyboard";
      _Leds[4]._LedBit = ALIENFX_A51_M15X_KEYBOARD;
      _Leds[4]._Used = false;

      _Leds[5]._Name = L"Touchpanel";
      _Leds[5]._LedBit = ALIENFX_A51_M15X_TOUCH_PANEL;
      _Leds[5]._Used = false;
      break;
    case ALL_POWERFULL_M17X:
      _Leds.resize(10);

      _Leds[0]._Name = L"Keyboard right";
      _Leds[0]._LedBit = ALIENFX_AP_M17X_KEYBOARD_RIGHT;
      _Leds[0]._Used = false;

      _Leds[1]._Name = L"Keyboard middle right";
      _Leds[1]._LedBit = ALIENFX_AP_M17X_KEYBOARD_MIDDLE_RIGHT;
      _Leds[1]._Used = false;

      _Leds[2]._Name = L"Keyboard middle left";
      _Leds[2]._LedBit = ALIENFX_AP_M17X_KEYBOARD_MIDDLE_LEFT;
      _Leds[2]._Used = false;

      _Leds[3]._Name = L"Keyboard left";
      _Leds[3]._LedBit = ALIENFX_AP_M17X_KEYBOARD_LEFT;
      _Leds[3]._Used = false;

      _Leds[4]._Name = L"right speaker";
      _Leds[4]._LedBit = ALIENFX_AP_M17X_RIGHT_SPEAKER;
      _Leds[4]._Used = false;

      _Leds[5]._Name = L"left speaker";
      _Leds[5]._LedBit = ALIENFX_AP_M17X_LEFT_SPEAKER;
      _Leds[5]._Used = false;

      _Leds[6]._Name = L"alienhead";
      _Leds[6]._LedBit = ALIENFX_AP_M17X_ALIENHEAD;
      _Leds[6]._Used = false;

      _Leds[7]._Name = L"alienware logo";
      _Leds[7]._LedBit = ALIENFX_AP_M17X_ALIENWARE_LOGO;
      _Leds[7]._Used = false;

      _Leds[8]._Name = L"touchpanel";
      _Leds[8]._LedBit = ALIENFX_AP_M17X_TOUCHPANEL;
      _Leds[8]._Used = false;

      _Leds[9]._Name = L"touchpad";
      _Leds[9]._LedBit = ALIENFX_AP_M17X_TOUCHPAD;
      _Leds[9]._Used = false;
      break;
    case ALL_POWERFULL_M15X:
      _Leds.resize(10);

      _Leds[0]._Name = L"Keyboard right";
      _Leds[0]._LedBit = ALIENFX_AP_M15X_KEYBOARD_RIGHT;
      _Leds[0]._Used = false;

      _Leds[1]._Name = L"Keyboard middle right";
      _Leds[1]._LedBit = ALIENFX_AP_M15X_KEYBOARD_MIDDLE_RIGHT;
      _Leds[1]._Used = false;

      _Leds[2]._Name = L"Keyboard middle left";
      _Leds[2]._LedBit = ALIENFX_AP_M15X_KEYBOARD_MIDDLE_LEFT;
      _Leds[2]._Used = false;

      _Leds[3]._Name = L"Keyboard left";
      _Leds[3]._LedBit = ALIENFX_AP_M15X_KEYBOARD_LEFT;
      _Leds[3]._Used = false;

      _Leds[4]._Name = L"right speaker";
      _Leds[4]._LedBit = ALIENFX_AP_M15X_RIGHT_SPEAKER;
      _Leds[4]._Used = false;

      _Leds[5]._Name = L"left speaker";
      _Leds[5]._LedBit = ALIENFX_AP_M15X_LEFT_SPEAKER;
      _Leds[5]._Used = false;

      _Leds[6]._Name = L"alienhead";
      _Leds[6]._LedBit = ALIENFX_AP_M15X_ALIENHEAD;
      _Leds[6]._Used = false;

      _Leds[7]._Name = L"alienware logo";
      _Leds[7]._LedBit = ALIENFX_AP_M15X_ALIENWARE_LOGO;
      _Leds[7]._Used = false;

      _Leds[8]._Name = L"touchpanel";
      _Leds[8]._LedBit = ALIENFX_AP_M15X_TOUCHPANEL;
      _Leds[8]._Used = false;

      _Leds[9]._Name = L"touchpad";
      _Leds[9]._LedBit = ALIENFX_AP_M15X_TOUCHPAD;
      _Leds[9]._Used = false;
      break;
    case ALL_POWERFULL_M11X:
      _Leds.resize(5);

      _Leds[0]._Name = L"keyboard";
      _Leds[0]._LedBit = ALIENFX_AP_M11X_KEYBOARD;
      _Leds[0]._Used = false;

      _Leds[1]._Name = L"right speaker";
      _Leds[1]._LedBit = ALIENFX_AP_M11X_RIGHT_SPEAKER;
      _Leds[1]._Used = false;

      _Leds[2]._Name = L"left speaker";
      _Leds[2]._LedBit = ALIENFX_AP_M11X_LEFT_SPEAKER;
      _Leds[2]._Used = false;

      _Leds[3]._Name = L"alienware logo";
      _Leds[3]._LedBit = ALIENFX_AP_M11X_ALIENWARE_LOGO;
      _Leds[3]._Used = false;

      _Leds[4]._Name = L"touchpanel";
      _Leds[4]._LedBit = ALIENFX_AP_M11X_TOUCHPANEL;
      _Leds[4]._Used = false;
      break;
    case M17X_R3:
      _Leds.resize(9);

      _Leds[0]._Name = L"Keyboard right";
      _Leds[0]._LedBit = ALIENFX_M17X_R3_KEYBOARD_RIGHT;
      _Leds[0]._Used = false;

      _Leds[1]._Name = L"Keyboard middle right";
      _Leds[1]._LedBit = ALIENFX_M17X_R3_KEYBOARD_MIDDLE_RIGHT;
      _Leds[1]._Used = false;

      _Leds[2]._Name = L"Keyboard middle left";
      _Leds[2]._LedBit = ALIENFX_M17X_R3_KEYBOARD_MIDDLE_LEFT;
      _Leds[2]._Used = false;

      _Leds[3]._Name = L"Keyboard left";
      _Leds[3]._LedBit = ALIENFX_M17X_R3_KEYBOARD_LEFT;
      _Leds[3]._Used = false;

      _Leds[4]._Name = L"right speaker";
      _Leds[4]._LedBit = ALIENFX_M17X_R3_LEFT_SPEAKER;
      _Leds[4]._Used = false;

      _Leds[5]._Name = L"left speaker";
      _Leds[5]._LedBit = ALIENFX_M17X_R3_RIGHT_SPEAKER;
      _Leds[5]._Used = false;

      _Leds[6]._Name = L"alienware logo";
      _Leds[6]._LedBit = ALIENFX_M17X_R3_ALIENWARE_LOGO;
      _Leds[6]._Used = false;

      _Leds[7]._Name = L"touchpanel";
      _Leds[7]._LedBit = ALIENFX_M17X_R3_TOUCHPANEL;
      _Leds[7]._Used = false;

      _Leds[8]._Name = L"touchpad";
      _Leds[8]._LedBit = ALIENFX_M17X_R3_TOUCHPAD;
      _Leds[8]._Used = false;
      break;
		case CUSTOM_MODEL:
			break;
  }
}

void Zone::AddFreqBand(int pIndex){
  if(pIndex >= 0 && pIndex < 32){
    _FreqBandUsage[pIndex] = this;
  }
}

void Zone::RemoveFreqBand(int pIndex){
  if(pIndex >= 0 && pIndex < 32){
    if(_FreqBandUsage[pIndex] == this)
      _FreqBandUsage[pIndex] = NULL;
  }
}

void Zone::AddLed(int pIndex){
  if(pIndex < 0 || pIndex >= _Leds.size()){
    SendMyMessage(0,std::wstring(L"AddLed out of bounds"),0);
    return;
  }
  if(!_Leds[pIndex]._Used){
    _LedsInUse.push_back(pIndex);
    _Leds[pIndex]._Used = true;
  }
}

void Zone::RemoveLed(int pIndex){
  if(pIndex >= 0 && static_cast<size_t>(pIndex) < _LedsInUse.size()){
    int Led = _LedsInUse[pIndex];
    if(Led < 0 || Led >= _Leds.size()){
      SendMyMessage(0,std::wstring(L"RemoveLed Led out of bounds"),0);
      return;
    }
    _Leds[Led]._Used = false;
    _LedsInUse.erase(_LedsInUse.begin()+pIndex);
  }
  else {
    SendMyMessage(0,std::wstring(L"RemoveLed out of bounds"),0);
  }
}

void Zone::EditDialog(HINSTANCE hInstance, HWND hWnd){
  DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DLG2),hWnd,(DLGPROC)DialogProc,(LPARAM)this);
  GenerateLedBits();
}

LRESULT CALLBACK Zone::DialogProc(HWND hDlg, UINT uMsg,WPARAM wParam, LPARAM lParam){
  Zone *self = NULL;
  DialogData *data = (DialogData*)GetWindowLongPtr(hDlg,GWLP_USERDATA);
  if(data != NULL)
    self = data->self;
  switch( uMsg )
  {
    case WM_INITDIALOG:
      {
        Zone *self = (Zone*)lParam;
        DialogData *data = new DialogData;
        SetWindowLongPtr(hDlg,GWLP_USERDATA,(LONG)data);

        data->self = self;
        data->LedBoxLeft = new ListBox(GetDlgItem(hDlg,IDC_LST3));
        data->LedBoxRight = new ListBox(GetDlgItem(hDlg,IDC_LST4));
        data->FreqBoxLeft = new ListBox(GetDlgItem(hDlg,IDC_LST1));
        data->FreqBoxRight = new ListBox(GetDlgItem(hDlg,IDC_LST2));

        //Display name
        SetDlgItemText(hDlg,IDC_EDT2,self->GetName().c_str());

        //Display leds
        const std::vector<int>& LedsInUse = self->GetLedsInUse();
        for(size_t i=0;i<LedsInUse.size();i++){
          data->LedBoxLeft->AppendEntry(_Leds[LedsInUse[i]]._Name.c_str(),LedsInUse[i]);
        }
        for(size_t i=0;i<_Leds.size();i++){
          if(!_Leds[i]._Used)
            data->LedBoxRight->AppendEntry(_Leds[i]._Name.c_str(),i);
        }

        //Display frequency bands
        for(size_t i=0;i<32;i++){
          wchar_t number[64];
          swprintf(number,L"%i",i+1);
          if(_FreqBandUsage[i] == self){
            data->FreqBoxLeft->AppendEntry(number,i);
          }
          else if(_FreqBandUsage[i] == NULL){
            data->FreqBoxRight->AppendEntry(number,i);
          }
        }

        //Display color
        boost::scoped_array<int> BmpData(new int[10*10]);
        for(int i=0;i<10*10;i++){
          BmpData[i] = self->_Color;
        }
        HBITMAP bitmap = CreateBitmap(10,10,1,32,BmpData.get());
        SendMessage(GetDlgItem(hDlg,IDC_BTN7),BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)bitmap);

        //Display random color
        if(self->_RandomColor)
          SendMessage(GetDlgItem(hDlg,IDC_CHK1),BM_SETCHECK,(WPARAM)BST_CHECKED,(LPARAM)NULL);
        else
          SendMessage(GetDlgItem(hDlg,IDC_CHK1),BM_SETCHECK,(WPARAM)BST_UNCHECKED,(LPARAM)NULL);
      }
      break;
    case WM_CLOSE:
      {
        wchar_t buffer[128];
        GetDlgItemText(hDlg,IDC_EDT2,buffer,128);
        self->SetName(buffer);

        delete data;
        EndDialog( hDlg, IDCANCEL );
      }
      break;
    case WM_COMMAND:
      switch(LOWORD(wParam)){
        case IDC_BTN3:
          {
            int sel = data->FreqBoxRight->GetSelection();
            if(sel < 0){
              MessageBoxA(hDlg,"You have to select a unused frequency band from the right listbox!","Error",MB_OK);
              return FALSE;
            }
            int id = data->FreqBoxRight->GetData(sel);
            data->FreqBoxLeft->AppendEntry(data->FreqBoxRight->GetText(sel).c_str(),id);
            self->AddFreqBand(id);
            data->FreqBoxRight->RemoveEntry(sel);
          }
          break;
        case IDC_BTN4:
          {
            int sel = data->FreqBoxLeft->GetSelection();
            if(sel < 0){
              MessageBoxA(hDlg,"You have to select a used frequency band from the left listbox!","Error",MB_OK);
              return FALSE;
            }
            int id = data->FreqBoxLeft->GetData(sel);
            data->FreqBoxRight->AppendEntry(data->FreqBoxLeft->GetText(sel).c_str(),id);
            self->RemoveFreqBand(id);
            data->FreqBoxLeft->RemoveEntry(sel);
          }
          break;
        case IDC_BTN5:
          {
            SendMyMessage(0,std::wstring(L"Adding Led"),0);
            if(self == NULL || data == NULL){
              SendMyMessage(0,std::wstring(L"self or data is NULL"),0);
              return FALSE;
            }
            int sel = data->LedBoxRight->GetSelection();
            if(sel < 0){
              MessageBoxA(hDlg,"You have to select a unused led from the right listbox","Error",MB_OK);
              return FALSE;
            }
            int id = data->LedBoxRight->GetData(sel);
            if(data->LedBoxRight->GetText(sel).c_str() == NULL){
              SendMyMessage(0,std::wstring(L"Text is NULL"),0);
              return FALSE;
            }
            data->LedBoxLeft->AppendEntry(data->LedBoxRight->GetText(sel).c_str(),id);
            self->AddLed(id);
            data->LedBoxRight->RemoveEntry(sel);
            self->GenerateLedBits();
          }
          break;
        case IDC_BTN6:
          {
            int sel = data->LedBoxLeft->GetSelection();
            if( sel < 0){
              MessageBoxA(hDlg,"You have to select a used led from the left listbox","Error",MB_OK);
              return FALSE;
            }
            int id = data->LedBoxLeft->GetData(sel);
            data->LedBoxRight->AppendEntry(data->LedBoxLeft->GetText(sel).c_str(),id);
            self->RemoveLed(sel);
            data->LedBoxLeft->RemoveEntry(sel);
            self->GenerateLedBits();
          }
          break;
        case IDC_BTN8:
          {
            wchar_t buffer[128];
            GetDlgItemText(hDlg,IDC_EDT2,buffer,128);
            self->SetName(buffer);

            delete data;
            EndDialog( hDlg, IDCANCEL );
          }
          break;
        case IDC_BTN7:
          {
            CHOOSECOLOR cc;
            cc.lStructSize = sizeof(cc);
            cc.hwndOwner = hDlg;
            cc.hInstance = NULL;
            cc.rgbResult = ((data->self->_Color >> 16) & 0x0000FF) |
                           ((data->self->_Color      ) & 0x00FF00) |
                           ((data->self->_Color << 16) & 0xFF0000);
            cc.lpCustColors = _CustomColors;
            cc.Flags = CC_ANYCOLOR | CC_RGBINIT;
            cc.lCustData = NULL;
            cc.lpfnHook = NULL;
            cc.lpTemplateName = NULL;

            if(ChooseColor(&cc)){
              data->self->_Color = ((cc.rgbResult >> 16) & 0x0000FF) |
                                   ((cc.rgbResult      ) & 0x00FF00) |
                                   ((cc.rgbResult << 16) & 0xFF0000);

              wchar_t debug[256];
              swprintf(debug,L"Color is %x",data->self->_Color);
              SendMyMessage(0,std::wstring(debug),0);

              //Display color
              HBITMAP oldBitmap = (HBITMAP)SendMessage(GetDlgItem(hDlg,IDC_BTN7),BM_GETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)NULL);

              boost::scoped_array<int> BmpData(new int[10*10]);
              for(int i=0;i<10*10;i++){
                BmpData[i] = self->_Color;
              }
              HBITMAP bitmap = CreateBitmap(10,10,1,32,BmpData.get());
              SendMessage(GetDlgItem(hDlg,IDC_BTN7),BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)bitmap);

              if(oldBitmap)
                DeleteObject(oldBitmap);
            }
          }
          break;
        case IDC_CHK1:
          {
            if(SendMessage(GetDlgItem(hDlg,IDC_CHK1),BM_GETCHECK,(WPARAM)NULL,(LPARAM)NULL) == BST_CHECKED){
              self->_RandomColor = true;
            }
            else {
              self->_RandomColor = false;
            }
          }
          break;
      }
      break;

    default :
      return (FALSE);
  }

  return(TRUE);
}

TiXmlElement* Zone::ToXml(){
  TiXmlElement *el = new TiXmlElement("zone");

  TiXmlElement *LedsInUse = new TiXmlElement("ledsinuse");
  el->LinkEndChild(LedsInUse);

  for(size_t i=0;i<_LedsInUse.size();i++){
    TiXmlElement *led = new TiXmlElement("led");
    led->SetAttribute("id",_LedsInUse[i]);
    LedsInUse->LinkEndChild(led);
  }

  TiXmlElement *id = new TiXmlElement("id");
  id->SetAttribute("value",_Id);
  el->LinkEndChild(id);

  TiXmlElement *color = new TiXmlElement("color");
  color->SetAttribute("value",_Color);
  el->LinkEndChild(color);

  TiXmlElement *random = new TiXmlElement("random");
  random->SetAttribute("value",(int)_RandomColor);
  el->LinkEndChild(random);

  TiXmlElement *name = new TiXmlElement("name");

  //convert name to utf8
  int size = WideCharToMultiByte(CP_UTF8,0,_Name.c_str(),-1,NULL,0,NULL,NULL);
  boost::scoped_array<char> NameStr(new char[size]);

  WideCharToMultiByte(CP_UTF8,0,_Name.c_str(),-1,NameStr.get(),size,NULL,NULL);

  name->SetAttribute("value",NameStr.get());
  el->LinkEndChild(name);
  return el;
}

void Zone::FromXml(TiXmlElement *pElement){
  _LedsInUse.resize(0);

  TiXmlElement *LedsInUse = pElement->FirstChildElement("ledsinuse");
  if(LedsInUse){
    for(TiXmlNode *pNode = LedsInUse->FirstChild();pNode!=0;pNode=pNode->NextSibling()){
      if(pNode->Type() == TiXmlNode::ELEMENT){
        TiXmlElement *led = pNode->ToElement();
        if(pNode->Value() == std::string("led")){
          int LedId;
          if(led->QueryIntAttribute("id",&LedId) == TIXML_SUCCESS){
            _LedsInUse.push_back(LedId);
            _Leds[LedId]._Used = true;
          }
        }
      }
    }
  }

  TiXmlElement *id = pElement->FirstChildElement("id");
  if(id){
    id->QueryIntAttribute("value",&_Id);
  }

  TiXmlElement *color = pElement->FirstChildElement("color");
  if(color){
    color->QueryIntAttribute("value",&_Color);
  }

  TiXmlElement *random = pElement->FirstChildElement("random");
  if(random){
    int RandomValue = 0;
    random->QueryIntAttribute("value",&RandomValue);
    _RandomColor = (bool)RandomValue;
  }

  TiXmlElement *name = pElement->FirstChildElement("name");
  if(name){
    const char *NameString = name->Attribute("value");
    if(NameString){
      int size = MultiByteToWideChar(CP_UTF8,0,NameString,-1,NULL,0);
      boost::scoped_array<wchar_t> NameWString(new wchar_t[size]);
      MultiByteToWideChar(CP_UTF8,0,NameString,-1,NameWString.get(),size);
      _Name = NameWString.get();
    }
  }

  GenerateLedBits();
}

TiXmlElement* Zone::StaticToXml(){
  TiXmlElement *zones = new TiXmlElement("zones");

  TiXmlElement *FreqBandUsage = new TiXmlElement("freqbandusage");
  for(int i=0;i<32;i++){
    TiXmlElement *band = new TiXmlElement("band");
    if(_FreqBandUsage[i] != NULL)
      band->SetAttribute("zone",_FreqBandUsage[i]->GetId());
    else
      band->SetAttribute("zone",-1);
    FreqBandUsage->LinkEndChild(band);
  }
  zones->LinkEndChild(FreqBandUsage);
  return zones;
}

void Zone::PreXmlLoad(){
  for(size_t i=0;i<_Leds.size();i++){
    _Leds[i]._Used = false;
  }
  for(int i=0;i<32;i++){
    _FreqBandUsage[i] = NULL;
  }
}

void Zone::StaticFromXml(TiXmlElement *pElement){
  TiXmlElement *FreqBandUsage = pElement->FirstChildElement("freqbandusage");
  if(FreqBandUsage){
    int i=0;
    for(TiXmlElement *band=FreqBandUsage->FirstChildElement("band");band!=0;band = band->NextSiblingElement("band")){
      int ZoneId = -1;
      band->QueryIntAttribute("zone",&ZoneId);
      _FreqBandUsage[i] = NULL;
      if(ZoneId >= 0){
        for(size_t j=0;j<Zones.size();j++){
          if(ZoneId == Zones[j]->GetId()){
            _FreqBandUsage[i] = Zones[j];
            break;
          }
        }
      }
      i++;
    }
  }
}

void Zone::GenerateLedBits(){
  _LedBits = 0;
  for(size_t i=0;i<_LedsInUse.size();i++){
    if(_LedsInUse[i] < 0 || _LedsInUse[i] >= _Leds.size()){
      SendMyMessage(0,std::wstring(L"GenerateLedBits out of bounds"),0);
      continue;
    }
    _LedBits |= _Leds[_LedsInUse[i]]._LedBit;
  }
}

int Zone::GetColor() const {
  if(!_RandomColor)
    return _Color;
  int Red = rand() % 256;
  int Green = rand() % 256;
  int Blue = rand() % 256;
  return (Red << 16) | (Green << 8) | Blue;
}
