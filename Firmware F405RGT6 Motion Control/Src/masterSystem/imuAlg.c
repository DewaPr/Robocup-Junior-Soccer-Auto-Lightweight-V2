#include "RCJLightWeightMaster.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"

float heading;
uint8_t imuDataValid = 0;

void imuBegin(){

}

void imuLoop(){
	uint8_t *uartPtr = getUART2Data();
	if(uartPtr[0] == 170 && uartPtr[7] == 85){
		imuDataValid = 1;
	//	if(uartPtr[1] >= 228) uartPtr[1] -= 116;
		heading = (float)(int16_t)((uint16_t)uartPtr[1] << 8 | uartPtr[2]) / 100;
		heading = 360 - heading;
		if(heading < 0) heading += 360;
		else if(heading > 360) heading -= 360;

		if(heading > 180) heading = (360 - heading) * -1;
	}
	else{
		UART_DMA_Restart(IMU_UART);
		heading = 0;
		imuDataValid = 0;
	}
}

float getHeadingDegree(){
	return heading;
}

uint8_t getImuDataValid(){
	return imuDataValid;
}
