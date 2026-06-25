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
extern uint8_t ballCloseDetectFlag;

extern int robotTheta;
extern int robotSpeed;
extern int robotRotate;
extern int drSpeed;

void defenderAlgorithm(){
	ballPosVal = getBallPosition(8, BALL_VALUE_SIGNED);
	ballCatchedFlag = getBallCatched(BALL_CATCH_MODE_OR);
	ballCloseDetectFlag = getIRAuxData() & 0b00000010;

	if(ballCloseDetectFlag || ballCatchedFlag){
		drSpeed = eepVal.dribblerVal;
	}
	else{
		drSpeed = eepVal.dribblerVal / 2;
	}

	PIDOutput = halPIDcompute(4, 0, 200, getHeadingDegree(), 0);

	if(ballPosVal != 255){
		robotSpeed = eepVal.robotSpeedVal * 255 / 100;
		if(ballPosVal > 0) robotTheta = 90;
		else if(ballPosVal < 0) robotTheta = -90;
		else{
			robotSpeed = 0;
			robotTheta = 0;
		}
	}
	else{
		robotSpeed = 0;
		robotTheta = 0;
	}

	if(ballCatchedFlag) drSpeed *= -1;

	writeDR1(drSpeed);
	setOmniValue(robotSpeed, robotTheta * -1, PIDOutput);
}
