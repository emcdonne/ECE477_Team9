#include "memory.h"
#include "MY_ILI9341.h"
#include "TSC2046.h"

void memoryTest() {
    //ILI9341_printText(data, 0, 0, COLOR_DCYAN, COLOR_WHITE, 3);
    uint32_t address = 0x0800F800;
    readData(address);
    writeData(address);
}

void readData(uint32_t address) {
    uint16_t * out1;
    uint16_t * out2;
    unsigned int w = ILI9341_WIDTH;
    unsigned char output[2];
    out1 = (uint16_t *) address;
    out2 = (uint16_t *) (address + 4);
    output[0] = (unsigned char) (*out1);
    output[1] = (unsigned char) (*out2);
    char * old = "old:";
    ILI9341_printText(old, 0, 0, COLOR_DCYAN, COLOR_WHITE, 3);
    ILI9341_printTextLength(output, w/2, 0, COLOR_DCYAN, COLOR_WHITE, 3, 2);
}

void writeData(uint32_t address) {
    char * new = "new:";
    unsigned char output[2];
    unsigned int h = ILI9341_HEIGHT;
    unsigned int w = ILI9341_WIDTH;
    ILI9341_printText(new, 0, h/2, COLOR_DCYAN, COLOR_WHITE, 3);
    uint64_t data[2] = {48,54};
    output[0] = (unsigned char) data[0];
    output[1] = (unsigned char) data[1];
    flash_initialize();
    flash_erase(address, 2);
    flash_write(address, data, 2);
    flash_deinitialize();
    ILI9341_printTextLength(output, w/2, h/2, COLOR_DCYAN, COLOR_WHITE, 3, 2);
}

void testRecipeStorage() {
    RecipeStruct recipe;
    RecipeStruct recipeRead;
    recipe.ingredient1 = 20000;
    recipe.ingredient2 = 30000;
    recipe.ingredient3 = 40000;
    recipe.ingredient4 = 50000;
    uint32_t address = RECIPE1;
    writeRecipe(address, recipe);
    recipeRead = readRecipe(address);
}

RecipeStruct readRecipe(uint32_t address) {
    RecipeStruct recipe;
    recipe.ingredient1 = *(unsigned short*) address;
    recipe.ingredient2 = *(unsigned short*) (address + 2);
    recipe.ingredient3 = *(unsigned short*) (address + 4);
    recipe.ingredient4 = *(unsigned short*) (address + 6);
    return recipe;
}

void writeRecipe(uint32_t address, RecipeStruct recipe) {
    uint64_t data[4];
    data[0] = recipe.ingredient1;
    data[1] = recipe.ingredient2;
    data[2] = recipe.ingredient3;
    data[3] = recipe.ingredient4;
    flash_initialize();
    flash_erase(address, 2);
    flash_write(address, data, 4);
    flash_deinitialize();
}
