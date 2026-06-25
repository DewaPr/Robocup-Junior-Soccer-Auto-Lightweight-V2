#include "RCJLightWeightMaster.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include "../u8g2/u8g2.h"

extern u8g2_t u8g2;
extern uint8_t btnPressed;

uint8_t runningCursor = 0;
uint8_t runningSlide = 0;
void runningModePage(int _gOff){
  if(getPageUpdateFlag()){
    resetPageUpdateFlag();

    // u8g2_SetFontPosBaseline();
  }

  uint8_t btnState = getButtonState();
  if(btnState & BTN_PRESS_BACK){
    if(!btnPressed){
      btnPressed = true;
      setCurrentPage(MENU_PAGE);
    }
  }
  else if(btnState & BTN_PRESS_UP){
    if(!btnPressed){
      btnPressed = true;
      if(runningCursor == 1) runningCursor = 0;
      else if(runningSlide > 0) runningSlide--;
    }
  }
  else if(btnState & BTN_PRESS_DOWN){
    if(!btnPressed){
      btnPressed = true;
      if(runningCursor == 0) runningCursor = 1;
//      else if(runningSlide < 3) runningSlide++;
      else if(runningSlide < 4) runningSlide++;
    }
  }
  else if(btnState & BTN_PRESS_OK){
    if(!btnPressed){
      btnPressed = true;
      setCurrentPage(START_PAGE);
//      if(runningCursor + runningSlide == 0) setRobotMode(ROBOT_MODE_ATTACKER1);
//      else if(runningCursor + runningSlide == 1) setRobotMode(ROBOT_MODE_ATTACKER2);
//      else if(runningCursor + runningSlide == 2) setRobotMode(ROBOT_MODE_DEFENDER);
//      else if(runningCursor + runningSlide == 3) setRobotMode(ROBOT_MODE_PENALTY_SHOOT);
//      else setRobotMode(ROBOT_MODE_PENALTY_KEEP);

      if(runningCursor + runningSlide == 0) setRobotMode(ROBOT_MODE_ATTACKER1);
      else if(runningCursor + runningSlide == 1) setRobotMode(ROBOT_MODE_ATTACKER2);
      else if(runningCursor + runningSlide == 2) setRobotMode(ROBOT_MODE_DEFENDER);
      else if(runningCursor + runningSlide == 3) setRobotMode(ROBOT_MODE_PENALTY_SHOOT1);
      else if(runningCursor + runningSlide == 4) setRobotMode(ROBOT_MODE_PENALTY_SHOOT2);
      else setRobotMode(ROBOT_MODE_PENALTY_KEEP);

//      if(runningCursor + runningSlide == 0) setRobotMode(ROBOT_MODE_ATTACKER1);
//      else if(runningCursor + runningSlide == 1) setRobotMode(ROBOT_MODE_ATTACKER2);
//      else if(runningCursor + runningSlide == 2) setRobotMode(ROBOT_MODE_ATTACKER3);
//      else if(runningCursor + runningSlide == 3) setRobotMode(ROBOT_MODE_DEFENDER);
//      else if(runningCursor + runningSlide == 4) setRobotMode(ROBOT_MODE_PENALTY_SHOOT);
//      else setRobotMode(ROBOT_MODE_PENALTY_KEEP);
    }
  }
  else btnPressed = false;

  drawArrowAnimation(_gOff, _gOff, runningCursor * 16);

  int _yOff = runningSlide * -16;
//  drawBoxWithText(10 + _gOff, 20, 108, 14, _yOff, "Attacker 1", (runningCursor + runningSlide) == 0);
//  drawBoxWithText(10 - _gOff, 36, 108, 14, _yOff, "Attacker 2", (runningCursor + runningSlide) == 1);
//  drawBoxWithText(10 + _gOff, 52, 108, 14, _yOff, "Defender", (runningCursor + runningSlide) == 2);
//  drawBoxWithText(10 - _gOff, 68, 108, 14, _yOff, "Penalty Shoot", (runningCursor + runningSlide) == 3);
//  drawBoxWithText(10 + _gOff, 84, 108, 14, _yOff, "Penalty Keep", (runningCursor + runningSlide) == 4);

  drawBoxWithText(10 + _gOff, 20, 108, 14, _yOff, "Attacker 1", (runningCursor + runningSlide) == 0);
  drawBoxWithText(10 - _gOff, 36, 108, 14, _yOff, "Attacker 2", (runningCursor + runningSlide) == 1);
  drawBoxWithText(10 + _gOff, 52, 108, 14, _yOff, "Defender", (runningCursor + runningSlide) == 2);
  drawBoxWithText(10 - _gOff, 68, 108, 14, _yOff, "Penalty Strght", (runningCursor + runningSlide) == 3);
  drawBoxWithText(10 + _gOff, 84, 108, 14, _yOff, "Penalty Side", (runningCursor + runningSlide) == 4);
  drawBoxWithText(10 - _gOff, 100, 108, 14, _yOff, "Penalty Keep", (runningCursor + runningSlide) == 5);

//  drawBoxWithText(10 + _gOff, 20, 108, 14, _yOff, "Attacker 1", (runningCursor + runningSlide) == 0);
//  drawBoxWithText(10 - _gOff, 36, 108, 14, _yOff, "Attacker 2", (runningCursor + runningSlide) == 1);
//  drawBoxWithText(10 + _gOff, 52, 108, 14, _yOff, "Attacker 3", (runningCursor + runningSlide) == 2);
//  drawBoxWithText(10 - _gOff, 68, 108, 14, _yOff, "Defender", (runningCursor + runningSlide) == 3);
//  drawBoxWithText(10 + _gOff, 84, 108, 14, _yOff, "Penalty Shoot", (runningCursor + runningSlide) == 4);
//  drawBoxWithText(10 - _gOff, 100, 108, 14, _yOff, "Penalty Keep", (runningCursor + runningSlide) == 5);

  maskTittle();
  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_SetFont(&u8g2, u8g2_font_timB12_tf);
  u8g2_DrawStr(&u8g2, 11, 12 - _gOff, "Running Mode");
  u8g2_DrawLine(&u8g2, 0, 15, 128, 15);

  displayBattery(0);
}
