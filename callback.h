#ifndef _CALLBACK_H_
#define _CALLBACK_H_
#include <boost/function.hpp>
#include <windows.h>
#include <map>

typedef boost::function<void()> CallbackFunc;
typedef std::map<HWND,CallbackFunc> CallbackMap;
typedef std::pair<HWND,CallbackFunc> CallbackPair;
typedef CallbackMap::iterator CallbackIterator;

#endif // _CALLBACK_H_
