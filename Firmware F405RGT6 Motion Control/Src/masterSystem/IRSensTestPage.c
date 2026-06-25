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

unsigned long IRLayoutOffUpdateTime = 0;
int IRDisplayBitsState[18];
int IRDisplayBitsChange[18];
void drawIRSensorLayout(int _gOff, uint16_t _valActive, uint8_t _valAuxActive){
  for(int i = 0; i < 3; i++){
    if(i == 1) u8g2_DrawBox(&u8g2, 32 + (i * 32), 20 + _gOff, 1, 5);
    u8g2_DrawBox(&u8g2, 32 + (i * 32), 43 + _gOff, 1, 5);
  }

  for(int i = 0; i < 18; i++){
    if(i < 16){
      if(_valActive & (1 << i)) IRDisplayBitsChange[i] = 6;
      else IRDisplayBitsChange[i] = 0;
    }
    else{
      if(_valAuxActive & (1 << (i - 16))) IRDisplayBitsChange[i] = 6;
      else IRDisplayBitsChange[i] = 0;
    }
  }

  if(HAL_GetTick() - IRLayoutOffUpdateTime > 10){
    IRLayoutOffUpdateTime = HAL_GetTick();

    for(int i = 0; i < 18; i++){
      if(IRDisplayBitsState[i] > IRDisplayBitsChange[i]) IRDisplayBitsState[i] -= ceil(abs(IRDisplayBitsState[i] - IRDisplayBitsChange[i]) * ANIMATION_SLIDE_SPEED);
      else if(IRDisplayBitsState[i] < IRDisplayBitsChange[i]) IRDisplayBitsState[i] += ceil(abs(IRDisplayBitsState[i] - IRDisplayBitsChange[i]) * ANIMATION_SLIDE_SPEED);
      if(i < 16){
        if(i == 0){
          u8g2_DrawBox(&u8g2, 1, 33 + _gOff, 2, IRDisplayBitsState[i] + 1);
          u8g2_DrawBox(&u8g2, 1, 33 - IRDisplayBitsState[i] + _gOff, 2, IRDisplayBitsState[i] + 1);
          u8g2_DrawBox(&u8g2, 126, 33 + _gOff, 2, IRDisplayBitsState[i] + 1);
          u8g2_DrawBox(&u8g2, 126, 33 - IRDisplayBitsState[i] + _gOff, 2, IRDisplayBitsState[i] + 1);
        }
        else{
          u8g2_DrawBox(&u8g2, 126 - (i * 8), 33 + _gOff, 5, IRDisplayBitsState[i] + 1);
          u8g2_DrawBox(&u8g2, 126 - (i * 8), 33 - IRDisplayBitsState[i] + _gOff, 5, IRDisplayBitsState[i] + 1);
        }
      }
      else{
        u8g2_DrawBox(&u8g2, 32 + _gOff + ((i - 16) * 64), 20, IRDisplayBitsState[i] + 1, 5);
        u8g2_DrawBox(&u8g2, 32 - IRDisplayBitsState[i] + _gOff + ((i - 16) * 64), 20, IRDisplayBitsState[i] + 1, 5);
      }
    }
  }
  u8g2_SetFont(&u8g2, u8g2_font_profont11_tf);
  u8g2_DrawStr(&u8g2, 0, 62 + _gOff, "Long press OK to cal");

  u8g2_SetFont(&u8g2, u8g2_font_timB12_tf);
  u8g2_DrawStr(&u8g2, 14, 12 - _gOff, "IR Sensor Test");
  u8g2_DrawLine(&u8g2, 0, 15, 128, 15);
}

void IRSensorTestPage(int _gOff){
  drawIRSensorLayout(_gOff, getIRData(), getIRAuxData());
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
