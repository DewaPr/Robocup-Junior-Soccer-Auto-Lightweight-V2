#include "RCJLightWeightMaster.h"
#include "../u8g2/u8g2.h"
#include "../HAL_PID/halPID.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"

extern u8g2_t u8g2;
extern uint8_t btnPressed;

double PIDOutput = 0;

uint8_t randomMoveChoosed;
uint8_t shootFlag = 0;
uint8_t firstShootFlag = 1;
uint32_t runningElapsedTime = 0;

int xCam;

void robotStartBegin(){
//	halPIDbegin(0.7, 0, 1000, P_ON_E, BACKWARD, 0, 0);
	halPIDbegin(P_ON_E, FORWARD, 0, 0);
	halPIDsetOutputLimits(-255, 255);
	halPIDsetWindUpLimits(-10, 10); // Groth bounds for the integral term to prevent integral wind-up
	halPIDstart();
//	halPIDsetSampleTime(50);
}

uint8_t robotMode;
extern uint8_t robotStart;

void setRobotMode(uint8_t _rm){
    robotMode = _rm;
}

void startPage(int _gOff){
    uint8_t btnState = getButtonState();
    if(btnState & BTN_PRESS_BACK){
        if(!btnPressed){
            btnPressed = true;
            if(!robotStart) setCurrentPage(RUNNING_MODE_PAGE);
            else{
                robotStart = false;
                killAllMotor();
            }
        }
    }
    else if(btnState & BTN_PRESS_OK){
        if(!btnPressed){
            btnPressed = true;
            if(!robotStart){
                drawLogo(0);
                shootFlag = 0;
                firstShootFlag = 1;
                randomMoveChoosed = (rand() % 2);
                runningElapsedTime = HAL_GetTick();
                robotStart = true;
                xCam = 0;
                calibrateIMU();
            }
        }
    }
    else btnPressed = false;

    if(robotMode == ROBOT_MODE_ATTACKER1){
        if(!robotStart){
            u8g2_SetFont(&u8g2, u8g2_font_timB14_tf);
            u8g2_DrawStr(&u8g2, 15, 45 + _gOff, "Attacker 1!");
        }
        else{
            attackerAlgorithm1();
        }
    }
    else if(robotMode == ROBOT_MODE_ATTACKER2){
        if(!robotStart){
            u8g2_SetFont(&u8g2, u8g2_font_timB14_tf);
            u8g2_DrawStr(&u8g2, 15, 45 + _gOff, "Attacker 2!");
        }
        else{
            attackerAlgorithm2();
        }
    }
    else if(robotMode == ROBOT_MODE_ATTACKER3){
        if(!robotStart){
            u8g2_SetFont(&u8g2, u8g2_font_timB14_tf);
            u8g2_DrawStr(&u8g2, 15, 45 + _gOff, "Attacker 3!");
        }
        else{
            attackerAlgorithm3();
        }
    }
    else if(robotMode == ROBOT_MODE_DEFENDER){
        if(!robotStart){
            u8g2_SetFont(&u8g2, u8g2_font_timB14_tf);
            u8g2_DrawStr(&u8g2, 25, 45 + _gOff, "Defender!");
        }
        else{
            defenderAlgorithm();
        }
    }
    else if(robotMode == ROBOT_MODE_PENALTY_SHOOT1){
        if(!robotStart){
            u8g2_SetFont(&u8g2, u8g2_font_timB14_tf);
            u8g2_DrawStr(&u8g2, 2, 45 + _gOff, "Penalty Strght!");
        }
        else{
            penaltyAlgorithmS1();
        }
    }
    else if(robotMode == ROBOT_MODE_PENALTY_SHOOT2){
        if(!robotStart){
            u8g2_SetFont(&u8g2, u8g2_font_timB14_tf);
            u8g2_DrawStr(&u8g2, 10, 45 + _gOff, "Penalty Side!");
        }
        else{
            penaltyAlgorithmS2();
        }
    }
    else{
    	if(!robotStart){
			u8g2_SetFont(&u8g2, u8g2_font_timB14_tf);
			u8g2_DrawStr(&u8g2, 10, 45 + _gOff, "Penalty Keep!");
		}
		else{
//			penaltyAlgorithmK();
			defenderAlgorithm();
		}
    }

    if(!robotStart){
        displayBattery(10);
        if(_gOff) u8g2_DrawLine(&u8g2, 0, 15, _gOff, 15);
        u8g2_SetFont(&u8g2, u8g2_font_timB14_tf);
        u8g2_DrawStr(&u8g2, 35, 22 + _gOff, "Ready!");
        u8g2_SetFont(&u8g2, u8g2_font_profont11_tf);
        u8g2_DrawStr(&u8g2, 15, 62 + _gOff, "Press OK to start");
    }
}
