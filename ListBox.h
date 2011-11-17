#ifndef _LIST_BOX_H_
#define _LIST_BOX_H_
#include <vector>
#include <windows.h>
#include "callback.h"

class ListBox {
  private:
    std::vector<int> _Data;
    bool _DeleteHandle;
    HWND _Handle;
    CallbackFunc _OnDoubleClick;

    void OnDoubleClick();
  public:
    ListBox(HWND pFather, HINSTANCE phInstance, int pX, int pY, int pWidth, int pHeight);
    ListBox(HWND pHandle);
    ~ListBox();
    int GetSelection();
    inline int GetData(int pIndex) const {return _Data[pIndex];}
    std::wstring GetText(int pIndex);
    void SetText(int pIndex, const wchar_t *pText);
    void AppendEntry(const wchar_t *pEntry, int pData);
    void InsertEntry(int pIndex, const wchar_t *pEntry, int pData);
    bool RemoveEntry(int pIndex);
    void RemoveAllEntries();
    void SetFont(HFONT pFont);
    int GetNumEntries();
    inline void SetOnDoubleClick(CallbackFunc pFunc){_OnDoubleClick = pFunc;}
};

#endif // _LIST_BOX_H_

