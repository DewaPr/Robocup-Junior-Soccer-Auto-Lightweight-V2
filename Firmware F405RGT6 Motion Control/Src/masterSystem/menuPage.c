#include "RCJLightWeightMaster.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include "../u8g2/u8g2.h"

extern u8g2_t u8g2;
extern uint8_t btnPressed;

uint8_t menuCursor = 0;
uint8_t menuSlide = 0;

void menuPage(int _gOff, int _lineOff){
  if(getPageUpdateFlag()){
    resetPageUpdateFlag();

    // u8g2_SetFontPosBaseline();
  }

  uint8_t btnState = getButtonState();

  if(btnState & BTN_PRESS_UP){
    if(!btnPressed){
      btnPressed = true;
      if(menuCursor == 1) menuCursor = 0;
      else if(menuSlide == 1) menuSlide = 0;
    }
  }
  else if(btnState & BTN_PRESS_DOWN){
    if(!btnPressed){
      btnPressed = true;
      if(menuCursor == 0) menuCursor = 1;
      else if(menuSlide == 0) menuSlide = 1;
    }
  }
  else if(btnState & BTN_PRESS_OK){
    if(!btnPressed){
      btnPressed = true;
      if(menuCursor + menuSlide == 0) setCurrentPage(RUNNING_MODE_PAGE);
      else if(menuCursor + menuSlide == 1) setCurrentPage(SETTINGS_PAGE);
      else {
        //Credits
      }
    }
  }
  else btnPressed = false;

  drawArrowAnimation(_gOff, _gOff, menuCursor * 16);

  int _yOff = menuSlide * -16;

  drawBoxWithText(10 + _gOff, 20, 108, 14, _yOff, "Running modes", (menuCursor + menuSlide) == 0);
  drawBoxWithText(10 - _gOff, 36, 108, 14, _yOff, "Robot settings", (menuCursor + menuSlide) == 1);
  drawBoxWithText(10 + _gOff, 52, 108, 14, _yOff, "Credits", (menuCursor + menuSlide) == 2);

  maskTittle();
  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_SetFont(&u8g2, u8g2_font_timB12_tf);
  u8g2_DrawStr(&u8g2, 22, 12 - _gOff, "Main menu");
  u8g2_DrawLine(&u8g2, 0, 15, 128 - _lineOff, 15);

  displayBattery(0);
}
