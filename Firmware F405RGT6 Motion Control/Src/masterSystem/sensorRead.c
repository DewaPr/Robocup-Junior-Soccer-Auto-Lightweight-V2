#include "RCJLightWeightMaster.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"

extern I2C_HandleTypeDef hi2c1;

void sensorsBegin(){
	/* nothing to begin */
}

uint16_t IRData = 0;
uint8_t IRAuxData = 0;

uint8_t dataFront = 0;
uint8_t dataLeft = 0;
uint8_t dataBack = 0;
uint8_t dataRight = 0;

uint8_t ballCatchedFr = 0;

unsigned long sensorUpdateTime = 0;
uint32_t ballCatchedTimeElapsed = 0;

void sensorsLoop(){
    if(HAL_GetTick() - sensorUpdateTime > 2){
        sensorUpdateTime = HAL_GetTick();
        uint8_t wireRxData[4];

        HAL_I2C_Master_Receive(&hi2c1, 0x55 << 1, wireRxData, 3, 1000);
        IRData = wireRxData[0] << 8 | wireRxData[1];
        IRAuxData = wireRxData[2];

//        HAL_I2C_Master_Receive(&hi2c1, 0x45 << 1, wireRxData, 4, 1000);
        dataFront = wireRxData[0];
        dataLeft = wireRxData[1];
		dataBack = wireRxData[2];
		dataRight = wireRxData[3];
    }

    if(IRAuxData & 0b00000010){
        if(HAL_GetTick() - ballCatchedTimeElapsed > 200 && !ballCatchedFr){
        	ballCatchedFr = 1;
        }
    }
    else{
    	ballCatchedTimeElapsed = HAL_GetTick();
    	ballCatchedFr = 0;
    }
}

uint16_t getIRData(){
    return IRData;
}

uint8_t getIRAuxData(){
    return IRAuxData;
}

float getBallPosition(int _bias, uint8_t _sig){
    uint8_t diff = 0;
    uint8_t lastDiff = 0;
    uint8_t detectCount = 0;
    uint8_t firstDetect = 0;
    uint8_t startValue = 255;
    for(uint8_t i = 0; i < 16; i++){
        if(IRData & (1 << i)){
            if(firstDetect == 0){
                firstDetect = 1;
                startValue = i;
                lastDiff = i;
            }
            detectCount += 1;
            diff = i - lastDiff;
            lastDiff = i;
        }
        if(diff > 6) startValue = i;
    }
    float mean = 255;
    if(detectCount > 0){
        mean = startValue + (detectCount - 1) / 2.0;
        // mean += 7; // offset
        mean += _bias;

        if(_sig){
        	if(mean > 8) mean = (16 - mean) * -1;
        }

        if(mean > 15.5) mean -= 16;
    }
    return mean;
}

uint8_t getBallCatched(uint8_t _mode){
    // if(_mode == BALL_CATCH_MODE_AND) return !(IRAuxData ^ 0b00000011);
    // else if(_mode == BALL_CATCH_MODE_OR) return (bool)IRAuxData;
    // else return 0;

//    return IRAuxData & 0b00000010;
	return ballCatchedFr;
}

uint8_t getLineSensorBits(uint8_t _sel){
	if(_sel == 0) return dataFront;
	else if(_sel == 1) return dataLeft;
	else if(_sel == 2) return dataBack;
	else if(_sel == 3) return dataRight;
	else return 0;
}
