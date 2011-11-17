#include "Slider.h"
#include "config.h"
#include "main.h"
#include <boost/bind.hpp>
#include  <commctrl.h>

extern CallbackMap SliderCallbacks;

Slider::Slider(HWND pFather, HINSTANCE pInstance, int pX, int pY, int pWidth, int pHeight, bool pVertical, const wchar_t* pCaption){
  _PosX = pX;
  _PosY = pY;
  if(pVertical)
    _Size = pHeight;
  else
    _Size = pWidth;
  _Vertical = pVertical;

  // Create a Slider
  DWORD dwStyle = WS_CHILD | WS_VISIBLE;
  if(pVertical)
    dwStyle |= TBS_VERT;
  _Slider = CreateWindow(TRACKBAR_CLASS, L"", dwStyle,
                         pX,pY,pWidth,pHeight, //pos-x, pos-y, width, height
                         pFather, //father
                         NULL, pInstance, NULL);
  UpdateWindow(_Slider);
  SendMessage(_Slider,TBM_SETRANGE,(WPARAM)true,(LPARAM)MAKELONG(0,_Size));

  _Caption = CreateWindow(WC_STATIC,pCaption,WS_CHILD|WS_VISIBLE|SS_CENTER,
                          pX, pY+pHeight-2, pWidth, 12,
                          pFather,
                          NULL, pInstance, NULL);


  _ValueCaption = CreateWindow(WC_STATIC,L"",WS_CHILD|WS_VISIBLE|SS_CENTER,
                       pX, pY+pHeight+10, pWidth, 12,
                       pFather,
                       NULL,pInstance,NULL);

  SliderCallbacks.insert(CallbackPair(_Slider,boost::bind(&Slider::Callback,this)));
  SetValue(_Min);
}

Slider::~Slider(){
  SliderCallbacks.erase(_Slider);
  DestroyWindow(_Slider);
  DestroyWindow(_Caption);
  DestroyWindow(_ValueCaption);
}

void Slider::SetFont(HFONT pFont){
  SendMessage(_Caption,WM_SETFONT,(WPARAM)pFont,(LPARAM)true);
  UpdateWindow(_Caption);
  SendMessage(_ValueCaption,WM_SETFONT,(WPARAM)pFont,(LPARAM)true);
  UpdateWindow(_ValueCaption);
}

void Slider::UpdateValue(){
  DWORD pos = SendMessage(_Slider, TBM_GETPOS, 0, 0);
  _Value = (1.0f - (float)pos / (float)_Size) * (_Max - _Min) + _Min;
  wchar_t Caption[100];
  swprintf(Caption,L"%.2f",_Value);
  SetWindowText(_ValueCaption,Caption);
}

void Slider::Callback(){
  UpdateValue();
  if(_Callback)
    _Callback();
}

void Slider::SetValue(float pValue){
  _Value = pValue;
  if(_Value < _Min) _Value = _Min;
  if(_Value > _Max) _Value = _Max;
  SendMessage(_Slider,TBM_SETPOS,(WPARAM)true,(LPARAM)((1.0f - (_Value-_Min) / (_Max-_Min)) * _Size));
  UpdateValue();
}

void Slider::SetRange(float pMin, float pMax){
  _Min = pMin;
  _Max = pMax;
  UpdateValue();
}
