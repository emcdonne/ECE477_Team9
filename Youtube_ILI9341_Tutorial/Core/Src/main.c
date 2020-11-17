/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "MY_ILI9341.h"
#include "TSC2046.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Declaring helpers here
int convertX(int);
int convertY(int);
int sliderSplit(int x);

unsigned int h = ILI9341_HEIGHT;
unsigned int w = ILI9341_WIDTH;

#define ToggleValue ((uint16_t) 3000)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim3;

// Global ingredient counts - on a 1 to 100 scale currently
int ingredient[4] = {0, 0, 0, 0};
char ingBuf[5] = {' ', ' ', ' ', ' ', ' '};
// Global current recipe
int currentRecipe = 1;
// Global first recipe on the page
int recipeStart = 1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

   HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	// manual
  MX_TIM3_Init();

  motorSetup();

  /* USER CODE BEGIN 2 */
  ILI9341_Init(&hspi2, LCD_CS_GPIO_Port, LCD_CS_Pin, LCD_DC_GPIO_Port, LCD_DC_Pin, LCD_RST_GPIO_Port, LCD_RST_Pin);
  ILI9341_setRotation(2);

  TSC2046_Begin(&hspi1, TS_CS_GPIO_Port, TS_CS_Pin);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  GPIOC-> MODER |= (1<<16);
  GPIOC-> ODR   |= (1<<8);


  unsigned int h = ILI9341_HEIGHT;
  unsigned int w = ILI9341_WIDTH;

  char state[32] = "start";
  ILI9341_Fill(COLOR_WHITE);


  RecipeStruct recTest;
  recTest.ingredient1 = 20;
  recTest.ingredient2 = 0;
  recTest.ingredient3 = 50;
  recTest.ingredient4 = 0;
  writeRecipe(RECIPE(1), recTest);


  //TouchTest();


  while (1)
  {
    /* USER CODE END WHILE */
	  /**
	   * This code is going to implement a state machine using if-else statements [can't switch/case on a string]
	   * Within this loop will be if/elses that will constantly draw to the screen,
	   * wait for input, change state, and erase the changes made.
	   *
	   * To erase simple changes, simply run all commands but with all colors set to WHITE.
	   * This will perfectly return every single pixel to the background color.
	   * However, some screens are complex enough that re-filling with white is faster.
	   *
	   * Do that in those cases. Usually takes about 2.25 seconds to full clear
	   *
	   * White will be assumed as the default background color.
	   */
	  if(strcmp(state, "start") == 0) {
		  // DEFAULT START SCREEN
		  ILI9341_printText("Welcome to the", 35, 10, COLOR_BLACK, COLOR_WHITE, 3);
		  ILI9341_printText("Boiler Mixer!", 50, 40, COLOR_BLACK, COLOR_WHITE, 3);
		  ILI9341_printText("Would you like to create", 20, 75, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_printText("a new recipe or load an", 20, 95, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_printText("existing one?", 20, 115, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_drawRect(4, w/2+15, h/2-5, w-10, COLOR_BLUE);
		  ILI9341_drawRect(h/2+5, w/2+15, h-4, w-10, COLOR_DGREEN);
		  ILI9341_printText("LOAD", 43, w/2+35, COLOR_BLUE, COLOR_WHITE, 3);
		  ILI9341_printText("RECIPE", 25, w/2+65, COLOR_BLUE, COLOR_WHITE, 3);
		  ILI9341_printText("NEW", h/2+52, w/2+35, COLOR_DGREEN, COLOR_WHITE, 3);
		  ILI9341_printText("RECIPE", h/2+26, w/2+65, COLOR_DGREEN, COLOR_WHITE, 3);


		  while(1) {
			  if(TSC2046_getRaw_Z() > 50) {
				  TS_TOUCH_DATA_Def myTouch = TSC2046_GetTouchData();
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, h/2+5, w/2+15, h-4, w-10) ) {
					  strcpy(state,"ipeanuts");
					  break;
				  }
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, 4, w/2+15, h/2-5, w-10) ) {
					  strcpy(state,"recipeSelect");
					  break;
				  }


			  }
		  }

		  ILI9341_Fill(COLOR_WHITE);


	  }
	  else if (strcmp(state, "ipeanuts") == 0) {
		  // INGREDIENT SCREEN: PEANUTS
		  IngredientSelect("PEANUTS", 3, ingredient[0]);

		  while(1) {
			  if(TSC2046_getRaw_Z() > 50) {
				  TS_TOUCH_DATA_Def myTouch = TSC2046_GetTouchData();
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, h/2+5, w/2+15, h-4, w-10) ) {
					  strcpy(state,"im&ms");
					  break;
				  }
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, h/2+50, 10, h-4, 70) ) {
					  strcpy(state,"start");
					  break;
				  }
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, 10, w/2-25, h-10, w/2-5)){
					  ingredient[0] = updateSlider(myTouch.rawX, ingredient[0]);
				  }
			  }

		  }

		  IngredientErase("PEANUTS", 3, ingredient[0]);

	  }
	  else if (strcmp(state, "im&ms") == 0) {
		  // INGREDIENT SCREEN: M&MS
		  IngredientSelect("M&MS", 3, ingredient[1]);

		  while (1) {
			  if(TSC2046_getRaw_Z() > 50) {
				  TS_TOUCH_DATA_Def myTouch = TSC2046_GetTouchData();
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, h/2+5, w/2+15, h-4, w-10) ) {
					  strcpy(state,"icheerios");
					  break;
				  }
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, h/2+50, 10, h-4, 70) ) {
					  strcpy(state,"ipeanuts");
					  break;
				  }
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, 10, w/2-25, h-10, w/2-5)){
					  ingredient[1] = updateSlider(myTouch.rawX, ingredient[1]);
				  }
			  }

		  }

		  IngredientErase("M&MS", 3, ingredient[1]);
	  }
	  else if (strcmp(state, "icheerios") == 0) {
		  // INGREDIENT SCREEN: CHEERIOS
		  IngredientSelect("CHEERIOS", 3, ingredient[2]);

		  while (1) {
			  if(TSC2046_getRaw_Z() > 50) {
				  TS_TOUCH_DATA_Def myTouch = TSC2046_GetTouchData();
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, h/2+5, w/2+15, h-4, w-10) ) {
					  strcpy(state,"igranola");
					  break;
				  }
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, h/2+50, 10, h-4, 70) ) {
					  strcpy(state,"im&ms");
					  break;
				  }
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, 10, w/2-25, h-10, w/2-5)){
					  ingredient[2] = updateSlider(myTouch.rawX, ingredient[2]);
				  }
			  }
		  }

		  IngredientErase("CHEERIOS", 3, ingredient[2]);
	  }
	  else if (strcmp(state, "igranola") == 0) {
		  // INGREDIENT SCREEN: GRANOLA BITS NOW BOONDI
		  IngredientSelect("GRANOLA", 3, ingredient[3]);

		  while (1) {
			  if(TSC2046_getRaw_Z() > 50) {
				  TS_TOUCH_DATA_Def myTouch = TSC2046_GetTouchData();
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, h/2+5, w/2+15, h-4, w-10) ) {
					  strcpy(state,"chooseToSave");
					  break;
				  }
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, h/2+50, 10, h-4, 70) ) {
					  strcpy(state,"icheerios");
					  break;
				  }
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, 10, w/2-25, h-10, w/2-5)){
					  ingredient[3] = updateSlider(myTouch.rawX, ingredient[3]);
				  }
			  }
		  }

		  IngredientErase("GRANOLA", 3, ingredient[3]);
	  }
	  else if (strcmp(state, "chooseToSave") == 0) {
		  // CHOOSE TO SAVE RECIPE OR CONTINUE
		  // Step 1: Find the first available slot
		  int newRecipe = 1;
		  for(int x = 1; x < 33; x++) {	// arbitrary maximum
			  if(!(isValid(RECIPE(x)))) {
				  newRecipe = x;
				  break;
			  }
		  }
		  char buf[16];
		  sprintf(buf,"Name: Recipe%2d", newRecipe);



		  ILI9341_printText("Would you like ", 20, 10, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_printText("to save this ", 20, 30, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_printText("recipe before", 20, 50, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_printText("making it?", 20, 70, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_drawRect(h/2+50, 10, h-4, 70, COLOR_RED);
		  ILI9341_printText("BACK", h/2+70, 30, COLOR_RED, COLOR_WHITE, 3);
		  ILI9341_printText(buf, 20, 100, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_drawRect(4, w/2+15, h/2-5, w-10, COLOR_BLUE);
		  ILI9341_printText("SAVE", 43, w/2+35, COLOR_BLUE, COLOR_WHITE, 3);
		  ILI9341_printText("RECIPE", 25, w/2+65, COLOR_BLUE, COLOR_WHITE, 3);
		  ILI9341_drawRect(h/2+5, w/2+15, h-4, w-10, COLOR_DGREEN);
		  ILI9341_printText("CONTINUE", h/2+10, w/2+50, COLOR_DGREEN, COLOR_WHITE, 3);

		  while(1) {
			  if(TSC2046_getRaw_Z() > 50) {
				  TS_TOUCH_DATA_Def myTouch = TSC2046_GetTouchData();
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, h/2+5, w/2+15, h-4, w-10) ) {
					  strcpy(state,"progressBar");
					  break;
			  	  }
			  	  if(DetectTouch(myTouch.rawX, myTouch.rawY, h/2+50, 10, h-4, 70) ) {
			  		  strcpy(state,"igranola");
			  		  break;
				  }
			  	  if(DetectTouch(myTouch.rawX, myTouch.rawY, 4, w/2+15, h/2-5, w-10)) {
			  		  strcpy(state,"progressBar");
			  		  RecipeStruct rec;
			  		  rec.ingredient1 = ingredient[0];
			  		  rec.ingredient2 = ingredient[1];
			  		  rec.ingredient3 = ingredient[2];
			  		  rec.ingredient4 = ingredient[3];
			  		  writeRecipe(RECIPE(newRecipe), rec);
			  		  break;
			  	  }
			  }
		  }

		  ILI9341_printText("Would you like ", 20, 10, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_printText("to save this ", 20, 30, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_printText("recipe before", 20, 50, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_printText("making it?", 20, 70, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_drawRect(h/2+50, 10, h-4, 70, COLOR_WHITE);
		  ILI9341_printText("BACK", h/2+70, 30, COLOR_WHITE, COLOR_WHITE, 3);
		  ILI9341_printText(buf, 20, 100, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_drawRect(4, w/2+15, h/2-5, w-10, COLOR_WHITE);
		  ILI9341_printText("SAVE", 43, w/2+35, COLOR_WHITE, COLOR_WHITE, 3);
		  ILI9341_printText("RECIPE", 25, w/2+65, COLOR_WHITE, COLOR_WHITE, 3);
		  ILI9341_drawRect(h/2+5, w/2+15, h-4, w-10, COLOR_WHITE);
		  ILI9341_printText("CONTINUE", h/2+10, w/2+50, COLOR_WHITE, COLOR_WHITE, 3);
	  }
	  else if (strcmp(state, "progressBar") == 0) {
		  // SHOW TRAIL MIX COMPLETION
		  ILI9341_printText("Creating your", 45, 10, COLOR_BLACK, COLOR_WHITE, 3);
		  ILI9341_printText("Trail Mix!", 75, 45, COLOR_BLACK, COLOR_WHITE, 3);
		  ILI9341_printText("Please wait until", 10, 80, COLOR_BLACK, COLOR_WHITE, 3);
		  ILI9341_printText("the bar is full.", 10, 115, COLOR_BLACK, COLOR_WHITE, 3);
		  ILI9341_drawRect(10, w/2+30, h-10, w-40, COLOR_BLACK);

		  // CALL MOTOR STUFF
		  HAL_GPIO_WritePin(GPIOC, Motor3EN_Pin, GPIO_PIN_RESET);
		  int ing = 0;
		  int totalIng = (ingredient[0] + ingredient[1] + ingredient[2] + ingredient[3]) / 10;
		  int barUpdate = (h-20) / totalIng;
		  int startSpot = 0;
		  int x;
		  while(ingredient[0] + ingredient[1] + ingredient[2] + ingredient[3] != 0) {
			  if(ingredient[ing] == 0) {
				  ing += 1;
				  if(ing > 3) {
					  ing = 0;
				  }
				  continue;
			  }
			  if(ing == 0) {
				  motorRun(1);
			  } else if(ing == 1) {
				  motorRun(2);
			  } else if(ing == 2) {
				  motorRun(4);
			  } else if(ing == 3) {
				  motorRun(5);
			  }
			  ingredient[ing] -= 10;
			  mixingIdle(600);
			  ing += 1;
			  if(ing > 3) {
				  ing = 0;
			  }
			  for(x = 0; x < barUpdate; x++) {
				  ILI9341_drawFastVLine(10+x+startSpot, w/2+30, 50, COLOR_BLACK);
			  }
			  startSpot += x - 2;	// -1 to combat rounding issues

		  }
		  HAL_GPIO_WritePin(GPIOC, Motor3EN_Pin, GPIO_PIN_SET);
		  for(x = startSpot; x < h-20; x++) {
			  ILI9341_drawFastVLine(10+x, w/2+30, 50, COLOR_BLACK);
		  }
		  //

		  //fillProgressBar(100);

		  ILI9341_printText("Creating your", 45, 10, COLOR_WHITE, COLOR_WHITE, 3);
		  ILI9341_printText("Trail Mix!", 75, 45, COLOR_WHITE, COLOR_WHITE, 3);
		  ILI9341_printText("Please wait until", 10, 80, COLOR_WHITE, COLOR_WHITE, 3);
		  ILI9341_printText("the bar is full.", 10, 115, COLOR_WHITE, COLOR_WHITE, 3);

		  ILI9341_printText("Trail Mix", 80, 10, COLOR_BLACK, COLOR_WHITE, 3);
		  ILI9341_printText("Complete!", 82, 45, COLOR_BLACK, COLOR_WHITE, 3);
		  ILI9341_printText("Please take your trail", 20, 75, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_printText("mix. Thank you for ", 20, 95, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_printText("using Boiler Mixer!", 20, 115, COLOR_BLACK, COLOR_WHITE, 2);

		  while (1) {
			  // Might want to consider using a timer so a user touch can also skip to the first state again
			  HAL_Delay(10000);
			  strcpy(state, "start");
			  break;
		  }

		  ILI9341_Fill_Rect(10, w/2+30, h-10, w-40, COLOR_WHITE);
		  ILI9341_printText("Trail Mix", 80, 10, COLOR_WHITE, COLOR_WHITE, 3);
		  ILI9341_printText("Complete!", 82, 45, COLOR_WHITE, COLOR_WHITE, 3);
		  ILI9341_printText("Please take your trail", 20, 75, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_printText("mix. Thank you for ", 20, 95, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_printText("using Boiler Mixer!", 20, 115, COLOR_WHITE, COLOR_WHITE, 2);

		  ingredient[0] = 0;
		  ingredient[1] = 0;
		  ingredient[2] = 0;
		  ingredient[3] = 0;

	  }
	  else if (strcmp(state, "recipeDisplay") == 0) {
		  // DISPLAY A RECIPE
		  RecipeDisplay(currentRecipe);
		  RecipeStruct recipe = readRecipe(RECIPE(currentRecipe));

		  while (1) {
			  if(TSC2046_getRaw_Z() > 50) {
				  TS_TOUCH_DATA_Def myTouch = TSC2046_GetTouchData();
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, h/2+5, w/2+15, h-4, w-10) ) {
					  strcpy(state,"progressBar");
					  ingredient[0] = recipe.ingredient1;
					  ingredient[1] = recipe.ingredient2;
					  ingredient[2] = recipe.ingredient3;
					  ingredient[3] = recipe.ingredient4;
					  break;
				  }
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, h/2+50, 10, h-4, 70) ) {
					  strcpy(state,"recipeSelect");
					  break;
				  }
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, 10, w/2-25, h-10, w/2-5)){
					  ingredient[3] = updateSlider(myTouch.rawX, ingredient[3]);
				  }
			  }
		  }

		  RecipeErase(currentRecipe);
	  }
	  else if (strcmp(state, "recipeSelect") == 0) {
		  // RECIPE SCREEN: SHOW CURRENT RECIPES
		  // FIRST STEP:
		  // Figure out which recipes are valid
		  char buf1[10];
		  char buf2[10];
		  char buf3[10];
		  char buf4[10];
		  bool showNextPage = true;
		  //int recipeStart = 1;
		  if(isValid(RECIPE(recipeStart))) {
			  sprintf(buf1, "Recipe%2d", recipeStart);
		  } else {
			  sprintf(buf1, "Empty");
			  showNextPage = false;
		  }
		  if(isValid(RECIPE(recipeStart+1))) {
			  sprintf(buf2, "Recipe%2d", recipeStart+1);
		  } else {
			  sprintf(buf2, "Empty");
			  showNextPage = false;
		  }
		  if(isValid(RECIPE(recipeStart+2))) {
			  sprintf(buf3, "Recipe%2d", recipeStart+2);
		  } else {
			  sprintf(buf3, "Empty");
			  showNextPage = false;
		  }
		  if(isValid(RECIPE(recipeStart+3))) {
			  sprintf(buf4, "Recipe%2d", recipeStart+3);
		  } else {
			  sprintf(buf4, "Empty");
			  showNextPage = false;
		  }

		  ILI9341_printText("Select your ", 40, 10, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_printText("  recipe:", 40, 30, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_drawRect(h/2+50, 10, h-4, 70, COLOR_RED);
		  ILI9341_printText("BACK", h/2+70, 30, COLOR_RED, COLOR_WHITE, 3);
		  ILI9341_drawRect(40, 75, 160, 125, COLOR_BLACK);
		  ILI9341_printText(buf1, 45, 90, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_drawRect(165, 75, 285, 125, COLOR_BLACK);
		  ILI9341_printText(buf2, 170, 90, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_drawRect(40, 130, 160, 180, COLOR_BLACK);
		  ILI9341_printText(buf3, 45, 150, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_drawRect(165, 130, 285, 180, COLOR_BLACK);
		  ILI9341_printText(buf4, 170, 150, COLOR_BLACK, COLOR_WHITE, 2);
		  if(showNextPage) {
			  ILI9341_drawRect(40, 190, h-40, w-10, COLOR_DGREEN);
			  ILI9341_printText("NEXT PAGE", 70, w-40, COLOR_DGREEN, COLOR_WHITE, 3);
		  }

		  while (1) {
			  if(TSC2046_getRaw_Z() > 50) {
				  TS_TOUCH_DATA_Def myTouch = TSC2046_GetTouchData();
				  if(DetectTouch(myTouch.rawX, myTouch.rawY, h/2+50, 10, h-4, 70) ) {
					  if(recipeStart == 1) {
						  strcpy(state,"start");
					  } else {
						  // Same state for next time
						  recipeStart -= 4;
					  }
					  break;
				  }
				  if(strcmp(buf1, "Empty") != 0 && DetectTouch(myTouch.rawX, myTouch.rawY, 40, 75, 160, 125) ) {
					  // Recipe 1 (top left)
					  currentRecipe = recipeStart;
					  strcpy(state,"recipeDisplay");
					  break;
				  }
				  if(strcmp(buf2, "Empty") != 0 && DetectTouch(myTouch.rawX, myTouch.rawY, 165, 75, 285, 125) ) {
					  // Recipe 1 (top left)
					  currentRecipe = recipeStart + 1;
					  strcpy(state,"recipeDisplay");
					  break;
				  }
				  if(strcmp(buf3, "Empty") != 0 && DetectTouch(myTouch.rawX, myTouch.rawY, 40, 130, 160, 180) ) {
					  // Recipe 1 (top left)
					  currentRecipe = recipeStart + 2;
					  strcpy(state,"recipeDisplay");
					  break;
				  }
				  if(strcmp(buf4, "Empty") != 0 && DetectTouch(myTouch.rawX, myTouch.rawY, 165, 130, 285, 180) ) {
					  // Recipe 1 (top left)
					  currentRecipe = recipeStart + 3;
					  strcpy(state,"recipeDisplay");
					  break;
				  }
				  if(showNextPage && DetectTouch(myTouch.rawX, myTouch.rawY, 40, 190, h-40, w-10) ) {
					  // Same state for next time
					  recipeStart += 4;
					  break;
				  }
			  }
		  }
		  ILI9341_printText("Select your ", 40, 10, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_printText("  recipe:", 40, 30, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_drawRect(h/2+50, 10, h-4, 70, COLOR_WHITE);
		  ILI9341_printText("BACK", h/2+70, 30, COLOR_WHITE, COLOR_WHITE, 3);
		  ILI9341_drawRect(40, 75, 160, 125, COLOR_WHITE);
		  ILI9341_printText(buf1, 45, 90, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_drawRect(165, 75, 285, 125, COLOR_WHITE);
		  ILI9341_printText(buf2, 170, 90, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_drawRect(40, 130, 160, 180, COLOR_WHITE);
		  ILI9341_printText(buf3, 45, 150, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_drawRect(165, 130, 285, 180, COLOR_WHITE);
		  ILI9341_printText(buf4, 170, 150, COLOR_WHITE, COLOR_WHITE, 2);
		  if(showNextPage) {
			  ILI9341_drawRect(40, 190, h-40, w-10, COLOR_WHITE);
			  ILI9341_printText("NEXT PAGE", 70, w-40, COLOR_WHITE, COLOR_WHITE, 3);
		  }
	  }
	  else {

		  ILI9341_Fill(COLOR_WHITE);
	  }

	/*
	for (i=0; i< 1000000; i++);
		GPIOC-> ODR |= (1<<8);
	for (i=0; i< 1000000; i++);
		GPIOC-> ODR &= ~(1<<8);
		*/
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Motor4EN_Pin|Motor4Dir_Pin|Motor4Step_Pin|LCD_RST_Pin
                          |LCD_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Motor2Dir_Pin|Motor2Step_Pin|Motor2EN_Pin|Motor5Dir_Pin
                          |Motor5Step_Pin|Motor5EN_Pin|LCD_CS_Pin|TS_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Motor1Dir_Pin|Motor1Step_Pin|Motor1EN_Pin|Motor3EN_Pin
                          |Motor3Step_Pin|Motor3Dir_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Motor4EN_Pin Motor4Dir_Pin Motor4Step_Pin LCD_RST_Pin
                           LCD_DC_Pin */
  GPIO_InitStruct.Pin = Motor4EN_Pin|Motor4Dir_Pin|Motor4Step_Pin|LCD_RST_Pin
                          |LCD_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Motor2Dir_Pin Motor2Step_Pin Motor2EN_Pin Motor5Dir_Pin
                           Motor5Step_Pin Motor5EN_Pin LCD_CS_Pin TS_CS_Pin */
  GPIO_InitStruct.Pin = Motor2Dir_Pin|Motor2Step_Pin|Motor2EN_Pin|Motor5Dir_Pin
                          |Motor5Step_Pin|Motor5EN_Pin|LCD_CS_Pin|TS_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : Motor1Dir_Pin Motor1Step_Pin Motor1EN_Pin Motor3EN_Pin
                           Motor3Step_Pin Motor3Dir_Pin */
  GPIO_InitStruct.Pin = Motor1Dir_Pin|Motor1Step_Pin|Motor1EN_Pin|Motor3EN_Pin
                          |Motor3Step_Pin|Motor3Dir_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/**
 * This is user code - heavily modified from their examples
 */
void motorSetup(void) {
	// Set bits for EN and DIR for ALL motors
	HAL_GPIO_WritePin(GPIOA, Motor4EN_Pin|Motor4Dir_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, Motor2EN_Pin|Motor5EN_Pin|Motor2Dir_Pin|Motor5Dir_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, Motor1EN_Pin|Motor3EN_Pin|Motor1Dir_Pin|Motor3Dir_Pin, GPIO_PIN_SET);
}



/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN TIM3_Init 2 */
  TIM3->CR1 |= TIM_CR1_CEN;
  /* USER CODE END TIM3_Init 2 */

}

/* USER CODE BEGIN 4 */

/**
 * Draw the ingredient selections screen
 * Try to keep ingredient down to 10 characters
 * Though you can pass in its size to help
 * [by default the size will be 3]
 *
 * ingNum is the current % of this ingredient (default 0)
 *
 * Only takes about 1.5 seconds to draw
 */
void IngredientSelect(char ingName[], int size, int ingNum) {
	ILI9341_printText("Select Amount:", 20, 10, COLOR_BLACK, COLOR_WHITE, 2);
	ILI9341_printText(ingName, 20, 36, COLOR_BLACK, COLOR_WHITE, size);
	ILI9341_drawRect(h/2+50, 10, h-4, 70, COLOR_RED);
	ILI9341_printText("BACK", h/2+70, 30, COLOR_RED, COLOR_WHITE, 3);
	ILI9341_printText("0%          25          50          75          100", 9, w/2-35, COLOR_BLACK, COLOR_WHITE, 1);
	ILI9341_drawRect(10, w/2-25, h-10, w/2-5, COLOR_BLACK);
	// Dark out the progress bar to avoid going past 100
	int ingSum = ingredient[0] + ingredient[1] + ingredient[2] + ingredient[3] - ingNum;
	ILI9341_Fill_Rect(h-ingSum*3-10, w/2-25, h-10, w/2-5, COLOR_BLACK);
	ILI9341_drawRect(33, w/2+15, 128, w-10, COLOR_BLUE);
	sprintf(ingBuf, "%3d%%", ingNum);
	ILI9341_printText(ingBuf, 35, w/2+50, COLOR_BLACK, COLOR_WHITE, 3);
	ILI9341_drawRect(h/2+5, w/2+15, h-4, w-10, COLOR_DGREEN);
	ILI9341_printText("CONTINUE", h/2+10, w/2+50, COLOR_DGREEN, COLOR_WHITE, 3);

}

/**
 * Pass in the same arguments you did to Ingredient select to wipe this out
 * Just as fast as Ingredient Select: 1.5 seconds!
 */
void IngredientErase(char ingName[], int size, int ingNum) {
	ILI9341_printText("Select Amount:", 20, 10, COLOR_WHITE, COLOR_WHITE, 2);
	ILI9341_printText(ingName, 20, 36, COLOR_WHITE, COLOR_WHITE, size);
	ILI9341_drawRect(h/2+50, 10, h-4, 70, COLOR_WHITE);
	ILI9341_printText("BACK", h/2+70, 30, COLOR_WHITE, COLOR_WHITE, 3);
	ILI9341_printText("0%          25          50          75          100", 9, w/2-35, COLOR_WHITE, COLOR_WHITE, 1);
	ILI9341_Fill_Rect(10, w/2-25, h-10, w/2-5, COLOR_WHITE);
	ILI9341_drawRect(33, w/2+15, 128, w-10, COLOR_WHITE);
	sprintf(ingBuf, "%3d%%", ingNum);
	ILI9341_printText(ingBuf, 35, w/2+50, COLOR_WHITE, COLOR_WHITE, 3);
	ILI9341_drawRect(h/2+5, w/2+15, h-4, w-10, COLOR_WHITE);
	ILI9341_printText("CONTINUE", h/2+10, w/2+50, COLOR_WHITE, COLOR_WHITE, 3);
}

/**
 * Read in from flash and display the ingredients for this recipe
 */
void RecipeDisplay(int recNum) {
	char buf[32];
	RecipeStruct recipe = readRecipe(RECIPE(recNum));
	ILI9341_printText("Would you like ", 20, 10, COLOR_BLACK, COLOR_WHITE, 2);
	ILI9341_printText(" this recipe?", 20, 30, COLOR_BLACK, COLOR_WHITE, 2);
	sprintf(buf, "Name: RECIPE%2d", recNum);
	ILI9341_printText(buf, 20, 70, COLOR_BLACK, COLOR_WHITE, 2);
	sprintf(buf, "PEANUTS:  %3d%%", recipe.ingredient1);
	ILI9341_printText(buf, 20, 90, COLOR_BLACK, COLOR_WHITE, 2);
	sprintf(buf, "M&MS:     %3d%%", recipe.ingredient2);
	ILI9341_printText(buf, 20, 110, COLOR_BLACK, COLOR_WHITE, 2);
	sprintf(buf, "CHEERIOS: %3d%%", recipe.ingredient3);
	ILI9341_printText(buf, 20, 130, COLOR_BLACK, COLOR_WHITE, 2);
	sprintf(buf, "BOONDI:   %3d%%", recipe.ingredient4);
	ILI9341_printText(buf, 20, 150, COLOR_BLACK, COLOR_WHITE, 2);

	ILI9341_drawRect(h/2+50, 10, h-4, 70, COLOR_RED);
	ILI9341_printText("BACK", h/2+70, 30, COLOR_RED, COLOR_WHITE, 3);
	ILI9341_drawRect(40, 170, h-40, w-10, COLOR_DGREEN);
	ILI9341_printText("USE RECIPE", 70, w-50, COLOR_DGREEN, COLOR_WHITE, 3);
}

void RecipeErase(int recNum) {
	char buf[32];
	RecipeStruct recipe = readRecipe(RECIPE(recNum));
	ILI9341_printText("Would you like ", 20, 10, COLOR_WHITE, COLOR_WHITE, 2);
	ILI9341_printText(" this recipe?", 20, 30, COLOR_WHITE, COLOR_WHITE, 2);
	sprintf(buf, "Name: RECIPE%2d", recNum);
	ILI9341_printText(buf, 20, 70, COLOR_WHITE, COLOR_WHITE, 2);
	sprintf(buf, "PEANUTS:  %3d%%", recipe.ingredient1);
	ILI9341_printText(buf, 20, 90, COLOR_WHITE, COLOR_WHITE, 2);
	sprintf(buf, "M&MS:     %3d%%", recipe.ingredient2);
	ILI9341_printText(buf, 20, 110, COLOR_WHITE, COLOR_WHITE, 2);
	sprintf(buf, "CHEERIOS: %3d%%", recipe.ingredient3);
	ILI9341_printText(buf, 20, 130, COLOR_WHITE, COLOR_WHITE, 2);
	sprintf(buf, "BOONDI:   %3d%%", recipe.ingredient4);
	ILI9341_printText(buf, 20, 150, COLOR_WHITE, COLOR_WHITE, 2);

	ILI9341_drawRect(h/2+50, 10, h-4, 70, COLOR_WHITE);
	ILI9341_printText("BACK", h/2+70, 30, COLOR_WHITE, COLOR_WHITE, 3);
	ILI9341_drawRect(40, 170, h-40, w-10, COLOR_WHITE);
	ILI9341_printText("USE RECIPE", 70, w-50, COLOR_WHITE, COLOR_WHITE, 3);
}

/**
 * Return a 0 - 100 percentage based on where on the IngredientSelect
 * slider bar the touch was detected from.
 *
 * Remember that touch is NOT perfectly accurate so maybe split by 5%
 */
int sliderSplit(int x) {
	// Input should be between 10 and h-10 (310)
	// Rudimentary for now:
	x = x - 5; 	// between 0 and 300 [ adjusting to the left a bit ]
	/*
	x = x / 15;		// between 0 and 20
	x = x * 5;		// between 0 and 100 as a multiple of 5 (b/c int)
	*/
	x = x / 30;		// between 0 and 10
	x = x * 10;		// between 0 and 100 as a multiple of 10 (b/c int)
	return x;
}

/**
 * Slider - code: read the x coordinate on the slider,
 * 	then draw the percentage in the lower left hand corner.
 *
 * Returns the ingredient
 */
int updateSlider(int rawTouch, int prevIng) {
	ILI9341_printText(ingBuf, 35, w/2+50, COLOR_WHITE, COLOR_WHITE, 3);	// erase old num
	int newIng = sliderSplit(convertX(rawTouch));
	int prevIngSum = ingredient[0] + ingredient[1] + ingredient[2] + ingredient[3];
	if(prevIngSum - prevIng + newIng >= 100) {
		newIng = 100 - (prevIngSum - prevIng);
	}
	sprintf(ingBuf, "%3d%%", newIng);
	ILI9341_printText(ingBuf, 35, w/2+50, COLOR_BLACK, COLOR_WHITE, 3);
	HAL_Delay(100);
	return newIng;
}

/**
 * fillProgressBar
 *
 * Given an integer number of MILLISECONDS
 * Fills up the progress bar in that much time
 */
void fillProgressBar(int fillTime) {
	int single_delay = fillTime / 300 - 7;	// NOT 300 because drawFastVLine takes its own time
										// this delay was experimentally determined so... yeah
	if(single_delay < 0) {
		single_delay = 0;
	}
	for (int x = 10; x <= 310; x++) {	// 310 = h-10
		ILI9341_drawFastVLine(x, w/2+30, 50, COLOR_BLACK);
		HAL_Delay(single_delay);
	}
}


/**
 * Detect a touch within a rectangle of bounds.
 * The detection is NOT pixel-perfect; the convert() functions must be fixed to address this
 * However, it is close enough that this is not particularly an issue
 *
 * Assumes x0, y0 is the top left of the desired rectangle and x1, y1 is the bottom right
 *
 * Could not pass the touch handlers directly in for some reason
 */
bool DetectTouch(int rawX, int rawY, int x0, int y0, int x1, int y1){
	int xnum = convertX(rawX);
	int ynum = convertY(rawY);

	return(x0 <= xnum && y0 <= ynum && x1 >= xnum && y1 >= ynum);
}


void ColorTest() {
	// Implement Software test 2.1
	ILI9341_Fill(COLOR_GREEN);
	ILI9341_Fill(COLOR_BLUE);
	ILI9341_Fill(COLOR_RED);
	ILI9341_Fill(COLOR_BLACK);
}

void ClearScreen() {
	// Implement Software test 2.2
	// Right now resets everything, can't even init afterwards...
	// Might be better just to use Init again
	ILI9341_SendCommand(ILI9341_RESET);
}

void RectangleTest() {
	// Implement Software test 2.3
	ILI9341_setRotation(2);
	ILI9341_Fill_Rect(h / 2, 0, h - 1, w / 2, COLOR_GREEN);
	HAL_Delay(1000);
	ILI9341_Fill_Rect(0, w / 2, h / 2, w - 1, COLOR_BLUE);
	HAL_Delay(1000);
	ILI9341_Fill_Rect(h / 4, w / 4, h * 3 / 4, w * 3 / 4, COLOR_RED);
}

void TextTest() {
	// Implement Software test 2.4
	ILI9341_setRotation(1); //	portrait
	char* number = "8675309";
	char* purdue = "PURDUE UNIVERSITY";
	char* team1 = "ECE 477";
	char* team2 = " Team 9";
	ILI9341_printText(number, w/2-4, 0, COLOR_WHITE, COLOR_BLACK, 3);
	ILI9341_printText(purdue, 0, h-16, COLOR_YELLOW, COLOR_BLACK, 2);
	ILI9341_printText(team1, 0, h/3, COLOR_BLACK, COLOR_WHITE, 4);
	ILI9341_printText(team2, 0, h/3+34, COLOR_BLACK, COLOR_WHITE, 4);
}

void TouchTest() {
	// Implement "Software test 2.7"
	// Modified, more rigorous touch test
	// More of a mockup of menus
	TS_TOUCH_DATA_Def myTS_Handle;
	int touches[4] = {0, 0, 0, 0};
	int xnum;
	int ynum;

	ILI9341_Fill_Rect(0, 0, h/2-1, w/2-1, COLOR_RED);
	ILI9341_Fill_Rect(0, w/2, h/2-1, w, COLOR_GREEN);
	ILI9341_Fill_Rect(h/2, 0, h, w/2-1, COLOR_BLUE);
	ILI9341_Fill_Rect(h/2, w/2, h, w, COLOR_YELLOW);
	while(1) {
		if(TSC2046_getRaw_Z() > 50) {
			myTS_Handle = TSC2046_GetTouchData();
			xnum = convertX(myTS_Handle.rawX);
			ynum = convertY(myTS_Handle.rawY);

			if(xnum < h/2 && ynum < w/2) {	// TOP LEFT
				if(touches[0] == 0){
					ILI9341_Fill_Rect(0, 0, h/2-1, w/2-1, COLOR_WHITE);
					touches[0] = 1;
				}
				else {
					ILI9341_Fill_Rect(0, 0, h/2-1, w/2-1, COLOR_RED);
					touches[0] = 0;
				}
			}
			else if(xnum < h/2) {	// BOT LEFT
				if(touches[1] == 0){
					ILI9341_Fill_Rect(0, w/2, h/2-1, w, COLOR_DGRAY);
					touches[1] = 1;
				}
				else {
					ILI9341_Fill_Rect(0, w/2, h/2-1, w, COLOR_GREEN);
					touches[1] = 0;
				}
			}
			else if(ynum < h/2) {	// TOP RIGHT
				if(touches[2] == 0){
					ILI9341_Fill_Rect(h/2, 0, h, w/2-1, COLOR_LGRAY);
					touches[2] = 1;
				}
				else {
					ILI9341_Fill_Rect(h/2, 0, h, w/2-1, COLOR_BLUE);
					touches[2] = 0;
				}
			}
			else {
				if(touches[3] == 0){
					ILI9341_Fill_Rect(h/2, w/2, h, w, COLOR_BLACK);
					touches[3] = 1;
				}
				else {
					ILI9341_Fill_Rect(h/2, w/2, h, w, COLOR_YELLOW);
					touches[3] = 0;
				}
			}
		}
	}

}

int convertX(int xnum){
	if (xnum < 2170) {
		xnum = 0;
	}
	else {
		xnum = (xnum - 2170);
	}
	xnum = xnum * h / 1721;
	return xnum;
}

int convertY(int ynum){
	if (ynum < 2250) {
		ynum = 0;
	}
	else if (ynum > 4000) {
		ynum = w;
	}
	else {
		ynum = ynum - 2250;
	}
	ynum = ynum * w / 1705;
	return ynum;
}

// MOTOR STUFF

void motorRun(int motorNum)
{
    uint16_t currentTimerVal = 0;
    int dispenseInterval = 0;

    //enable the motor driver for said motor
    if(motorNum == 1){ //motor 1 has M&Ms
    	HAL_GPIO_WritePin(Motor1EN_GPIO_Port, Motor1EN_Pin, GPIO_PIN_RESET);
        dispenseInterval = 40000; //30000
    }
    else if(motorNum == 2){ //motor 2 has peanuts
    	HAL_GPIO_WritePin(Motor2EN_GPIO_Port, Motor2EN_Pin, GPIO_PIN_RESET);
        dispenseInterval = 40000; //31000
    }
    else if(motorNum == 4){ //motor 4 has cheerios
    	HAL_GPIO_WritePin(Motor4EN_GPIO_Port, Motor4EN_Pin, GPIO_PIN_RESET);
        dispenseInterval = 40000; //31000
    }
    else if(motorNum == 5){ //motor 5 has boondi
    	HAL_GPIO_WritePin(Motor5EN_GPIO_Port, Motor5EN_Pin, GPIO_PIN_RESET);
        dispenseInterval = 40000; //31000
    }
    else{
        return;
    }

    int k;
    int j;

    for (k=0; k < 2; k++){
        if(k == 1){
            dispenseInterval = 20000;
        }
        else{
            dispenseInterval = 60000;
        }
        for (j=0; j <= dispenseInterval; j++) //j <= 55000 for peanuts, j <= 30000 for M&Ms for 1 flap dispensing
        {
            if(motorNum == 1){
                currentTimerVal = TIM3->CNT;
                if(currentTimerVal > ToggleValue){
                	HAL_GPIO_WritePin(Motor1Step_GPIO_Port, Motor1Step_Pin, GPIO_PIN_SET);
                	HAL_GPIO_WritePin(Motor3Step_GPIO_Port, Motor3Step_Pin, GPIO_PIN_SET);
                }
                else{
                	HAL_GPIO_WritePin(Motor1Step_GPIO_Port, Motor1Step_Pin, GPIO_PIN_RESET);
                	HAL_GPIO_WritePin(Motor3Step_GPIO_Port, Motor3Step_Pin, GPIO_PIN_RESET);
                }
                if(k == 0 || k == 3){
                	HAL_GPIO_WritePin(Motor1Dir_GPIO_Port, Motor1Dir_Pin, GPIO_PIN_RESET);
                }
                else if(k == 1 || k == 2){
                	HAL_GPIO_WritePin(Motor1Dir_GPIO_Port, Motor1Dir_Pin, GPIO_PIN_SET);
                }
            }
            else if(motorNum == 2){
                currentTimerVal = TIM3->CNT;
                if(currentTimerVal > ToggleValue){
                    HAL_GPIO_WritePin(Motor2Step_GPIO_Port, Motor2Step_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(Motor3Step_GPIO_Port, Motor3Step_Pin, GPIO_PIN_SET);
                }
                else{
                    HAL_GPIO_WritePin(Motor2Step_GPIO_Port, Motor2Step_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(Motor3Step_GPIO_Port, Motor3Step_Pin, GPIO_PIN_RESET);
                }
                if(k == 0 || k == 3){
                    HAL_GPIO_WritePin(Motor2Dir_GPIO_Port, Motor2Dir_Pin, GPIO_PIN_RESET);
                }
                else if(k == 1 || k == 2){
                    HAL_GPIO_WritePin(Motor2Dir_GPIO_Port, Motor2Dir_Pin, GPIO_PIN_SET);
                }
            }
            else if(motorNum == 4){
                currentTimerVal = TIM3->CNT;
                if(currentTimerVal > ToggleValue){
                    HAL_GPIO_WritePin(Motor4Step_GPIO_Port, Motor4Step_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(Motor3Step_GPIO_Port, Motor3Step_Pin, GPIO_PIN_SET);
                }
                else{
                    HAL_GPIO_WritePin(Motor4Step_GPIO_Port, Motor4Step_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(Motor3Step_GPIO_Port, Motor3Step_Pin, GPIO_PIN_RESET);
                }
                if(k == 0 || k == 3){
                	HAL_GPIO_WritePin(Motor4Dir_GPIO_Port, Motor4Dir_Pin, GPIO_PIN_RESET);
                }
                else if(k == 1 || k == 2){
                    HAL_GPIO_WritePin(Motor4Dir_GPIO_Port, Motor4Dir_Pin, GPIO_PIN_SET);
                }
            }
            else if(motorNum == 5){
                currentTimerVal = TIM3->CNT;
                if(currentTimerVal > ToggleValue){
                    HAL_GPIO_WritePin(Motor5Step_GPIO_Port, Motor5Step_Pin, GPIO_PIN_SET);
                    HAL_GPIO_WritePin(Motor3Step_GPIO_Port, Motor3Step_Pin, GPIO_PIN_SET);
                }
                else{
                    HAL_GPIO_WritePin(Motor5Step_GPIO_Port, Motor5Step_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(Motor3Step_GPIO_Port, Motor3Step_Pin, GPIO_PIN_RESET);
                }
                if(k == 0 || k == 3){
                	HAL_GPIO_WritePin(Motor5Dir_GPIO_Port, Motor5Dir_Pin, GPIO_PIN_RESET);
                }
                else if(k == 1 || k == 2){
                    HAL_GPIO_WritePin(Motor5Dir_GPIO_Port, Motor5Dir_Pin, GPIO_PIN_SET);
                }
            }
        }
        if(k != 1){
            HAL_Delay(100);
        }
    }
    if(motorNum == 1){ //motor 1 has M&Ms
    	HAL_GPIO_WritePin(Motor1EN_GPIO_Port, Motor1EN_Pin, GPIO_PIN_SET);
        //GPIO_SetBits(MotorGPIOC, Motor3EN);
    }
    else if(motorNum == 2){ //motor 2 has peanuts
    	HAL_GPIO_WritePin(Motor2EN_GPIO_Port, Motor2EN_Pin, GPIO_PIN_SET);
    }
    else if(motorNum == 4){ //motor 4 has cheerios
    	HAL_GPIO_WritePin(Motor4EN_GPIO_Port, Motor4EN_Pin, GPIO_PIN_SET);
    }
    else if(motorNum == 5){ //motor 5 has boondi
    	HAL_GPIO_WritePin(Motor5EN_GPIO_Port, Motor5EN_Pin, GPIO_PIN_SET);
    }

}

void mixingIdle(int wait){
    int i;
    for(i = 0; i < wait; i++){
    	HAL_GPIO_WritePin(Motor3Step_GPIO_Port, Motor3Step_Pin, GPIO_PIN_SET);
    	micro_wait(1000);
        HAL_GPIO_WritePin(Motor3Step_GPIO_Port, Motor3Step_Pin, GPIO_PIN_RESET);
        micro_wait(1000);
    }
}

void mixAndBar(int wait, int quarter){
    int i;
    for(i = 0; i < wait; i++){
    	HAL_GPIO_WritePin(Motor3Step_GPIO_Port, Motor3Step_Pin, GPIO_PIN_SET);
        micro_wait(1000);
        HAL_GPIO_WritePin(Motor3Step_GPIO_Port, Motor3Step_Pin, GPIO_PIN_RESET);
        micro_wait(1000);
        //ILI9341_drawFastVLine(10+75*quarter+i*75/wait, w/2+30, 20, COLOR_BLACK);
    }
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
