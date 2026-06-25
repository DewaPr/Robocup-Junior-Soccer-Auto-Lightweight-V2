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

extern I2C_HandleTypeDef hi2c1;

uint8_t calFlag = 0;

void drawLineSensorLayout(int _gOff, uint8_t _frontActive, uint8_t _leftActive, uint8_t _backActive, uint8_t _rightActive){
  uint8_t dotMiddleSpace = 7;
  uint8_t dotNeighSpace = 7;
  int dotyOffset = 8;

  u8g2_SetDrawColor(&u8g2, 1);
  for(int i = 0; i < 5; i++){
    if(_leftActive & (1 << (4 - i))) u8g2_DrawBox(&u8g2, 64 - (i * dotNeighSpace) - dotMiddleSpace, 32 - dotyOffset + _gOff, 1, 1);
    else u8g2_DrawFrame(&u8g2, 64 - 1 - (i * dotNeighSpace) - dotMiddleSpace, 32 - 1 - dotyOffset + _gOff, 3, 3);

    if(_backActive & (1 << (4 - i))) u8g2_DrawBox(&u8g2, 64, 32 + (i * dotNeighSpace) + dotMiddleSpace - dotyOffset + _gOff, 1, 1);
    else u8g2_DrawFrame(&u8g2, 64 - 1, 32 - 1 + (i * dotNeighSpace) + dotMiddleSpace - dotyOffset + _gOff, 3, 3);

    if(_rightActive & (1 << (4 - i))) u8g2_DrawBox(&u8g2, 64 + (i * dotNeighSpace) + dotMiddleSpace, 32 - dotyOffset + _gOff, 1, 1);
    else u8g2_DrawFrame(&u8g2, 64 - 1 + (i * dotNeighSpace) + dotMiddleSpace, 32 - 1 - dotyOffset + _gOff, 3, 3);

    if(i < 3){
      if(i == 0){
          if(_frontActive & (1 << (4 - i))) u8g2_DrawBox(&u8g2, 64, 32 - (i * dotNeighSpace) - dotMiddleSpace - dotyOffset + _gOff, 1, 1);
          else u8g2_DrawFrame(&u8g2, 64 - 1, 32 - 1 - (i * dotNeighSpace) - dotMiddleSpace - dotyOffset + _gOff, 3, 3);
      }
      else{
    	  if(_frontActive & (1 << (2 - (i - 1)))) u8g2_DrawBox(&u8g2, 64, 32 - (i * dotNeighSpace) - dotMiddleSpace - dotyOffset + _gOff, 1, 1);
    	  else u8g2_DrawFrame(&u8g2, 64 - 1, 32 - 1 - (i * dotNeighSpace) - dotMiddleSpace - dotyOffset + _gOff, 3, 3);
      }
    }
  }
  for(int i = 0; i < 2; i++){
    if(_frontActive & (1 << 0)) u8g2_DrawBox(&u8g2, 64 - (i * dotNeighSpace) - dotNeighSpace, 32 - (2 * dotNeighSpace) - dotMiddleSpace - dotyOffset + _gOff, 1, 1);
    else u8g2_DrawFrame(&u8g2, 64 - 1 - (i * dotNeighSpace) - dotNeighSpace, 32 - 1 - (2 * dotNeighSpace) - dotMiddleSpace - dotyOffset + _gOff, 3, 3);

    if(_frontActive & (1 << 3)) u8g2_DrawBox(&u8g2, 64 + (i * dotNeighSpace) + dotNeighSpace, 32 - (2 * dotNeighSpace) - dotMiddleSpace - dotyOffset + _gOff, 1, 1);
    else u8g2_DrawFrame(&u8g2, 64 - 1 + (i * dotNeighSpace) + dotNeighSpace, 32 - 1 - (2 * dotNeighSpace) - dotMiddleSpace - dotyOffset + _gOff, 3, 3);
  }

  for(int i = 0; i < 5; i++){
    if(_leftActive & (1 << (4 - i))) u8g2_DrawFrame(&u8g2, 64 - 2 - (i * dotNeighSpace) - dotMiddleSpace, 32 - 2 - dotyOffset + _gOff, 5, 5);
    if(_backActive & (1 << (4 - i))) u8g2_DrawFrame(&u8g2, 64 - 2, 32 - 2 + (i * dotNeighSpace) + dotMiddleSpace - dotyOffset + _gOff, 5, 5);
    if(_rightActive & (1 << (4 - i))) u8g2_DrawFrame(&u8g2, 64 - 2 + (i * dotNeighSpace) + dotMiddleSpace, 32 - 2 - dotyOffset + _gOff, 5, 5);
    if(i < 3){
      if(i == 0){
		  if(_frontActive & (1 << (4 - i))) u8g2_DrawFrame(&u8g2, 64 - 2, 32 - 2 - (i * dotNeighSpace) - dotMiddleSpace - dotyOffset + _gOff, 5, 5);
	  }
	  else{
		  if(_frontActive & (1 << (2 - (i - 1)))) u8g2_DrawFrame(&u8g2, 64 - 2, 32 - 2 - (i * dotNeighSpace) - dotMiddleSpace - dotyOffset + _gOff, 5, 5);
	  }
    }
  }
  for(int i = 0; i < 2; i++){
    if(_frontActive & (1 << 0)) u8g2_DrawFrame(&u8g2, 64 - 2 - (i * dotNeighSpace) - dotNeighSpace, 32 - 2 - (2 * dotNeighSpace) - dotMiddleSpace - dotyOffset + _gOff, 5, 5);
    if(_frontActive & (1 << 3)) u8g2_DrawFrame(&u8g2, 64 - 2 + (i * dotNeighSpace) + dotNeighSpace, 32 - 2 - (2 * dotNeighSpace) - dotMiddleSpace - dotyOffset + _gOff, 5, 5);
  }
  u8g2_SetFont(&u8g2, u8g2_font_profont11_tf);
  u8g2_DrawStr(&u8g2, 0, 52 + _gOff, "Long OK");
  if(!calFlag) u8g2_DrawStr(&u8g2, 0, 62 + _gOff, "to cal");
  else u8g2_DrawStr(&u8g2, 0, 62 + _gOff, "to finish");
  if(_gOff) u8g2_DrawLine(&u8g2, 0, 15, _gOff, 15);
}

void lineSensorTestPage(int _gOff){
  uint8_t dataFrontBits = getLineSensorBits(0);
  uint8_t dataLeftBits = getLineSensorBits(1);
  uint8_t dataBackBits = getLineSensorBits(2);
  uint8_t dataRightBits = getLineSensorBits(3);

  drawLineSensorLayout(_gOff, dataFrontBits, dataLeftBits, dataBackBits, dataRightBits);
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
    	btnLongPressTime = HAL_GetTick();
    	uint8_t _pData[1];
    	if(!calFlag){
    		_pData[0] = 0xCA;
    		calFlag = 1;
    	}
    	else{
    		_pData[0] = 0x9A;
    		calFlag = 0;
    	}
    	HAL_I2C_Master_Transmit(&hi2c1, 0x45 << 1, _pData, 1, HAL_MAX_DELAY);
    }
  }
  else btnLongPressTime = HAL_GetTick();
}
