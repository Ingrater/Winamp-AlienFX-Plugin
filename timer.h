/*************************************************************
Copyright (c) 2008

Benjamin Thaut. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this list of conditions
     and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
     and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY BENJAMIN THAUT "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

web: www.benjamin-thaut.de
*************************************************************/
#ifndef _TIMER_H_
#define _TIMER_H_

#ifdef WIN32
#include <windows.h>
typedef unsigned __int64 uint64;
#else
#include <time.h>
typedef unsigned long long uint64;
#endif

namespace sys {

  class Timer {
    private:
      double _Resolution;
      uint64 _Frequency;
      uint64 _Start;
      #ifdef __linux
      struct timespec _timestruct;
      #endif

    public:
      Timer();
      float GetTimeFloat() const;
      double GetTimeDouble() const;
      uint64 GetTimeInt64() const;
      inline double GetResolution() const {return _Resolution;}
  };

  class Zeitpunkt {
    private:
      Timer* _TimerReference;
      uint64 _Time;
    public:
      Zeitpunkt(Timer& pTimer);
      Zeitpunkt(Timer* pTimer);
      Zeitpunkt(const Zeitpunkt& pSource);
      Zeitpunkt& operator = (const uint64& src);
      Zeitpunkt& operator = (const Zeitpunkt& src);
      double operator - (const Zeitpunkt& op2) const;
      bool operator > (const Zeitpunkt& op2) const;
      bool operator < (const Zeitpunkt& op2) const;
      bool operator <= (const Zeitpunkt& op2) const;
      bool operator >= (const Zeitpunkt& op2) const;
      bool operator == (const Zeitpunkt& op2) const;
      bool operator != (const Zeitpunkt& op2) const;
      inline void SetTimerReference(Timer *pTimer) {_TimerReference = pTimer;}
  };

#ifndef TIMER_MAIN_FILE
extern Timer* MainTimer;
#else
Timer* MainTimer = NULL;
#endif

};

#endif
