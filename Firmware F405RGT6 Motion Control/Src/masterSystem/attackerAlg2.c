#include "RCJLightWeightMaster.h"
#include "../HAL_PID/halPID.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"

extern eepObject eepVal;

extern double PIDOutput;

extern float ballPosVal;

extern int robotTheta;
extern int robotSpeed;
extern int robotRotate;

extern uint8_t ballCatchedFlag;
extern uint8_t ballCloseDetectFlag;
extern uint8_t shootFlag;
extern int drSpeed;
extern uint32_t thetaRecoverTime;
extern uint32_t shootHoldTime;
extern uint8_t firstShootFlag;
extern uint32_t runningElapsedTime;
extern uint32_t firstShootMovementTime;
extern uint8_t randomMoveChoosed;

//int lastXCam = 0;

uint8_t firstCamLost = 0;
extern int xCam;

uint32_t sideMoveTime = 0;

uint32_t ballCatchedTime = 0;

uint8_t firstBallCatchedTimeoutFired = 0;
int headingFirstChoosed = 0;

void attackerAlgorithm2(){

	ballPosVal = getBallPosition(8, BALL_VALUE_SIGNED);
	ballCatchedFlag = getBallCatched(BALL_CATCH_MODE_OR);
	ballCloseDetectFlag = getIRAuxData() & 0b00000010;

	if(ballCloseDetectFlag || ballCatchedFlag){
		drSpeed = eepVal.dribblerVal;
	}
	else{
		drSpeed = eepVal.dribblerVal / 2;
	}

//	if(ballCatchedFlag)	lastXCam = getCamX();

	if(firstShootFlag && HAL_GetTick() - runningElapsedTime > 3000) firstShootFlag = 0;

	if(!shootFlag){
		if(ballCatchedFlag){
			if(firstShootFlag){
				if(HAL_GetTick() - firstShootMovementTime > 300){
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
					firstCamLost = 0;
					xCam = getCamX();

					if(HAL_GetTick() - ballCatchedTime > BALL_CATCHED_TIMEOUT){
						if(!firstBallCatchedTimeoutFired){
							firstBallCatchedTimeoutFired = 1;
							headingFirstChoosed = getHeadingDegree();
						}
						if(HAL_GetTick() - ballCatchedTime > BALL_CATCHED_TIMEOUT + SIDE_TIMEOUT_MOVEMENT_TIME){
							ballCatchedTime = HAL_GetTick();
							firstBallCatchedTimeoutFired = 0;
						}

						if(headingFirstChoosed > 0) robotTheta = SIDE_MOVEMENT_THETA;
						else robotTheta = SIDE_MOVEMENT_THETA * -1;

						PIDOutput = halPIDcompute(3, 0, 200, getHeadingDegree(), 0);
					}
					else{
						robotTheta = 0;
						PIDOutput = halPIDcompute(0.2, 0, 500, xCam, 0);
					}

					if(getCamWidth() > CAM_X_TOTAL_PX - CAM_X_SUBSTRACT_VAL){
						if(HAL_GetTick() - shootHoldTime > 100)	shootFlag = 1;
					}
					else shootHoldTime = HAL_GetTick();

					robotSpeed = eepVal.robotSpeedVal * 255 / 100;
				}
				else{
					if(!firstCamLost){
						if(xCam > 0) robotTheta = SIDE_MOVEMENT_THETA;
						else robotTheta = SIDE_MOVEMENT_THETA * -1;
						sideMoveTime = HAL_GetTick();
						firstCamLost = 1;
					}

					if(HAL_GetTick() - sideMoveTime > ZIGZAG_MOVEMENT_TIME){
						sideMoveTime = HAL_GetTick();
						robotTheta *= -1;
					}


					PIDOutput = halPIDcompute(3, 0, 200, getHeadingDegree(), 0);
					robotSpeed = eepVal.robotSpeedVal * 255 / 100;
					shootHoldTime = HAL_GetTick();
				}
			}
		}
		else{
			firstCamLost = 0;
			if(firstShootFlag) firstShootMovementTime = HAL_GetTick();
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
		if(HAL_GetTick() - shootHoldTime < 500){
			robotSpeed = eepVal.robotSpeedVal * 255 / 100;
			robotTheta = 0;
		}
		else if(HAL_GetTick() - shootHoldTime < 700){
			robotSpeed = eepVal.robotSpeedVal * 255 / 100;
			robotTheta = 0;
			drSpeed *= -1;
		}
		else if(HAL_GetTick() - shootHoldTime < 1500){
			robotSpeed = eepVal.robotSpeedVal * 255 / 100;
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
