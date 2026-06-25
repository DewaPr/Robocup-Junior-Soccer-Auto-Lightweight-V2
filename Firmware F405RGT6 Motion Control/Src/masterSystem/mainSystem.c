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

uint8_t robotStart = false;

int globalOffset = 127;
int globalOffChange = 0;

int settingOffset = 0;
int settingOffChange = 0;

int firstLineOffset = 127;
int firstLineOffChange = 0;

unsigned long globalXOffUpdateTime = 0;

void systemBegin(){
//  Serial.println("System starting!");
  // u8g2.setBusClock(1000000);

  actuatorBegin();

  camBegin();
  imuBegin();
  sensorsBegin();
  buttonBegin();

  eepBegin();
  robotStartBegin();

//  u8g2.begin();

//  writeMA(100);
//  HAL_Delay(2000);
//  writeMB(100);
//  HAL_Delay(2000);
//  writeMC(100);
//  HAL_Delay(2000);
//  writeMD(100);
//  HAL_Delay(2000);

//  setOmniValue(150, 0, 0);
//  HAL_Delay(2000);
  killAllMotor();

  displayLogo();

  setCurrentPage(MENU_PAGE);
  u8g2_SendBuffer(&u8g2);
}

uint8_t currentChangePage;
unsigned long loopTime;
void systemLoop(){
  // Serial.println(HAL_GetTick() - loopTime);
  // loopTime = HAL_GetTick();

  if(currentChangePage == RUNNING_MODE_PAGE) runningModePage(globalOffset);
  else if(currentChangePage == SETTINGS_PAGE) settingsPage(globalOffset);
  else if(currentChangePage == SENSOR_TEST_PAGE) sensorTestPage(globalOffset);
  else if(currentChangePage == CAMERA_TEST_PAGE) cameraTestPage(globalOffset);
  else if(currentChangePage == IMU_SENSOR_PAGE) IMUTestPage(globalOffset);
  else if(currentChangePage == BALL_SENSOR_PAGE) IRSensorTestPage(globalOffset);
  else if(currentChangePage == LINE_SENSOR_PAGE) lineSensorTestPage(globalOffset);
  else if(currentChangePage == BATTERY_CAL_PAGE) batteryCallPage(globalOffset);
  else if(currentChangePage == START_PAGE) startPage(globalOffset);
  else menuPage(globalOffset, firstLineOffset);

  if(!robotStart){
    if(HAL_GetTick() - globalXOffUpdateTime > ANIMATION_UPDATE_MILLIS){
      globalXOffUpdateTime = HAL_GetTick();
      if(globalOffset > globalOffChange) globalOffset -= ceil(abs(globalOffset - globalOffChange) * ANIMATION_SLIDE_SPEED);
      else if(globalOffset < globalOffChange){
        globalOffset += ceil(abs(globalOffset + 2) * ANIMATION_SLIDE_SPEED);
        if(globalOffset > globalOffChange) globalOffset = globalOffChange;
      }
      else{
        if(globalOffChange == 127){
          globalOffChange = 0;
          currentChangePage = getCurrentPage();
        }
      }

      if(firstLineOffset > firstLineOffChange) firstLineOffset -= ceil(abs(firstLineOffset - firstLineOffChange) * ANIMATION_SLIDE_SPEED);

      if(settingOffset > settingOffChange) settingOffset -= ceil(abs(settingOffset - settingOffChange) * ANIMATION_SLIDE_SPEED);
      else if(settingOffset < settingOffChange) settingOffset += ceil(abs(settingOffset - settingOffChange) * ANIMATION_SLIDE_SPEED);
    }

    if(!eepPutFlag){
      eepPutFlag = true;
      eepPutValue();
    }
  }
}

void displayUpdate(){
  if(!robotStart){
    u8g2_SendBuffer(&u8g2);
    u8g2_ClearBuffer(&u8g2);
  }
}
