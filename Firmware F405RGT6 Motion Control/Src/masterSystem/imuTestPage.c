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

extern UART_HandleTypeDef huart2;

void drawImuYawLayout(int _gOff, float _degRaw){
  float _deg;
  if(_degRaw < 0) _deg = 360 + _degRaw;
  else _deg = _degRaw;
  _deg = constrain(_deg, 0, 360);
  u8g2_DrawCircle(&u8g2, 63, 26 + _gOff, 15, U8G2_DRAW_ALL);

  for(int i = 0; i < 5; i++){
    u8g2_DrawLine(&u8g2, 63, 26 + _gOff, 63 - 14 * sin((_deg + i) * PI / 180), 26 - 14 * cos((_deg + i) * PI / 180) + _gOff);
    u8g2_DrawLine(&u8g2, 63, 26 + _gOff, 63 - 14 * sin((_deg - i - 1) * PI / 180), 26 - 14 * cos((_deg - i - 1) * PI / 180) + _gOff);
  }

  u8g2_DrawDisc(&u8g2, 63, 26 + _gOff, 8, U8G2_DRAW_ALL);
  u8g2_SetDrawColor(&u8g2, 0);
  u8g2_DrawDisc(&u8g2, 63, 26 + _gOff, 6, U8G2_DRAW_ALL);
  u8g2_SetDrawColor(&u8g2, 1);

  u8g2_DrawBox(&u8g2, 62, 0 + _gOff, 3, 6);
  u8g2_DrawBox(&u8g2, 62, 23 + _gOff, 3, 7);
  u8g2_DrawTriangle(&u8g2, 60, 5 + _gOff, 67, 5 + _gOff, 63, 9 + _gOff);

  u8g2_SetFont(&u8g2, u8g2_font_profont11_tf);
  char _str[20];
  memset(_str, 0, sizeof(_str));
  sprintf(_str, "Yaw: %.1f", _degRaw);
  u8g2_DrawStr(&u8g2, 0, 52 + _gOff, _str);
  u8g2_DrawStr(&u8g2, 0, 62 + _gOff, "Long press OK to cal");
  if(_gOff) u8g2_DrawLine(&u8g2, 0, 15, _gOff, 15);
}

void IMUTestPage(int _gOff){
  drawImuYawLayout(_gOff, getHeadingDegree());
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
    	btnLongPressTime = HAL_GetTick();
    	calibrateIMU();
      /* Go to calibration*/

    }
  }
  else btnLongPressTime = HAL_GetTick();
}

void calibrateIMU(){
	uint8_t calImu[2];
	calImu[0] = 0xA5;
	calImu[1] = 0x54;
	HAL_UART_Transmit(&huart2, calImu, 2, HAL_MAX_DELAY);
	HAL_Delay(100);

	calImu[0] = 0xA5;
	calImu[1] = 0x55;
	HAL_UART_Transmit(&huart2, calImu, 2, HAL_MAX_DELAY);
	HAL_Delay(100);

	calImu[0] = 0xA5;
	calImu[1] = 0x52;
	HAL_UART_Transmit(&huart2, calImu, 2, HAL_MAX_DELAY);
}
