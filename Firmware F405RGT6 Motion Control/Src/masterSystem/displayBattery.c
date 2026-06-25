#include "RCJLightWeightMaster.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include "string.h"
#include "../u8g2/u8g2.h"

extern u8g2_t u8g2;
extern eepObject eepVal;

extern uint8_t btnPressed;
extern uint8_t eepPutFlag;

extern unsigned long btnLongPressTime;
extern unsigned long btnDelayPressTime;

unsigned long batteryBlinkTime = 0;
uint8_t batteryBlinkState = 0;

unsigned long batteryOffsetTime = 0;
int battAnimateOffset = 20;

void drawBatteryLayout(float _voltage, int _off){
  int _yOff = battAnimateOffset;

  uint8_t _battPrcnt = constrain(map(_voltage * 1000, 10900, 12600, 0, 100), 0, 100);
  u8g2_SetFont(&u8g2, u8g2_font_profont11_tf);
  u8g2_SetDrawColor(&u8g2, 0);
  u8g2_DrawBox(&u8g2, 0, 52 + _yOff, 128, 11);
  u8g2_SetDrawColor(&u8g2, 1);

  if(HAL_GetTick() - batteryBlinkTime > 250){
    batteryBlinkTime = HAL_GetTick();
    batteryBlinkState = !batteryBlinkState;
  }

    if(_battPrcnt > 15 || batteryBlinkState){
//  if(1){
    char _str[20];
    memset(_str, 0, sizeof(_str));

    sprintf(_str, "Batt: ");
    u8g2_DrawStr(&u8g2, 5, 62 + _yOff, _str);

    memset(_str, 0, sizeof(_str));
    sprintf(_str, "%.1fV", _voltage);

    u8g2_DrawStr(&u8g2, 95, 62 + _yOff, _str);

    uint8_t _boxLength = constrain(map(_voltage * 1000, 10900, 12600, 0, 53), 0, 53);
    u8g2_DrawBox(&u8g2, 38, 55 + _yOff, _boxLength, 7);

    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_DrawRFrame(&u8g2, 38, 55 + _yOff, 53, 7, 0); //Remove excessive box outside frame
    u8g2_DrawRFrame(&u8g2, 38, 55 + _yOff, 53, 7, 1);

    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawRFrame(&u8g2, 38, 55 + _yOff, 53, 7, 3);

    memset(_str, 0, sizeof(_str));
    sprintf(_str, "Cal value: %d", eepVal.batteryCal);
    u8g2_DrawStr(&u8g2, 5, 72 + _yOff, _str);
  }

  if(HAL_GetTick() - batteryOffsetTime > 10){
    batteryOffsetTime = HAL_GetTick();
    if(battAnimateOffset > _off) battAnimateOffset -= ceil(abs(battAnimateOffset - _off) * ANIMATION_SLIDE_SPEED);
    else if(battAnimateOffset < _off) battAnimateOffset += ceil(abs(battAnimateOffset - _off) * ANIMATION_SLIDE_SPEED);
  }
}

void displayBattery(int _yOff){
  drawBatteryLayout(getBatteryVoltage(), _yOff);
}

void batteryCallPage(int _gOff){
  maskTittle();
  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_SetFont(&u8g2, u8g2_font_timB12_tf);
  u8g2_DrawStr(&u8g2, 20, 12 - _gOff, "Battery Cal");
  u8g2_DrawLine(&u8g2, 0, 15, 128, 15);

  displayBattery((getCurrentPage() == BATTERY_CAL_PAGE)? -20: 0);

  uint8_t btnState = getButtonState();
  if(btnState & BTN_PRESS_BACK){
    if(!btnPressed){
      btnPressed = true;
      eepPutFlag = false;
      setCurrentPage(SENSOR_TEST_PAGE);
    }
  }
  else btnPressed = false;

  if(btnState & (BTN_PRESS_UP | BTN_PRESS_DOWN)){
    if(HAL_GetTick() - btnDelayPressTime > ((HAL_GetTick() - btnLongPressTime > 1000)? 1 : 100)){
      btnDelayPressTime = HAL_GetTick();
      if(btnState & BTN_PRESS_UP){
        if(eepVal.batteryCal < 5000) eepVal.batteryCal++;
      }
      else{
        if(eepVal.batteryCal > 0) eepVal.batteryCal--;
      }
    }
  }
  else btnLongPressTime = HAL_GetTick();

}
