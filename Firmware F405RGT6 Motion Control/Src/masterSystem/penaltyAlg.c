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
extern uint8_t ballCatchedFlag;

extern int robotTheta;
extern int robotSpeed;
extern int robotRotate;
extern int drSpeed;

extern uint32_t runningElapsedTime;
extern uint8_t randomMoveChoosed;

void penaltyAlgorithmS1(){
	drSpeed = eepVal.dribblerVal * -1;
	robotTheta = 0;

	if(HAL_GetTick() - runningElapsedTime < 700) robotSpeed = eepVal.robotSpeedVal * 255 / 100;
	else if(HAL_GetTick() - runningElapsedTime < 1000) robotSpeed = 0;
	else drSpeed = 0;

	ballPosVal = getBallPosition(8, BALL_VALUE_SIGNED);
	ballCatchedFlag = getBallCatched(BALL_CATCH_MODE_OR);

	PIDOutput = halPIDcompute(4, 0, 200, getHeadingDegree(), 0);

	writeDR1(drSpeed);
	setOmniValue(robotSpeed, robotTheta * -1, PIDOutput);
}

void penaltyAlgorithmS2(){
	drSpeed = eepVal.dribblerVal * -1;
	robotTheta = 0;

	if(HAL_GetTick() - runningElapsedTime < 700) robotSpeed = eepVal.robotSpeedVal * 255 / 100;
	else if(HAL_GetTick() - runningElapsedTime < 1000) robotSpeed = 0;
	else drSpeed = 0;

	ballPosVal = getBallPosition(8, BALL_VALUE_SIGNED);
	ballCatchedFlag = getBallCatched(BALL_CATCH_MODE_OR);

	PIDOutput = halPIDcompute(4, 0, 200, getHeadingDegree(), 0);

	writeDR1(drSpeed);
	setOmniValue(robotSpeed, (robotTheta * -1) - (30 * (randomMoveChoosed? 1 : -1)), PIDOutput + (100 * (randomMoveChoosed? 1 : -1)));
}

void penaltyAlgorithmK(){

}
