#include "RCJLightWeightMaster.h"
#include "../HAL_PID/halPID.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"

extern eepObject eepVal;

extern double PIDOutput;

float ballPosVal;

int robotTheta;
int robotSpeed;
int robotRotate;

uint8_t ballCatchedFlag;
uint8_t ballCloseDetectFlag;
extern uint8_t shootFlag;
int drSpeed = 0;
uint32_t thetaRecoverTime = 0;
uint32_t shootHoldTime = 0;
uint32_t ballCatchElapsed = 0;
uint32_t firstShootMovementTime = 0;
extern uint8_t firstShootFlag;
extern uint32_t runningElapsedTime;
extern uint32_t firstShootMovementTime;
extern uint8_t randomMoveChoosed;

extern int xCam;

void attackerAlgorithm1(){

	ballPosVal = getBallPosition(8, BALL_VALUE_SIGNED);
	ballCatchedFlag = getBallCatched(BALL_CATCH_MODE_OR);
	ballCloseDetectFlag = getIRAuxData() & 0b00000010;

	if(ballCloseDetectFlag || ballCatchedFlag){
		drSpeed = eepVal.dribblerVal;
	}
	else{
		drSpeed = eepVal.dribblerVal / 2;
	}

	if(!shootFlag){
		if(ballCatchedFlag){
			if(firstShootFlag){
				if(HAL_GetTick() - firstShootMovementTime > 500){
					firstShootFlag = 0;
				}
				PIDOutput = halPIDcompute(3, 0, 200, getHeadingDegree(), 0);
				if(randomMoveChoosed){
					robotTheta = 80;
					PIDOutput += 20;
				}
				else{
					robotTheta = -80;
					PIDOutput -= 20;
				}
				robotSpeed = eepVal.robotSpeedVal * 255 / 100;
			}
			else{
				if(getCamDetect()){
					xCam = getCamX();
					robotTheta = 0;
					if(getCamWidth() > CAM_X_TOTAL_PX - CAM_X_SUBSTRACT_VAL){
						if(HAL_GetTick() - shootHoldTime > 100)	shootFlag = 1;
					}
					else shootHoldTime = HAL_GetTick();

					PIDOutput = halPIDcompute(0.2, 0, 500, xCam, 0);
					robotSpeed = eepVal.robotSpeedVal * 255 / 100;
				}
				else{
					shootHoldTime = HAL_GetTick();
					xCam = 0;
					robotTheta = 0;
					PIDOutput = halPIDcompute(3, 0, 200, getHeadingDegree(), 0);
					robotSpeed = eepVal.robotSpeedVal * 255 / 100;
					shootHoldTime = HAL_GetTick();
				}
				if(HAL_GetTick() - ballCatchElapsed > 8000) shootFlag = 1;
			}
		}
		else{
			ballCatchElapsed = HAL_GetTick();
			if(fabs(ballPosVal) > 7 && ballPosVal != 255){
				if(ballPosVal > 0) PIDOutput = -50;
				else if(ballPosVal < 0) PIDOutput = 50;
			}
			else if(fabs(ballPosVal) > 2|| fabs(getHeadingDegree()) > 70) PIDOutput = halPIDcompute(3, 0, 200, getHeadingDegree(), 0);
			else PIDOutput = halPIDcompute(5, 0, 500, ballPosVal, 0);
		//	if(ballPosVal != 255) PIDOutput = halPIDcompute(ballPosVal * 30, 0);
		//	else PIDOutput = 0;

			if(ballPosVal != 255){
		//	if(0){
				if(ballCatchedFlag){
					robotTheta = 0;
				}
				else{
					if(ballPosVal > 0){
						robotTheta = constrain(70 + (ballPosVal * 25), 0, 190);
					}
					else if(ballPosVal < 0){
						robotTheta = constrain(-70 + (ballPosVal * 25), -190, 0);
					}
					else{
						robotTheta = 0;
		//				if(HAL_GetTick() - thetaRecoverTime > 0){
		//					thetaRecoverTime = HAL_GetTick();
		//					if(robotTheta > 0) robotTheta--;
		//					else if(robotTheta < 0) robotTheta++;
		//				}
					}
				}
				robotSpeed = eepVal.robotSpeedVal * 255 / 100;
			}
			else{
				robotTheta = 0;
				robotSpeed = 0;
			}
		}
	}
	else{
		PIDOutput = halPIDcompute(3, 0, 200, getHeadingDegree(), 0);
		if(HAL_GetTick() - shootHoldTime < 200){
			robotSpeed = eepVal.robotSpeedVal * 255 / 100;;
			robotTheta = 0;
			drSpeed *= -1;
		}
		else if(HAL_GetTick() - shootHoldTime < 1000){
			robotSpeed = eepVal.robotSpeedVal * 255 / 100;;
			robotTheta = 180;
			drSpeed *= -1;
		}
		else{
			robotSpeed = 0;
			robotTheta = 0;
			drSpeed = 0;

		}
		if(fabs(ballPosVal) > 3 || (HAL_GetTick() - shootHoldTime > HOLD_AFTER_SHOOT_MS)){
			shootFlag = 0;
			shootHoldTime = HAL_GetTick();
		}
	}

	writeDR1(drSpeed);
	setOmniValue(robotSpeed, robotTheta * -1, PIDOutput);
}
