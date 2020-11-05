#include "stm32f0xx_hal.h"
#include <stdint.h>

void flash_initialize();
void flash_deinitialize();
void flash_erase(uint32_t address, uint32_t pages);
uint8_t flash_write(volatile uint32_t address, uint64_t * data, uint16_t size);
uint64_t * flash_read(volatile uint32_t address, uint16_t size);
