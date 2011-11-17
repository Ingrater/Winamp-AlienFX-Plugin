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
#define TIMER_MAIN_FILE
#include "timer.h"

#include "assert.h"

sys::Timer::Timer()								// Initialize Our Timer (Get It Ready)
{
  #ifdef WIN32
  if (!QueryPerformanceFrequency((LARGE_INTEGER *) &_Frequency))
  {
    // No Performace Counter Available
    //ThException ex(TH_EXCEPTION("Kein ausreichend genauer Timer vorhanden","sys::Timer"));
    //throw ex;
  }
  else
  {
	// Performance Counter Is Available
	QueryPerformanceCounter((LARGE_INTEGER *) &_Start);
	_Resolution = ((double)1.0f)/((double)_Frequency);
  }
  #else
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&_timestruct);
  _Start = static_cast<uint64>(_timestruct.tv_nsec)
           static_cast<uint64>(_timestruct.tv_sec * 1000000000L);
  #endif
}

float sys::Timer::GetTimeFloat() const
{
  uint64 ui64Time;
  #ifdef WIN32
  QueryPerformanceCounter((LARGE_INTEGER *) &ui64Time);
  return ( (float) ( ui64Time - _Start) * (float)_Resolution)*1000.0f;
  #else
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&_timestruct);
  ui64Time = (uint64)_timestruct.tv_nsec + (uint64)_timestruct.tv_sec * 1000000000L;
  return (float)((ui64Time - _Start) / 1000000L);
  #endif
}

double sys::Timer::GetTimeDouble() const
{
  uint64 ui64Time;
  #ifdef WIN32
  QueryPerformanceCounter((LARGE_INTEGER *) &ui64Time);
  return ( (double) ( ui64Time - _Start) * _Resolution) * 1000;
  #else
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&_timestruct);
  ui64Time = (uint64)_timestruct.tv_nsec + (uint64)_timestruct.tv_sec * 1000000000L;
  return (double)((ui64Time - _Start) / 1000000L);
  #endif
}

uint64 sys::Timer::GetTimeInt64() const{
  uint64 ui64Time;
  #ifdef WIN32
  QueryPerformanceCounter((LARGE_INTEGER *) &ui64Time);
  #else
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&_timestruct);
  ui64Time = (uint64)_timestruct.tv_nsec + (uint64)_timestruct.tv_sec * 1000000000L;
  #endif
  return ui64Time - _Start;
}

sys::Zeitpunkt::Zeitpunkt(Timer& pTimer){
  _TimerReference = &pTimer;
  _Time = pTimer.GetTimeInt64();
}

sys::Zeitpunkt::Zeitpunkt(Timer* pTimer){
  assert(pTimer != NULL);
  _TimerReference = pTimer;
  _Time = pTimer->GetTimeInt64();
}

sys::Zeitpunkt::Zeitpunkt(const Zeitpunkt& pSource){
  _TimerReference = pSource._TimerReference;
  _Time = pSource._Time;
}

sys::Zeitpunkt& sys::Zeitpunkt::operator = (const uint64& src){
  _Time = src;
  return *this;
}

sys::Zeitpunkt& sys::Zeitpunkt::operator = (const Zeitpunkt& src){
  _Time = src._Time;
  _TimerReference = src._TimerReference;
  return *this;
}

double sys::Zeitpunkt::operator - (const Zeitpunkt& op2) const{
  #ifdef WIN32
  double dResolution=_TimerReference->GetResolution();
  return static_cast<double>(_Time - op2._Time) * dResolution * 1000.0;
  #else
  return static_cast<double>(_Time - op2._Time) / 1000000.0;
  #endif
}

bool sys::Zeitpunkt::operator > (const Zeitpunkt& op2) const
{
  return (_Time > op2._Time);
}

bool sys::Zeitpunkt::operator < (const Zeitpunkt& op2) const
{
  return (_Time < op2._Time);
}

bool sys::Zeitpunkt::operator <= (const Zeitpunkt& op2) const
{
  return (_Time <= op2._Time);
}

bool sys::Zeitpunkt::operator >= (const Zeitpunkt& op2) const
{
  return (_Time >= op2._Time);
}

bool sys::Zeitpunkt::operator == (const Zeitpunkt& op2) const
{
  return (_Time == op2._Time);
}

bool sys::Zeitpunkt::operator != (const Zeitpunkt& op2) const
{
  return (_Time != op2._Time);
}
