#ifndef _ZONE_H_
#define _ZONE_H_
#include <windows.h>
#include <string>
#include <vector>
#include "ListBox.h"
#include "tinyxml.h"

enum SupportedModels {
  FIRST_MODEL = 0,
  AREA_51_M15X = FIRST_MODEL,
  ALL_POWERFULL_M17X = 1,
  ALL_POWERFULL_M15X = 2,
  ALL_POWERFULL_M11X = 3,
  LAST_MODEL = ALL_POWERFULL_M11X,
	CUSTOM_MODEL = 128
};

class Zone {
  private:
    struct Led {
      std::wstring _Name;
      int _LedBit;
      bool _Used;
      Led() : _LedBit(0), _Used(false) {}
    };

    struct DialogData {
      class Zone *self;
      ListBox *LedBoxLeft;
      ListBox *LedBoxRight;
      ListBox *FreqBoxLeft;
      ListBox *FreqBoxRight;
      ~DialogData(){
        delete LedBoxLeft;
        delete LedBoxRight;
        delete FreqBoxLeft;
        delete FreqBoxRight;
      }
    };

    int _Id;
    int _Color; //Color layout is AARRGGBB (winapi...)
    bool _RandomColor;
    int _LedBits;
    bool _Used;
    std::vector<int> _LedsInUse;
    std::wstring _Name;

    static SupportedModels _Model;
    static std::vector<Led> _Leds;
    static class Zone *_FreqBandUsage[32];
    static COLORREF _CustomColors[16];

    static LRESULT CALLBACK DialogProc(HWND hDlg, UINT uMsg,WPARAM wParam, LPARAM lParam);
    void GenerateLedBits();

  public:
    Zone(int pId, const wchar_t *pName, int pColor, bool pRandomColor);
    Zone(){};
    ~Zone();

    void AddFreqBand(int pIndex);
    void RemoveFreqBand(int pIndex);
    void AddLed(int pIndex);
    void RemoveLed(int pIndex);

    TiXmlElement* ToXml();
    void FromXml(TiXmlElement* pElement);

    void EditDialog(HINSTANCE hInstance, HWND hWnd);

    inline void SetColor(int pColor) {_Color = pColor;}
    int GetColor() const;
    inline const std::vector<int>& GetLedsInUse() const {return _LedsInUse;}
    inline void SetName(const wchar_t* pName){_Name = pName;}
    inline std::wstring GetName() const {return _Name;}
    inline int GetId() const {return _Id;}
    inline void SetUsed(bool pValue){_Used = pValue;}
    inline bool GetUsed() const {return _Used;}
    inline int GetLedBits() const {return _LedBits;}

    static void SetModel(SupportedModels pModel);
    static SupportedModels GetModel(){return _Model;}
    static TiXmlElement* StaticToXml();
    static void StaticFromXml(TiXmlElement *pElement);
    static void PreXmlLoad();
    static class Zone** GetFreqBandUsage(){return _FreqBandUsage;}

		static void PrepareCustomModel();
		static void AddCustomLed(std::wstring name, int ledBit);
};
#endif // _ZONE_H_

