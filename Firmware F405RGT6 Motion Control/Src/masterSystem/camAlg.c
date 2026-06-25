#include "RCJLightWeightMaster.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"

int camX = 0;
int camWidth = 0;
uint8_t camIdo = 0;
uint8_t camDetect = 0;
uint8_t camDataValid = 0;

uint32_t camObjectLostTime = 0;

void camBegin(){

}

// H1 H2 IDX XH XL YH YL WH WL IDO TL
// 0  1  2   3  4  5  6  7  8  9   10

void camLoop(){
	uint8_t *uartPtr = getUART1Data();
	uint16_t _camXData = uartPtr[3] << 8 | uartPtr[4];

	if(uartPtr[0] == 251 && uartPtr[1] == 250 && uartPtr[10] == 249){
		camDataValid = 1;
		if(_camXData == 0xffff){
			if(HAL_GetTick() - camObjectLostTime > 250){
				camDetect = 0;
				camX = 0;
			}
			else{
				camDetect = 1;
				camX = 0;
			}
		}
		else{
			camObjectLostTime = HAL_GetTick();

			camDetect = 1;

			camX = (int)_camXData - (CAM_X_TOTAL_PX / 2);
			camX *= -1;
			camWidth = uartPtr[7] << 8 | uartPtr[8];
			camIdo = uartPtr[9];
		}
	}
	else{
		UART_DMA_Restart(CAM_UART);
		camDataValid = 0;
		camX = 0;
		camWidth = 0;
		camDetect = 0;
	}
}

int getCamX(){
	return camX;
}

int getCamWidth(){
	return camWidth;
}

uint8_t getCamIdo(){
	return camIdo;
}

uint8_t getCamDetect(){
	return camDetect;
}

uint8_t getCamDataValid(){
	return camDataValid;
}
