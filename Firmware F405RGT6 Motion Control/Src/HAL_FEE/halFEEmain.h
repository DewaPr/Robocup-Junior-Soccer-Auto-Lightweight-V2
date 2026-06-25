#include "stdio.h"
#include "stdint.h"
#include "main.h"

void erasePage(uint32_t pageAddr);
void setEepSize(uint16_t _size);
void setEepSectorAddr(uint32_t _secAddr);
void eepPut(uint8_t *eepPtr);
void eepGet(uint8_t *eepPtr);
