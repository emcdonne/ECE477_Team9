#include "flash.h"

void flash_initialize()
{
    HAL_FLASH_Unlock();
}

void flash_deinitialize()
{
    HAL_FLASH_Lock();
}

void flash_erase(uint32_t address, uint32_t pages)
{
    uint32_t error = 0;
    FLASH_EraseInitTypeDef FLASH_EraseInitStruct =
    {
        .TypeErase = FLASH_TYPEERASE_PAGES,
        .PageAddress = address,
        .NbPages = pages
    };

    HAL_FLASHEx_Erase(&FLASH_EraseInitStruct,&error);
}

uint8_t flash_write(volatile uint32_t address, uint64_t * data, uint16_t size)
{
   for (int i = 0; i < size; i++)
     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, ((address)+(i*4)), data[i]))
        return 1;
   return 0;
}

unsigned char * flash_read(volatile uint32_t address, uint16_t size) {
    unsigned char * output;
    uint32_t addr = address;
    for(int i = 0; i < size; i++) {
        output[i] = * (unsigned char *)addr;
        addr = addr + i * 4;
    }
    return output;
}
