#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <string>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

#define MAX_ZONE_NUM 8

// main structure with plugin information, version, name...
struct winampVisModule{
  const char *description;             // name/title of the plugin
  HWND hwndParent;               // hwnd of the Winamp client main window (stored by Winamp when dll is loaded)
  HINSTANCE hDllInstance;        // hinstance of this plugin DLL. (stored by Winamp when dll is loaded)
  size_t sRate;                  // sample rate (filled by winamp)
  size_t nCh;                    // number of channels (filled by winamp)
  size_t latencyMs;              // latency from call to Renderer to actual drawing
  size_t delayMs;                // delay between calls to renderer (ms)

  //the data is filled in before each render call
  size_t spectrumNCh;            // number of channels for spectrum data
  size_t waveformNCh;            // number of chnanles for waveform data
  byte spectrumData[2][576];     // Spectrum data
  char waveformData[2][576];     // Waveform data

  //Functions called by winamp
  void (*config)(struct winampVisModule*);              // function which will be executed on config event
  int (*init)(struct winampVisModule*);                 // function which will be executed on init event
  int (*render)(struct winampVisModule*);               // function which will be executed on render event
  void (*quit)(struct winampVisModule*);                // function which will be executed on quit event
  void* userData;                // userdata optional
};

struct winampVisHeader {
  int version;
  const char *description;
  winampVisModule* (*GetModule)(int which);
};

struct ZoneInfo {
  int _Color;
  int _Leds;
};

void config(winampVisModule* pVisModule);
int  init(winampVisModule* pVisModule);
int  render(winampVisModule* pVisModule);
void quit(winampVisModule* pVisModule);
winampVisModule* GetModule(int which);
void SendMyMessage(int pId, const std::wstring& pString, int pRGB);

#ifdef __cplusplus
extern "C"
{
#endif

DLL_EXPORT winampVisHeader* winampVisGetHeader();

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
