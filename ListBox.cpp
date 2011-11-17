#include "ListBox.h"
#include "config.h"
#include <boost/scoped_array.hpp>
#include <boost/bind.hpp>

extern CallbackMap ListBoxOnDoubleClickCallbacks;

ListBox::ListBox(HWND pFather, HINSTANCE phInstance, int pX, int pY, int pWidth, int pHeight){
  _Handle = CreateWindowEx(WS_EX_CLIENTEDGE,
                           L"LISTBOX",L"",WS_CHILD|WS_VISIBLE|LBS_NOTIFY,
                           pX,pY,pWidth,pHeight,
                           pFather,
                           NULL,phInstance,NULL);
  _DeleteHandle = true;
  ListBoxOnDoubleClickCallbacks.insert(CallbackPair(_Handle,boost::bind(&ListBox::OnDoubleClick,this)));
}

ListBox::ListBox(HWND pHandle){
  _Handle = pHandle;
  _DeleteHandle = false;
  ListBoxOnDoubleClickCallbacks.insert(CallbackPair(_Handle,boost::bind(&ListBox::OnDoubleClick,this)));
}

ListBox::~ListBox(){
  ListBoxOnDoubleClickCallbacks.erase(_Handle);
  if(_DeleteHandle)
    DestroyWindow(_Handle);
}

void ListBox::SetFont(HFONT pFont){
  SendMessage(_Handle,WM_SETFONT,(WPARAM)pFont,(LPARAM)true);
}

int ListBox::GetSelection(){
  int result = SendMessage(_Handle,LB_GETCURSEL,0,0);
  if(result == LB_ERR)
    return -1;
  return result;
}

void ListBox::AppendEntry(const wchar_t *pEntry, int pData){
  SendMessage(_Handle,LB_ADDSTRING,(WPARAM)0,(LPARAM)pEntry);
  _Data.push_back(pData);
}

void ListBox::InsertEntry(int pIndex, const wchar_t *pEntry, int pData){
  SendMessage(_Handle,LB_INSERTSTRING,(WPARAM)pIndex,(LPARAM)pEntry);
  if(pIndex == -1)
    _Data.push_back(pData);
  else
    _Data.insert(_Data.begin()+pIndex,pData);
}

bool ListBox::RemoveEntry(int pIndex){
  int res = SendMessage(_Handle,LB_DELETESTRING,(WPARAM)pIndex,(LPARAM)0);
  if(res == LB_ERR)
    return false;
  _Data.erase(_Data.begin()+pIndex);
  return true;
}

void ListBox::RemoveAllEntries(){
  while(RemoveEntry(0));
}

int ListBox::GetNumEntries(){
  int res = SendMessage(_Handle,LB_GETCOUNT,(WPARAM)0,(LPARAM)0);
  return res;
}

std::wstring ListBox::GetText(int pIndex){
  int len = SendMessage(_Handle,LB_GETTEXTLEN,(WPARAM)pIndex,(LPARAM)0);
  if(len == LB_ERR)
    return std::wstring(L"");
  boost::scoped_array<wchar_t> str(new wchar_t[len+1]);
  len = SendMessage(_Handle,LB_GETTEXT,(WPARAM)pIndex,(LPARAM)str.get());
  if(len == LB_ERR)
    return std::wstring(L"");
	std::wstring res(str.get());
  return res;
}

void ListBox::SetText(int pIndex, const wchar_t *pText){
  int sel = GetSelection();
  int data = GetData(pIndex);
  RemoveEntry(pIndex);
  int InsertIndex = (pIndex == GetNumEntries()) ? -1 : pIndex;
  InsertEntry(pIndex,pText,data);
  if(pIndex == sel)
    SendMessage(_Handle,LB_SETCURSEL,(WPARAM)sel,(LPARAM)NULL);
}

void ListBox::OnDoubleClick(){
  if(_OnDoubleClick)
    _OnDoubleClick();
}
