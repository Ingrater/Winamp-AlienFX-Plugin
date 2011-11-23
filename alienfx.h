#ifndef _ALIENFX_H_
#define _ALIENFX_H_
#include <windows.h>

#define ALIENFX_USER_CONTROLS 0x01
#define ALIENFX_SLEEP_LIGHTS 0x02
#define ALIENFX_ALL_OFF 0x03
#define ALIENFX_ALL_ON 0x04

#define ALIENFX_MORPH 0x01
#define ALIENFX_BLINK 0x02
#define ALIENFX_STAY 0x03
#define ALIENFX_BATTERY_STATE 0x0F

#define ALIENFX_A51_M15X_TOUCHPAD        0x000001
#define ALIENFX_A51_M15X_LIGHTPIPE       0x000020
#define ALIENFX_A51_M15X_ALIENWARE_LOGO  0x000080
#define ALIENFX_A51_M15X_ALIENHEAD       0x000100
#define ALIENFX_A51_M15X_KEYBOARD        0x000400
#define ALIENFX_A51_M15X_POWER_BUTTON    0x008000
#define ALIENFX_A51_M15X_TOUCH_PANEL     0x010000

#define ALIENFX_AP_M17X_KEYBOARD_RIGHT          0x0000001
#define ALIENFX_AP_M17X_KEYBOARD_MIDDLE_RIGHT   0x0000002
#define ALIENFX_AP_M17X_KEYBOARD_MIDDLE_LEFT    0x0000004
#define ALIENFX_AP_M17X_KEYBOARD_LEFT           0x0000008
#define ALIENFX_AP_M17X_RIGHT_SPEAKER           0x0000020
#define ALIENFX_AP_M17X_LEFT_SPEAKER            0x0000040
#define ALIENFX_AP_M17X_ALIENHEAD               0x0000080
#define ALIENFX_AP_M17X_ALIENWARE_LOGO          0x0000100
#define ALIENFX_AP_M17X_TOUCHPAD                0x0000200
#define ALIENFX_AP_M17X_TOUCHPANEL              0x0000400
#define ALIENFX_AP_M17X_POWER_BUTTON            0x0002000
#define ALIENFX_AP_M17X_POWER_BUTTON_EYES       0x0004000

#define ALIENFX_AP_M15X_KEYBOARD_LEFT           0x0000001
#define ALIENFX_AP_M15X_KEYBOARD_MIDDLE_LEFT    0x0000002
#define ALIENFX_AP_M15X_KEYBOARD_MIDDLE_RIGHT   0x0000004
#define ALIENFX_AP_M15X_KEYBOARD_RIGHT          0x0000008
#define ALIENFX_AP_M15X_LEFT_SPEAKER            0x0000020
#define ALIENFX_AP_M15X_RIGHT_SPEAKER           0x0000040
#define ALIENFX_AP_M15X_ALIENHEAD               0x0000080
#define ALIENFX_AP_M15X_ALIENWARE_LOGO          0x0000100
#define ALIENFX_AP_M15X_TOUCHPAD                0x0000200
#define ALIENFX_AP_M15X_TOUCHPANEL              0x0000400
#define ALIENFX_AP_M15X_POWER_BUTTON            0x0002000
#define ALIENFX_AP_M15X_POWER_BUTTON_EYES       0x0004000

#define ALIENFX_AP_M11X_KEYBOARD                0x0000001
#define ALIENFX_AP_M11X_RIGHT_SPEAKER           0x0000020
#define ALIENFX_AP_M11X_LEFT_SPEAKER            0x0000040
#define ALIENFX_AP_M11X_ALIENWARE_LOGO          0x0000100
#define ALIENFX_AP_M11X_TOUCHPANEL              0x0000800
#define ALIENFX_AP_M11X_POWER_BUTTON            0x0002000
#define ALIENFX_AP_M11X_POWER_BUTTON_EYES       0x0004000

#define ALIENFX_DEVICE_RESET 0x06
#define ALIENFX_READY 0x10
#define ALIENFX_BUSY 0x11
#define ALIENFX_UNKOWN_COMMAND 0x12

bool AlienfxInit();
bool AlienfxInitDevice(int deviceId, int protocolVersion);
void AlienfxDeinit();

void AlienfxReset(byte pOptions);
void AlienfxSetColor(byte pAction, byte pSetId, int pLeds, int pColor);
void AlienfxEndLoopBlock();
void AlienfxEndTransmitionAndExecute();
void AlienfxStoreNextInstruction(byte pStorageId);
void AlienfxEndStorageBlock();
void AlienfxSaveStorageData();
void AlienfxSetSpeed(int pSpeed);
byte AlienfxGetDeviceStatus();
byte AlienfxWaitForReady();
byte AlienfxWaitForBusy();
bool AlienfxReinit();


#endif // _ALIENFX_H_

