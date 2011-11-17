#include "Button.h"
#include "config.h"

#include <boost/bind.hpp>

extern CallbackMap ButtonOnPressCallbacks;

Button::Button(HWND pFather, HINSTANCE phInstance, int pX, int pY, int pWidth, int pHeight, const wchar_t *pCaption){
  _Handle = CreateWindow(L"BUTTON",pCaption,WS_CHILD|WS_VISIBLE|BS_CENTER|BS_VCENTER|BS_TEXT,
                         pX,pY,pWidth,pHeight,
                         pFather,
                         NULL,phInstance,NULL);
  ButtonOnPressCallbacks.insert(CallbackPair(_Handle,boost::bind(&Button::OnPress,this)));
}

Button::~Button(){
  DestroyWindow(_Handle);
  ButtonOnPressCallbacks.erase(_Handle);
}

void Button::SetCaption(const wchar_t *pCaption){
  SetWindowText(_Handle,pCaption);
}

void Button::SetFont(HFONT pFont){
  SendMessage(_Handle,WM_SETFONT,(WPARAM)pFont,(LPARAM)true);
}

void Button::OnPress(){
  if(_OnPress)
    _OnPress();
}
