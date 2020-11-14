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
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

// Global ingredient counts - on a 1 to 100 scale currently
int ingredient[4] = {0, 0, 0, 0};
char ingBuf[5] = {' ', ' ', ' ', ' ', ' '};

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
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
		  // INGREDIENT SCREEN: GRANOLA BITS
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
		  ILI9341_printText("Would you like ", 20, 10, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_printText("to save this ", 20, 30, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_printText("recipe before", 20, 50, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_printText("making it?", 20, 70, COLOR_BLACK, COLOR_WHITE, 2);
		  ILI9341_drawRect(h/2+50, 10, h-4, 70, COLOR_RED);
		  ILI9341_printText("BACK", h/2+70, 30, COLOR_RED, COLOR_WHITE, 3);
		  ILI9341_printText("Name: Recipe X", 20, 100, COLOR_BLACK, COLOR_WHITE, 2);
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
			  }
		  }

		  ILI9341_printText("Would you like ", 20, 10, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_printText("to save this ", 20, 30, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_printText("recipe before", 20, 50, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_printText("making it?", 20, 70, COLOR_WHITE, COLOR_WHITE, 2);
		  ILI9341_drawRect(h/2+50, 10, h-4, 70, COLOR_WHITE);
		  ILI9341_printText("BACK", h/2+70, 30, COLOR_WHITE, COLOR_WHITE, 3);
		  ILI9341_printText("Name: Recipe X", 20, 100, COLOR_WHITE, COLOR_WHITE, 2);
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

		  fillProgressBar(5000);

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
  HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, TS_CS_Pin|LCD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LCD_DC_Pin */
  GPIO_InitStruct.Pin = LCD_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_DC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_RST_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : TS_CS_Pin LCD_CS_Pin */
  GPIO_InitStruct.Pin = TS_CS_Pin|LCD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
 * Return a 0 - 100 percentage based on where on the IngredientSelect
 * slider bar the touch was detected from.
 *
 * Remember that touch is NOT perfectly accurate so maybe split by 5%
 */
int sliderSplit(int x) {
	// Input should be between 10 and h-10 (310)
	// Rudimentary for now:
	x = x - 5; 	// between 0 and 300 [ adjusting to the left a bit ]
	x = x / 15;		// between 0 and 20
	x = x * 5;		// between 0 and 100 as a multiple of 5 (b/c int)
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
