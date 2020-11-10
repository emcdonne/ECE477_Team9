#include "stm32f0xx_hal.h"
#include <stdint.h>

#define RECIPE1 0x0800F000
#define RECIPE2 0x0800F008
#define RECIPE3 0x0800F000
#define RECIPE4 0x0800F008
#define RECIPE5 0x0800F000
#define RECIPE6 0x0800F008
#define RECIPE7 0x0800F000
#define RECIPE8 0x0800F008
#define RECIPE9 0x0800F000
#define RECIPE10 0x0800F008

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
