#ifndef _SLIDER_H_
#define _SLIDER_H_
#include <windows.h>
#include <boost/function.hpp>

class Slider {
  public:
    typedef boost::function<void()> CallbackFunc;

  private:
    int _PosX,_PosY,_Size;
    HWND _Slider,_Caption,_ValueCaption;
    float _Value,_Min,_Max;
    bool _Vertical;
    CallbackFunc _Callback;

    void UpdateValue();

  public:
    Slider(HWND pFather, HINSTANCE pInstance, int pX, int pY, int pWidth, int pHeight, bool pVertical, const wchar_t* pCaption);
    ~Slider();
    void SetRange(float pMin, float pMax);
    inline float GetValue() const {return _Value;}
    void SetValue(float pValue);
    inline void SetCallback(CallbackFunc pCallback){
      _Callback = pCallback;
    }
    void SetFont(HFONT pFont);
    void Callback();
};

#endif // _SLIDER_H_

