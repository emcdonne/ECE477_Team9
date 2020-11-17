#include "stm32f0xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#define RECIPE1 0x0800F000
#define RECIPE2 0x0800F008
#define RECIPE3 0x0800F010
#define RECIPE4 0x0800F018
#define RECIPE5 0x0800F020
#define RECIPE6 0x0800F028
#define RECIPE7 0x0800F030
#define RECIPE8 0x0800F038
#define RECIPE(x) 0x0800F000 + (x-1) * 8

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
