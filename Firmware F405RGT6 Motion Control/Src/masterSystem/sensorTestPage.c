#include "RCJLightWeightMaster.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include "../u8g2/u8g2.h"

extern u8g2_t u8g2;
extern uint8_t btnPressed;

uint8_t sensorTestCursor = 0;
uint8_t sensorTestSlide = 0;
void sensorTestPage(int _gOff){
  if(getPageUpdateFlag()){
    resetPageUpdateFlag();

    // u8g2_SetFontPosBaseline();
  }

  uint8_t btnState = getButtonState();
  if(btnState & BTN_PRESS_BACK){
    if(!btnPressed){
      btnPressed = true;
      setCurrentPage(SETTINGS_PAGE);
    }
  }
  else if(btnState & BTN_PRESS_UP){
    if(!btnPressed){
      btnPressed = true;
      if(sensorTestCursor == 1) sensorTestCursor = 0;
      else if(sensorTestSlide > 0) sensorTestSlide--;
    }
  }
  else if(btnState & BTN_PRESS_DOWN){
    if(!btnPressed){
      btnPressed = true;
      if(sensorTestCursor == 0) sensorTestCursor = 1;
      else if(sensorTestSlide < 3) sensorTestSlide++;
    }
  }
  else if(btnState & BTN_PRESS_OK){
    if(!btnPressed){
      btnPressed = true;
      if(sensorTestCursor + sensorTestSlide == 0) setCurrentPage(CAMERA_TEST_PAGE);
      else if(sensorTestCursor + sensorTestSlide == 1) setCurrentPage(IMU_SENSOR_PAGE);
      else if(sensorTestCursor + sensorTestSlide == 2) setCurrentPage(BALL_SENSOR_PAGE);
      else if(sensorTestCursor + sensorTestSlide == 3) setCurrentPage(LINE_SENSOR_PAGE);
      else setCurrentPage(BATTERY_CAL_PAGE);
    }
  }
  else btnPressed = false;

  drawArrowAnimation(_gOff, _gOff, sensorTestCursor * 16);

  int _yOff = sensorTestSlide * -16;

  drawBoxWithText(10 + _gOff, 20, 108, 14, _yOff, "Camera Test", (sensorTestCursor + sensorTestSlide) == 0);
  drawBoxWithText(10 - _gOff, 36, 108, 14, _yOff, "IMU Sensor", (sensorTestCursor + sensorTestSlide) == 1);
  drawBoxWithText(10 + _gOff, 52, 108, 14, _yOff, "Ball Sensor", (sensorTestCursor + sensorTestSlide) == 2);
  drawBoxWithText(10 - _gOff, 68, 108, 14, _yOff, "Line Sensor", (sensorTestCursor + sensorTestSlide) == 3);
  drawBoxWithText(10 + _gOff, 84, 108, 14, _yOff, "Battery Voltage", (sensorTestCursor + sensorTestSlide) == 4);

  maskTittle();
  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_SetFont(&u8g2, u8g2_font_timB12_tf);
  u8g2_DrawStr(&u8g2, 23, 12 - _gOff, "Sensor Test");
  u8g2_DrawLine(&u8g2, 0, 15, 128, 15);

  displayBattery(0);
}
