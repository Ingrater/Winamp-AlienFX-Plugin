#ifndef _BUTTON_H_
#define _BUTTON_H_
#include <windows.h>
#include <boost/function.hpp>

class Button {
  public:
    typedef boost::function<void()> CallbackFunc;

  private:
    HWND _Handle;
    CallbackFunc _OnPress;

    void OnPress();

  public:
    Button(HWND pFather, HINSTANCE phInstance,int pX, int pY, int pWidth, int pHeight, const wchar_t *pCaption);
    ~Button();
    inline void SetOnPress(CallbackFunc pFunc) {_OnPress = pFunc;}
    void SetCaption(const wchar_t *pCaption);
    void SetFont(HFONT pFont);
};

#endif // _BUTTON_H_
