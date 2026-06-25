#include "RCJLightWeightMaster.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"

extern eepObject eepVal;

// #define SOLENOID_ACTIVE_TIME    50
#define SOLENOID_CHARGE_TIME    3000

void actuatorBegin(){
	/* nothing to begin */
}

void updateCCR(uint16_t _pin, uint8_t _val){
//	if(_pin == PWM_IN1_Pin) TIM4 -> CCR3 = _val;
//	else if(_pin == PWM_IN2_Pin) TIM4 -> CCR4 = _val;
//	else if(_pin == PWM_IN3_Pin) TIM1 -> CCR1 = _val;
//	else if(_pin == PWM_IN4_Pin) TIM3 -> CCR4 = _val;
//	else if(_pin == PWM_IN5_Pin) TIM3 -> CCR3 = _val;
//	else if(_pin == PWM_IN6_Pin) TIM1 -> CCR4 = _val;
//	else if(_pin == PWM_IN7_Pin) TIM3 -> CCR1 = _val;
//	else if(_pin == PWM_IN8_Pin) TIM3 -> CCR2 = _val;
//	else if(_pin == PWM_DR1_Pin) TIM2 -> CCR1 = _val;
//	else if(_pin == PWM_DR2_Pin) TIM2 -> CCR2 = _val;
}

//void writeMA(int _val){
//    _val = constrain(_val, -250, 250);
//    if(_val < 0){
////        updateCCR(PWM_IN7_Pin, 0);
////        updateCCR(PWM_IN8_Pin, abs(_val));
//    	TIM3 -> CCR1 = 0;
//    	TIM3 -> CCR2 = abs(_val);
//    }
//    else{
////        updateCCR(PWM_IN7_Pin, _val);
////        updateCCR(PWM_IN8_Pin, 0);
//    	TIM3 -> CCR1 = _val;
//    	TIM3 -> CCR2 = 0;
//    }
//}

//void writeMB(int _val){
void writeMA(int _val){
    _val = constrain(_val, -250, 250);
    if(_val < 0){
//        updateCCR(PWM_IN2_Pin, 0);
//        updateCCR(PWM_IN1_Pin, abs(_val));
    	TIM4 -> CCR4 = 0;
    	TIM4 -> CCR3 = abs(_val);
    }
    else{
//        updateCCR(PWM_IN2_Pin, _val);
//        updateCCR(PWM_IN1_Pin, 0);
    	TIM4 -> CCR4 = _val;
    	TIM4 -> CCR3 = 0;
    }
}

//void writeMC(int _val){
void writeMB(int _val){
    _val = constrain(_val, -250, 250);
    if(_val < 0){
//        updateCCR(PWM_IN3_Pin, 0);
//        updateCCR(PWM_IN4_Pin, abs(_val));
    	TIM1 -> CCR1 = 0;
    	TIM3 -> CCR4 = abs(_val);
    }
    else{
//        updateCCR(PWM_IN3_Pin, _val);
//        updateCCR(PWM_IN4_Pin, 0);
    	TIM1 -> CCR1 = _val;
    	TIM3 -> CCR4 = 0;
    }
}

//void writeMD(int _val){
void writeMC(int _val){
    _val = constrain(_val, -250, 250);
    if(_val < 0){
//        updateCCR(PWM_IN6_Pin, 0);
//        updateCCR(PWM_IN5_Pin, abs(_val));
    	TIM1 -> CCR4 = 0;
    	TIM3 -> CCR3 = abs(_val);
    }
    else{
//        updateCCR(PWM_IN6_Pin, _val);
//        updateCCR(PWM_IN5_Pin, 0);
    	TIM1 -> CCR4 = _val;
    	TIM3 -> CCR3 = 0;
    }
}

void writeMD(int _val){

}

//void setOmniValue(int _vd, int _thetaDeg, int _rotation){
//    float _theta = _thetaDeg * PI / 180;
//    writeMA(round(_vd*sin((60*PI/180) + _theta) + _rotation));
//    writeMB(round(_vd*cos((45*PI/180) + _theta) + _rotation));
//    writeMC(round(_vd*cos((45*PI/180) - _theta) - _rotation));
//    writeMD(round(_vd*sin((60*PI/180) - _theta) - _rotation));
//}

//void writeDR1(int _val){
//    _val = constrain(map(_val, 0, 100, 0, 250), 0, 250);
////    updateCCR(PWM_DR1_Pin, _val);
//    TIM2 -> CCR1 = _val;
//}

void setOmniValue(int _vd, int _thetaDeg, int _rotation){
	float _theta = _thetaDeg * PI / 180;
	writeMA(round(_vd*sin((60*PI/180) + _theta) + _rotation));
	writeMB(round(_vd*cos((90*PI/180) + _theta) + _rotation));
	writeMC(round(_vd*sin((60*PI/180) - _theta) - _rotation));
}

void writeDR1(int _val){
	_val = _val * 255 / 100;
	_val = constrain(_val, -250, 250);
	if(_val < 0){
//        updateCCR(PWM_IN7_Pin, 0);
//        updateCCR(PWM_IN8_Pin, abs(_val));
		TIM3 -> CCR1 = 0;
		TIM3 -> CCR2 = abs(_val);
	}
	else{
//        updateCCR(PWM_IN7_Pin, _val);
//        updateCCR(PWM_IN8_Pin, 0);
		TIM3 -> CCR1 = _val;
		TIM3 -> CCR2 = 0;
	}
}

void writeDR2(int _val){
    // _val = constrain(map(_val, 0, 100, 0, 255), 0, 255);
    // analogWrite(DR2_PIN, _val);
}

unsigned long solenoidActiveTime = 0;
uint8_t solenoidActivated = false;
void activateSolenoid(){
    if(!solenoidActivated){
        solenoidActivated = true;
        HAL_GPIO_WritePin(SOL_GPIO_Port, SOL_Pin, 1);
        solenoidActiveTime = HAL_GetTick();
    }
}

void killAllMotor(){
    setOmniValue(0, 0, 0);
    writeDR1(0);
    writeDR2(0);
}

void actuatorLoop(){
    if(HAL_GetTick() - solenoidActiveTime > SOLENOID_CHARGE_TIME) solenoidActivated = false;
    else if(HAL_GetTick() - solenoidActiveTime > eepVal.kickerVal) HAL_GPIO_WritePin(SOL_GPIO_Port, SOL_Pin, 0);
}
