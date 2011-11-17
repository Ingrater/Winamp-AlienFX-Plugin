#define CONFIG_MAIN_FILE
#include "config.h"
#include  <commctrl.h>
#include "Slider.h"
#include "main.h"
#include "ListBox.h"
#include "Button.h"
#include "resource.h"
#include <string>
#include "Zone.h"
#include "XmlConfig.h"
#include "window.h"

#ifndef CLEARTYPE_QUALITY
#define CLEARTYPE_QUALITY       5
#endif

#define SLIDER1     1
#define SLIDER2     2
#define SLIDER3     3
#define SLIDER4     4
#define SLIDER5     5
#define SLIDER6     6
#define SLIDER7     7
#define SLIDER8     8
#define TEXT1       9
#define TEXT2      10
#define TEXT3      11
#define TEXT4      12
#define TEXT5      13
#define TEXT6      14
#define TEXT7      15
#define TEXT8      16

CallbackMap SliderCallbacks;
CallbackMap ButtonOnPressCallbacks;
CallbackMap ListBoxOnDoubleClickCallbacks;
Slider *EnergySlider[8] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
Slider *VarianceSlider[8] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
Slider *FactorSlider[2] = {NULL,NULL};
Button *ZoneButtons[5] = {NULL,NULL,NULL,NULL,NULL};
ListBox *ZoneSelect = NULL;
float ConfigEnergies[8];
float ConfigVariances[8];
bool ConfigVisible = false;
std::vector<Zone*> Zones;
std::wstring ZoneName;
HBRUSH RedBrush;
HBRUSH GreenBrush;
HBRUSH BackgroundBrush;

LRESULT CALLBACK ConfigProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CreateZoneProc(HWND hDlg, UINT uMsg,WPARAM wParam, LPARAM lParam );
void OnCreateZone();
void OnDeleteZone();
void OnEditZone();
HINSTANCE hInstanceConfig = NULL;

bool CreateConfigWindow(HWND phWnd, HINSTANCE phInstance){
	hInstanceConfig = phInstance;

	// Register A Window Class
	WNDCLASSEX windowClass;												// Window Class
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));						// Make Sure Memory Is Cleared
	windowClass.cbSize			= sizeof (WNDCLASSEX);					// Size Of The windowClass Structure
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraws The Window For Any Movement / Resizing
	windowClass.lpfnWndProc		= (WNDPROC)(ConfigProc);				// WindowProc Handles Messages
	windowClass.hInstance		= phInstance;				            // Set The Instance
	windowClass.hIcon			= LoadIcon(NULL, IDI_WINLOGO);          // Load default Icon
	windowClass.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);			                        // No Background required
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	windowClass.lpszClassName	= L"VisAlienfxConfig";				            // Sets The Applications Classname
	if (RegisterClassEx (&windowClass) == 0)							// Did Registering The Class Fail?
	{
		MessageBox(phWnd,L"Registering Window Class failed",L"Error",MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

  ConfigWindow = CreateWindowEx(
  WS_EX_APPWINDOW,
  L"VisAlienfxConfig",
  L"AlienFX Visualization Config",
  WS_POPUP | WS_CAPTION | WS_SYSMENU,
  CW_USEDEFAULT, CW_USEDEFAULT, 305, 660,
  phWnd, //Parent
  NULL, phInstance, NULL);

  if(ConfigWindow == NULL)
  {
    MessageBox(phWnd, L"Window Creation Failed!", L"Error!",MB_ICONEXCLAMATION | MB_OK);
    return false;
  }
  UpdateWindow(ConfigWindow);
  return true;
}

LRESULT CALLBACK ConfigProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
      case WM_CREATE:
      {
        RECT rcChild;

        GetClientRect(hWnd,&rcChild);

        /*HWND hBackground = CreateWindow(WC_STATIC,L"",WS_CHILD|WS_VISIBLE|SS_SUNKEN,
                                        0,0,rcChild.right,rcChild.bottom,
                                        hWnd,
                                        NULL,hInstanceConfig,NULL);*/

        RedBrush = CreateSolidBrush(RGB(255,0,0));
        GreenBrush = CreateSolidBrush(RGB(0,255,0));
        BackgroundBrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));

        HFONT hFont = CreateFont(11,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                      CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,L"Calibri");
        if(hFont == NULL)
          hFont = CreateFont(11,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                      CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,L"Arial");
        if(hFont == NULL)
          hFont = CreateFont(11,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                      CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,NULL);

        HFONT hFont2 = CreateFont(16,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                      CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,L"Calibri");
        if(hFont2 == NULL)
          hFont2 = CreateFont(16,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                      CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,L"Arial");
        if(hFont == NULL)
          hFont2 = CreateFont(16,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                      CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,NULL);

        HWND hGroup1 = CreateWindow(L"BUTTON",L"Sound Energy Factor",WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
                                    5,0,rcChild.right-10,145,
                                    hWnd,
                                    NULL,hInstanceConfig,NULL);
        SendMessage(hGroup1,WM_SETFONT,(WPARAM)hFont2,(LPARAM)true);

        HWND hGroup2 = CreateWindow(L"BUTTON",L"Energy Variance Factor",WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
                                    5,150,rcChild.right-10,145,
                                    hWnd,
                                    NULL,hInstanceConfig,NULL);
        SendMessage(hGroup2,WM_SETFONT,(WPARAM)hFont2,(LPARAM)true);

        HWND hGroup3 = CreateWindow(L"BUTTON",L"Zones",WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
                                    5,450,rcChild.right-10,145,
                                    hWnd,
                                    NULL,hInstanceConfig,NULL);
        SendMessage(hGroup3,WM_SETFONT,(WPARAM)hFont2,(LPARAM)true);

        ZoneSelect = new ListBox(hWnd,hInstanceConfig,10,465,rcChild.right-70,131);
        ZoneSelect->SetFont(hFont2);
        ZoneSelect->SetOnDoubleClick(OnEditZone);

        ZoneButtons[0] = new Button(hWnd,hInstanceConfig,rcChild.right-60,465,50,20,L"Add");
        ZoneButtons[0]->SetFont(hFont2);
        ZoneButtons[0]->SetOnPress(OnCreateZone);

        ZoneButtons[1] = new Button(hWnd,hInstanceConfig,rcChild.right-60,486,50,20,L"Remove");
        ZoneButtons[1]->SetFont(hFont2);
        ZoneButtons[1]->SetOnPress(OnDeleteZone);

        ZoneButtons[2] = new Button(hWnd,hInstanceConfig,rcChild.right-60,507,50,20,L"Edit");
        ZoneButtons[2]->SetFont(hFont2);
        ZoneButtons[2]->SetOnPress(OnEditZone);

        ZoneButtons[3] = new Button(hWnd,hInstanceConfig,10,610,100,20,L"Save Config");
        ZoneButtons[3]->SetFont(hFont2);
        ZoneButtons[3]->SetOnPress(XmlSave);

				ZoneButtons[4] = new Button(hWnd,hInstanceConfig,115,610,150,20,L"Show Frequency Analysis");
				ZoneButtons[4]->SetFont(hFont2);
				ZoneButtons[4]->SetOnPress(ShowOpenGLWindow);

        for(int i=0;i<8;i++){
          wchar_t Text[100];
          wsprintf(Text,L"%d - %d",i*4+1,(i+1)*4);
          EnergySlider[i] = new Slider(hWnd,hInstanceConfig,10+35*i,17,30,100,true,Text);
          EnergySlider[i]->SetFont(hFont);
          EnergySlider[i]->SetRange(0.0f,4.0f);
          EnergySlider[i]->SetValue(1.5f);

          VarianceSlider[i] = new Slider(hWnd,hInstanceConfig,10+35*i,167,30,100,true,Text);
          VarianceSlider[i]->SetFont(hFont);
          VarianceSlider[i]->SetRange(0.0f,400.0f);
          VarianceSlider[i]->SetValue(100.0f);
        }

        FactorSlider[0] = new Slider(hWnd,hInstanceConfig,10,307,70,100,true,L"Absolute Variance");
        FactorSlider[0]->SetFont(hFont);
        FactorSlider[0]->SetRange(0.0f,2.0f);
        FactorSlider[0]->SetValue(1.0f);

        FactorSlider[1] = new Slider(hWnd,hInstanceConfig,85,307,70,100,true,L"Relative Variance");
        FactorSlider[1]->SetFont(hFont);
        FactorSlider[1]->SetRange(0.0f,2.0f);
        FactorSlider[1]->SetValue(1.2f);
      }
      break;

      case WM_CLOSE:
          ShowWindow(ConfigWindow,SW_HIDE);
          ConfigVisible = false;
      break;

      case WM_PAINT:
      {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(ConfigWindow,&ps);
        for(int num=0;num<8;num++){
          RECT rc;

          //Draw energy
          rc.left = 40 + 35 * num;
          rc.right = rc.left + 3;
          rc.bottom = 104;
          rc.top = 30;
          FillRect(hdc,&rc,BackgroundBrush);

          if(ConfigEnergies[num] > 1.0f){
            rc.top = rc.bottom - (rc.bottom - rc.top) * (ConfigEnergies[num]-1.0f);
            if(ConfigEnergies[num] > EnergySlider[num]->GetValue())
              FillRect(hdc,&rc,RedBrush);
            else
              FillRect(hdc,&rc,GreenBrush);
          }

          //Draw variance
          rc.bottom = 254;
          rc.top = 180;
          FillRect(hdc,&rc,BackgroundBrush);
          if(ConfigVariances[num] > 0.0f){
            rc.top = rc.bottom - (rc.bottom - rc.top) * (ConfigVariances[num] / 200.0f);
            if(ConfigVariances[num] > VarianceSlider[num]->GetValue())
              FillRect(hdc,&rc,RedBrush);
            else
              FillRect(hdc,&rc,GreenBrush);
          }
        }
        EndPaint(ConfigWindow,&ps);
      }
      break;

      case WM_DESTROY:
          for(int i=0;i<8;i++){
            delete EnergySlider[i];
            delete VarianceSlider[i];
          }
          for(int i=0;i<5;i++){
            delete ZoneButtons[i];
          }
          delete ZoneSelect;
          return 0;

      case WM_VSCROLL:
        switch(LOWORD(wParam)){
          case TB_THUMBTRACK:
            {
              CallbackIterator it = SliderCallbacks.find((HWND)lParam);
              if(it != SliderCallbacks.end()){
                (it->second)();
              }
            }
            break;

          default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
        break;

      case WM_COMMAND:
        switch(HIWORD(wParam)){
          case BN_CLICKED:
            {
              CallbackIterator it = ButtonOnPressCallbacks.find((HWND)lParam);
              if(it != ButtonOnPressCallbacks.end()){
                (it->second)();
              }
            }
            break;
          case LBN_DBLCLK:
            {
              CallbackIterator it = ListBoxOnDoubleClickCallbacks.find((HWND)lParam);
              if(it != ListBoxOnDoubleClickCallbacks.end()){
                (it->second)();
              }
            }
            break;
          default:
            return DefWindowProc(hWnd,uMsg,wParam,lParam);
        }
        break;

      default:
          return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  return 0;
}

void DestroyConfigWindow(){
  if(ConfigWindow != NULL){
    DestroyWindow(ConfigWindow);
    ConfigWindow = NULL;
  }
  if(hInstanceConfig != NULL){
    UnregisterClass(L"VisAlienfxConfig",hInstanceConfig);
    hInstanceConfig = NULL;
  }
}

void ShowConfigWindow(){
  if(ConfigWindow != NULL){
    ShowWindow(ConfigWindow,SW_SHOW);
    ConfigVisible = true;
  }
}

void ConfigDraw(int i, float pEnergy, float pVariance){
  if(ConfigVisible){
    int num = i / 4;
    if(i % 4 == 0){
      ConfigEnergies[num] = 0.0f;
      ConfigVariances[num] = 0.0f;
    }
    ConfigEnergies[num] += pEnergy;
    ConfigVariances[num] += pVariance;
    if(i % 4 == 3){
      ConfigEnergies[num] /= 4.0f;
      ConfigVariances[num] /= 4.0f;
      if (ConfigEnergies[num] > 2.0f)
        ConfigEnergies[num] = 2.0f;
      if(ConfigVariances[num] > 200.0f)
        ConfigVariances[num] = 200.0f;
      if(num == 7){
        RECT rc;
        rc.top = 5;
        rc.bottom = 305;
        rc.left = 5;
        rc.right = 300;
        InvalidateRect(ConfigWindow,&rc,false);
      }
    }
  }
}

void OnCreateZone(){
  if(ZoneSelect->GetNumEntries() >= MAX_ZONE_NUM){
    MessageBox(ConfigWindow,L"Max zone number reached",L"Error",MB_OK | MB_ICONERROR);
    return;
  }
  if(DialogBox(hInstanceConfig,
            MAKEINTRESOURCE(IDD_DLG1),
            ConfigWindow,
            (DLGPROC)CreateZoneProc) == IDOK)
  {
    int id = ZoneSelect->GetNumEntries();
    ZoneSelect->AppendEntry(ZoneName.c_str(),id);
    Zones.push_back(new Zone(id,ZoneName.c_str(),RGB(255,0,0),false));
  }
}

void OnDeleteZone(){
  int sel = ZoneSelect->GetSelection();
  if(sel >= 0){
    if(MessageBox(ConfigWindow,L"Do you really want to delete this zone?",L"Delete?",MB_YESNO) == IDYES){
      ZoneSelect->RemoveEntry(sel);
      delete Zones[sel];
      Zones.erase(Zones.begin()+sel);
    }
  }
  else {
    MessageBox(ConfigWindow,L"You have to select a zone to delete",L"Notice",MB_OK);
  }
}

void OnEditZone(){
  int sel = ZoneSelect->GetSelection();
  if(sel >= 0){
    Zones[sel]->EditDialog(hInstanceConfig,ConfigWindow);
    //MessageBoxA(NULL,"Dialog Finished","Notice",MB_OK);
    ZoneSelect->SetText(sel,Zones[sel]->GetName().c_str());
  }
  else {
    MessageBox(ConfigWindow,L"You have to select the zone you want to edit",L"Notice",MB_OK);
  }
}

LRESULT CALLBACK CreateZoneProc( HWND hDlg, UINT uMsg,
                                WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
      case WM_INITDIALOG:
        break;
      case WM_CLOSE:
        EndDialog( hDlg, IDCANCEL );
        break;
      case WM_COMMAND:
        switch(LOWORD(wParam)){
          case IDC_BTN1: //OK
            {
              wchar_t buffer[128];
              int anz = GetDlgItemText(hDlg,IDC_EDT1,buffer,128);
              ZoneName = buffer;
              if(anz <= 0){
                MessageBox(hDlg,L"Error: The text field is empty",L"Error",MB_OK|MB_ICONERROR);
                return (FALSE);
              }
            }
            EndDialog(hDlg, IDOK);
            break;
          case IDC_BTN2: //Cancel
            EndDialog(hDlg, IDCANCEL);
            break;
        }
        break;

      default :
        return (FALSE);
    }

    return(TRUE);
}


