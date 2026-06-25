#include "RCJLightWeightMaster.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include "../u8g2/u8g2.h"
#include "../HAL_FEE/halFEEmain.h"

extern u8g2_t u8g2;

eepObject eepVal;

uint8_t eepPutFlag = true;
uint8_t btnPressed = false;

unsigned long btnLongPressTime = 0;
unsigned long btnDelayPressTime = 0;

extern int globalOffChange;

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void displayLoading(){
  u8g2_SetDrawColor(&u8g2, 0);
  u8g2_DrawRBox(&u8g2, 21, 25, 80, 21, 5);
  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_DrawRFrame(&u8g2, 21, 25, 80, 21, 5);
  u8g2_SetFont(&u8g2, u8g2_font_timB12_tf);
  u8g2_DrawStr(&u8g2, 25, 40, "Loading...");
  u8g2_SendBuffer(&u8g2);
}

void eepBegin(){
	setEepSize(sizeof(eepVal));
	setEepSectorAddr(FEE_SECTOR_ADDRESS);

	eepGet((uint8_t *) &eepVal);

	if(eepVal.robotSpeedVal == 0xFF) eepVal.robotSpeedVal = 50;
	if(eepVal.kickerVal == 0xFF) eepVal.kickerVal = 50;
	if(eepVal.dribblerVal == 0xFF) eepVal.dribblerVal = 50;
	if(eepVal.batteryCal == 0xFFFF) eepVal.batteryCal = 2500;
}

void eepPutValue(){
  displayLoading();
  eepPut((uint8_t *) &eepVal);
}

void eepGetValue(){
	eepGet((uint8_t *) &eepVal);
}

unsigned long ADCreadTime = 0;
uint16_t mvaVoltArr[50];
float mvaVoltage;
float getBatteryVoltage()
{
  const int R1 = 10000;
  const int R2 = 1000 + (eepVal.batteryCal * 2 - 5000);
  if(HAL_GetTick() - ADCreadTime > 100){
    ADCreadTime = HAL_GetTick();

    mvaVoltage = 0;

    int _adc = analogRead(BATT_Pin);

    for (int i = 0; i < sizeof(mvaVoltArr) / sizeof(mvaVoltArr[0]) - 1; i++) {
      mvaVoltArr[i] = mvaVoltArr[i + 1];
      if (!mvaVoltArr[i]) mvaVoltArr[i] = _adc;
      mvaVoltage += mvaVoltArr[i];
    }
    mvaVoltArr[sizeof(mvaVoltArr) / sizeof(mvaVoltArr[0]) - 1] = _adc;
    mvaVoltage += _adc;
    mvaVoltage /= (sizeof(mvaVoltArr) / sizeof(mvaVoltArr[0]) - 1);
  }
  return (mvaVoltage * 3.3 / 4095) * (R1 + R2) / R2;
}

void buttonBegin(){
	/* nothing to begin */
}

uint8_t lastBtnVal = 0;
unsigned long debouncingTime = 0;
uint8_t getButtonState(){
  uint8_t _btnVal = 0;
  int btnLVal = analogRead(BTN_L_Pin);
  int btnRVal = analogRead(BTN_R_Pin);

  // Serial.println(String(btnLVal) + ", " + String(btnRVal));

  if(HAL_GetTick() - debouncingTime < 50){
    _btnVal = lastBtnVal;
  }
  else{
    if(btnLVal > 1600 && btnLVal < 2300) _btnVal |= (1 << 5);
    else if(btnLVal > 2300 && btnLVal < 3300) _btnVal |= (1 << 6);
    else if(btnLVal > 3500) _btnVal |= (1 << 7);

    if(btnRVal > 1600 && btnRVal < 2300) _btnVal |= (1 << 2);
    else if(btnRVal > 2300 && btnRVal < 3300) _btnVal |= (1 << 1);
    else if(btnRVal > 3500) _btnVal |= (1 << 0);

    // _btnVal |=  !((uint8_t)digitalRead(OK_PIN))     << 7 |
    //             !((uint8_t)digitalRead(UP_PIN))     << 6 |
    //             !((uint8_t)digitalRead(DOWN_PIN))   << 5 |
    //             !((uint8_t)digitalRead(MINUS_PIN))  << 2 |
    //             !((uint8_t)digitalRead(PLUS_PIN))   << 1 |
    //             !((uint8_t)digitalRead(BACK_PIN))   << 0;
  }

  if(lastBtnVal != _btnVal){
    lastBtnVal = _btnVal;
    debouncingTime = HAL_GetTick();
  }

  return _btnVal;
}

unsigned long arrowFinsUpdateTime = 0;
uint8_t arrowFinsInc = true;
int arrowFinsState = 0;

unsigned long arrowPosUpdateTime = 0;
uint8_t arrowPosInc = true;
int arrowPosState = 0;

unsigned long arrowOffUpdateTime = 0;
int arrowOffState = 0;

void drawArrowAnimation(int _x1GOff, int _x2GOff, int _off){
  int _xOff1 = arrowPosState - _x1GOff;
  int _xOff2 = arrowPosState - _x2GOff;
  int _yOff = arrowOffState;

  u8g2_SetDrawColor(&u8g2, 1);
  for(int i = arrowFinsState; i < (9 - arrowFinsState); i++){
    if(i < 4) u8g2_DrawBox(&u8g2, _xOff1, 23 + i + _yOff, i + 1, 1);
    else u8g2_DrawBox(&u8g2, _xOff1, 23 + i + _yOff, 8 - i + 1, 1);

    if(i < 4) u8g2_DrawBox(&u8g2, 127 - i - _xOff2, 23 + i + _yOff, i + 1, 1);
    else u8g2_DrawBox(&u8g2, 119 + i - _xOff2, 23 + i + _yOff, 8 - i + 1, 1);
  }

  if(HAL_GetTick() - arrowFinsUpdateTime > 75){
    arrowFinsUpdateTime = HAL_GetTick();

    if(arrowFinsInc) arrowFinsState++;
    else arrowFinsState--;

    if(arrowFinsState <= 0) arrowFinsInc = true;
    else if(arrowFinsState >= 3) arrowFinsInc = false;
  }

  if(HAL_GetTick() - arrowPosUpdateTime > 80){
    arrowPosUpdateTime = HAL_GetTick();
    if(arrowPosInc) arrowPosState++;
    else arrowPosState--;

    if(arrowPosState <= 0) arrowPosInc = true;
    else if(arrowPosState >= 4) arrowPosInc = false;
  }

  if(HAL_GetTick() - arrowOffUpdateTime > 10){
    arrowOffUpdateTime = HAL_GetTick();
    if(arrowOffState > _off) arrowOffState -= ceil(abs(arrowOffState - _off) * ANIMATION_SLIDE_SPEED);
    else if(arrowOffState < _off) arrowOffState += ceil(abs(arrowOffState - _off) * ANIMATION_SLIDE_SPEED);
  }
}

unsigned long boxOffUpdateTime = 0;
int boxOffState = 0;
void drawBoxWithText(int _x, int _y, int _w, int _h, int _off, char *_str, uint8_t _selected){
  u8g2_SetFont(&u8g2, u8g2_font_profont11_tf);
  int _yOff = boxOffState;

  if(_selected){
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawBox(&u8g2, _x, _y + _yOff, _w, _h);
    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_DrawRFrame(&u8g2, _x, _y + _yOff, _w, _h, 3);
  }
  else{
    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_DrawBox(&u8g2, _x, _y + _yOff, _w, _h);
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawRFrame(&u8g2, _x, _y + _yOff, _w, _h, 3);
  }

  u8g2_DrawStr(&u8g2, _x + 10, _y + 10 + _yOff, _str);

  if(HAL_GetTick() - boxOffUpdateTime > 10){
    boxOffUpdateTime = HAL_GetTick();
    if(boxOffState > _off) boxOffState -= ceil(abs(boxOffState - _off) * ANIMATION_SLIDE_SPEED);
    else if(boxOffState < _off) boxOffState += ceil(abs(boxOffState - _off) * ANIMATION_SLIDE_SPEED);
  }
}

void maskTittle(){
  u8g2_SetDrawColor(&u8g2, 0);
  u8g2_DrawBox(&u8g2, 0, 0, 128, 20);
  u8g2_DrawBox(&u8g2, 0, 50, 128, 14);
}

uint8_t currentPage = 0;
uint8_t pageUpdate = false;
uint8_t getCurrentPage(){
  return currentPage;
}

void setCurrentPage(uint8_t _page){
  currentPage = _page;
  pageUpdate = true;
  globalOffChange = 127;
}

uint8_t getPageUpdateFlag(){
  return pageUpdate;
}

void resetPageUpdateFlag(){
  pageUpdate = false;
}
