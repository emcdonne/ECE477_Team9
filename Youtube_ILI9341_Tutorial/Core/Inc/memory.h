#include "stm32f0xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#define RECIPE1 0x0800F000
#define RECIPE(x) 0x08007000 + (x-1) * 0x00000400

typedef struct
{
  unsigned short ingredient1;
  unsigned short ingredient2;
  unsigned short ingredient3;
  unsigned short ingredient4;
} RecipeStruct;

void memoryTest();
void readData(uint32_t address);
void writeData(uint32_t address);
RecipeStruct readRecipe(uint32_t address);
void writeRecipe(uint32_t address, RecipeStruct recipe);
void deleteRecipe(uint32_t address);
bool isValid(uint32_t address);
