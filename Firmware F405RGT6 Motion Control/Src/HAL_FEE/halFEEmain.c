#include "stdio.h"
#include "stdint.h"
#include "main.h"

void erasePage(uint32_t sectorAddr){
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t SECTORError;

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.Sector = FLASH_SECTOR_7;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.NbSectors = 1;

	HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);
}

uint16_t eepSize = 0;
uint32_t eepSecAddr = 0;

void setEepSize(uint16_t _size){
	eepSize = _size;
}

void setEepSectorAddr(uint32_t _secAddr){
	eepSecAddr = _secAddr;
}

void eepGet(uint8_t *eepPtr){
	uint8_t *ptr = eepPtr;
    for(int i = 0; i < eepSize; i++){
        *ptr++ = *(__IO uint8_t *)(eepSecAddr + i);
    }
}

void eepPut(uint8_t *eepPtr){
    const uint8_t *ptr = eepPtr;
    HAL_FLASH_Unlock();
    erasePage(eepSecAddr);
	for(int i = 0; i < eepSize; i++){
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, eepSecAddr + i, *ptr++);
	}
	HAL_FLASH_Lock();
}
