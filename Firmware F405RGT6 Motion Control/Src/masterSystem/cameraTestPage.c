#include "RCJLightWeightMaster.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include "string.h"
#include "../u8g2/u8g2.h"

extern u8g2_t u8g2;
extern uint8_t btnPressed;
extern unsigned long btnLongPressTime;

void drawCameraLayout(int _gOff, int _px, uint8_t _det){
  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_DrawFrame(&u8g2, 10, 25 + _gOff, 108, 20);
  u8g2_SetDrawColor(&u8g2, 0);
  u8g2_DrawRFrame(&u8g2,10, 25 + _gOff, 108, 20, 3);
  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_DrawRFrame(&u8g2,10 + 1, 25 + 1 + _gOff, 108 - 2, 20 - 2, 3);

  if(_det == 1){
	  int markingPos = map(_px, CAM_X_MIN_PX, CAM_X_MAX_PX, -50, 50);
	  u8g2_DrawBox(&u8g2, 62 - markingPos, 26 + _gOff, 3, 18);
  }

  u8g2_DrawBox(&u8g2, 63, 19 + _gOff, 1, 4);
  u8g2_DrawBox(&u8g2, 63 + 27, 19 + _gOff, 1, 4);
  u8g2_DrawBox(&u8g2, 63 - 27, 19 + _gOff, 1, 4);

  u8g2_DrawBox(&u8g2, 63, 47 + _gOff, 1, 4);
  u8g2_DrawBox(&u8g2, 63 + 27, 47 + _gOff, 1, 4);
  u8g2_DrawBox(&u8g2, 63 - 27, 47 + _gOff, 1, 4);

  u8g2_SetFont(&u8g2, u8g2_font_profont11_tf);
  char _str[40];

  memset(_str, 0, sizeof(_str));

  if(_det == 1) sprintf(_str, "Camera pixel: %d", _px);
  else if(_det == 2) sprintf(_str, "Camera pixel: Error");
  else sprintf(_str, "Camera pixel: Nothing");
  u8g2_DrawStr(&u8g2, 0, 62 + _gOff, _str);

  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_SetFont(&u8g2, u8g2_font_timB12_tf);
  u8g2_DrawStr(&u8g2, 18, 12 - _gOff, "Camera Test");
  u8g2_DrawLine(&u8g2, 0, 15, 128, 15);
}

int currCamX = 0;
void cameraTestPage(int _gOff){
  if(getCamDetect()){
	  currCamX = getCamX();
  }
  drawCameraLayout(_gOff, currCamX, getCamDataValid()? getCamDetect() : 2);
  displayBattery(20);

  uint8_t btnState = getButtonState();
  if(btnState & BTN_PRESS_BACK){
    if(!btnPressed){
      btnPressed = true;
      setCurrentPage(SENSOR_TEST_PAGE);
    }
  }
  else btnPressed = false;

  if(btnState & BTN_PRESS_OK){
    if(HAL_GetTick() - btnLongPressTime > 3000){
      /* Go to calibration*/
    }
  }
  else btnLongPressTime = HAL_GetTick();
}
