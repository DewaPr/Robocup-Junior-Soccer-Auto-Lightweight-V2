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

extern eepObject eepVal;

extern unsigned long btnLongPressTime;
extern unsigned long btnDelayPressTime;

extern int settingOffChange;
extern int settingOffset;

extern uint8_t eepPutFlag;

uint8_t settingsCursor = 0;
uint8_t settingsSlide = 0;
void settingsPage(int _gOff){
  if(getPageUpdateFlag()){
    resetPageUpdateFlag();

    // u8g2.setFontPosBaseline();
  }

  uint8_t btnState = getButtonState();
  if(btnState & BTN_PRESS_BACK){
    if(!btnPressed){
      btnPressed = true;
      if(settingOffChange != 0){
        settingOffChange = 0;
        eepPutFlag = false;
      }
      else setCurrentPage(MENU_PAGE);
    }
  }
  else if(btnState & BTN_PRESS_UP){
    if(!btnPressed){
      btnPressed = true;
      if(settingOffChange == 0){
        if(settingsCursor == 1) settingsCursor = 0;
        else if(settingsSlide > 0) settingsSlide--;
      }
    }
  }
  else if(btnState & BTN_PRESS_DOWN){
    if(!btnPressed){
      btnPressed = true;
      if(settingOffChange == 0){
        if(settingsCursor == 0) settingsCursor = 1;
        else if(settingsSlide < 3) settingsSlide++;
      }
    }
  }
  else if(btnState & BTN_PRESS_OK){
    if(!btnPressed){
      btnPressed = true;
      if(settingsCursor + settingsSlide >= 0 && settingsCursor + settingsSlide < 3) settingOffChange = 40;
      else if(settingsCursor + settingsSlide == 3) setCurrentPage(SENSOR_TEST_PAGE);
      else{

      }
    }
  }
  else btnPressed = false;

  if(settingOffChange != 0){
    if(btnState & (BTN_PRESS_UP | BTN_PRESS_DOWN)){
      if(HAL_GetTick() - btnDelayPressTime > ((HAL_GetTick() - btnLongPressTime > 1000)? 10 : 100)){
        btnDelayPressTime = HAL_GetTick();
        if(btnState & BTN_PRESS_UP){
          if((settingsCursor + settingsSlide) == 0){
            if(eepVal.robotSpeedVal < 100) eepVal.robotSpeedVal++;
          }
          else if((settingsCursor + settingsSlide) == 1){
            if(eepVal.kickerVal < 100) eepVal.kickerVal++;
          }
          else if((settingsCursor + settingsSlide) == 2){
            if(eepVal.dribblerVal < 100) eepVal.dribblerVal++;
          }
        }
        else{
          if((settingsCursor + settingsSlide) == 0){
            if(eepVal.robotSpeedVal > 0) eepVal.robotSpeedVal--;
          }
          else if((settingsCursor + settingsSlide) == 1){
            if(eepVal.kickerVal > 0) eepVal.kickerVal--;
          }
          else if((settingsCursor + settingsSlide) == 2){
            if(eepVal.dribblerVal > 0) eepVal.dribblerVal--;
          }
        }
      }
    }
    else btnLongPressTime = HAL_GetTick();

    if((settingsCursor + settingsSlide) == 1) activateSolenoid();
    else if((settingsCursor + settingsSlide) == 2) writeDR1(eepVal.dribblerVal);
  }
  else killAllMotor();

  drawArrowAnimation(_gOff + settingOffset, _gOff, settingsCursor * 16);

  int _yOff = settingsSlide * -16;

  char _str[20];
  int val;
  if(_gOff == 0){
    memset(_str, 0, sizeof(_str));
    val = eepVal.robotSpeedVal;
    sprintf(_str, "%03d", val);
    drawBoxWithText(80, 20, 38, 14, _yOff, _str, (settingsCursor + settingsSlide) == 0);

    memset(_str, 0, sizeof(_str));
    val = eepVal.kickerVal;
    sprintf(_str, "%03d", val);
    drawBoxWithText(80, 36, 38, 14, _yOff, _str, (settingsCursor + settingsSlide) == 1);

    memset(_str, 0, sizeof(_str));
    val = eepVal.dribblerVal;
    sprintf(_str, "%03d", val);
    drawBoxWithText(80, 52, 38, 14, _yOff, _str, (settingsCursor + settingsSlide) == 2);
  }

  drawBoxWithText(10 + _gOff - ((settingsCursor + settingsSlide == 0)? settingOffset : 0), 20, 108, 14, _yOff, "Robot Speed", (settingsCursor + settingsSlide) == 0);
  drawBoxWithText(10 - _gOff - ((settingsCursor + settingsSlide == 1)? settingOffset : 0), 36, 108, 14, _yOff, "Kicker Power", (settingsCursor + settingsSlide) == 1);
  drawBoxWithText(10 + _gOff - ((settingsCursor + settingsSlide == 2)? settingOffset : 0), 52, 108, 14, _yOff, "Dribbler Speed", (settingsCursor + settingsSlide) == 2);
  drawBoxWithText(10 - _gOff, 68, 108, 14, _yOff, "Sensor Tests", (settingsCursor + settingsSlide) == 3);
  drawBoxWithText(10 + _gOff, 84, 108, 14, _yOff, "Motor Settings", (settingsCursor + settingsSlide) == 4);
  // drawBoxWithText(10 - _gOff, 100, 108, 14, _yOff, "Bla - bla", (settingsCursor + settingsSlide) == 5);
  // drawBoxWithText(10 + _gOff, 116, 108, 14, _yOff, "Bla - bla", (settingsCursor + settingsSlide) == 6);
  // drawBoxWithText(10 + _gOff, 132, 108, 14, _yOff, "Bla - bla", (settingsCursor + settingsSlide) == 7);

  maskTittle();
  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_SetFont(&u8g2, u8g2_font_timB12_tf);
  u8g2_DrawStr(&u8g2, 11, 12 - _gOff, "Robot Settings");
  u8g2_DrawLine(&u8g2, 0, 15, 128, 15);

  displayBattery(0);
}
