/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>	// I think it looks clean, okay!
#include "stm32f0xx_hal.h"
#include "memory.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
void IngredientSelect(char ingredient[], int size, int ingNum);
void IngredientErase(char ingredient[], int size, int ingNum);
void RecipeDisplay(int recNum);
bool DetectTouch(int rawx, int rawy, int x0, int y0, int x1, int y1);
int updateSlider(int rawTouch, int prevIng);
void fillProgressBar(int fillTime);
void RecipeErase(int recNum);
void InformationScreen();
// MOTOR STUFF
void motorSetup(void);
void motorRun(int motorNum);
void mixingIdle(int wait);
void mixAndBar(int wait, int quarter);
void micro_wait(unsigned int);
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Motor4EN_Pin GPIO_PIN_1
#define Motor4EN_GPIO_Port GPIOA
#define Motor4Dir_Pin GPIO_PIN_2
#define Motor4Dir_GPIO_Port GPIOA
#define Motor4Step_Pin GPIO_PIN_3
#define Motor4Step_GPIO_Port GPIOA
#define Motor2Dir_Pin GPIO_PIN_0
#define Motor2Dir_GPIO_Port GPIOB
#define Motor2Step_Pin GPIO_PIN_1
#define Motor2Step_GPIO_Port GPIOB
#define Motor2EN_Pin GPIO_PIN_2
#define Motor2EN_GPIO_Port GPIOB
#define TS_DO_Pin GPIO_PIN_14
#define TS_DO_GPIO_Port GPIOB
#define T_DIN_Pin GPIO_PIN_15
#define T_DIN_GPIO_Port GPIOB
#define Motor1Dir_Pin GPIO_PIN_6
#define Motor1Dir_GPIO_Port GPIOC
#define Motor1Step_Pin GPIO_PIN_7
#define Motor1Step_GPIO_Port GPIOC
#define Motor1EN_Pin GPIO_PIN_8
#define Motor1EN_GPIO_Port GPIOC
#define Motor3EN_Pin GPIO_PIN_11
#define Motor3EN_GPIO_Port GPIOC
#define LCD_RST_Pin GPIO_PIN_9
#define LCD_RST_GPIO_Port GPIOA
#define LCD_DC_Pin GPIO_PIN_10
#define LCD_DC_GPIO_Port GPIOA
#define Motor3Step_Pin GPIO_PIN_10
#define Motor3Step_GPIO_Port GPIOC
#define Motor3Dir_Pin GPIO_PIN_9
#define Motor3Dir_GPIO_Port GPIOC
#define Motor5Dir_Pin GPIO_PIN_3
#define Motor5Dir_GPIO_Port GPIOB
#define Motor5Step_Pin GPIO_PIN_4
#define Motor5Step_GPIO_Port GPIOB
#define Motor5EN_Pin GPIO_PIN_5
#define Motor5EN_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_6
#define LCD_CS_GPIO_Port GPIOB
#define TS_CS_Pin GPIO_PIN_7
#define TS_CS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
