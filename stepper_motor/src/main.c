/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
// #include <stm32f0xx_gpio.h>
// #include <stm32f0xx_rcc.h>
// #include <stm32f0xx_tim.h>


#define Motor1Step GPIO_Pin_7 //PC7
#define Motor1Dir GPIO_Pin_6 //PC6
#define Motor1M0 GPIO_Pin_8 //PC8
#define Motor1M1 GPIO_Pin_9 //PC9
#define Motor1M2 GPIO_Pin_10 //PC10
#define Motor1EN GPIO_Pin_11 //PC11

#define Motor2Step GPIO_Pin_1 //PC2
#define Motor2Dir GPIO_Pin_2 //PC1
#define Motor2M0 GPIO_Pin_3 //PC3
#define Motor2M1 GPIO_Pin_6 //PF6
#define Motor2M2 GPIO_Pin_0 //PC0
#define Motor2EN GPIO_Pin_12 //PC12

#define Motor3Step GPIO_Pin_11 //PB11
#define Motor3Dir GPIO_Pin_12 //PB12
#define Motor3M0 GPIO_Pin_15 //PB15
#define Motor3M1 GPIO_Pin_14 //PB14
#define Motor3M2 GPIO_Pin_13 //PB13
#define Motor3EN GPIO_Pin_13 //PC13

#define Motor4Step GPIO_Pin_6 //PB6
#define Motor4Dir GPIO_Pin_7 //PB7
#define Motor4M0 GPIO_Pin_8 //PB8
#define Motor4M1 GPIO_Pin_9 //PB9
#define Motor4M2 GPIO_Pin_10 //PB10
#define Motor4EN GPIO_Pin_4 //PF4

#define Motor5Step GPIO_Pin_12 //PA12
#define Motor5Dir GPIO_Pin_15 //PA15
#define Motor5M0 GPIO_Pin_3 //PB3
#define Motor5M1 GPIO_Pin_4 //PB4
#define Motor5M2 GPIO_Pin_5 //PB5
#define Motor5EN GPIO_Pin_5 //PF5

#define ToggleValue ((uint16_t) 3000)

#define MotorGPIOA GPIOA
#define MotorGPIOB GPIOB
#define MotorGPIOC GPIOC
#define MotorGPIOF GPIOF

void micro_wait(unsigned int);

//Initialize structs
GPIO_InitTypeDef MOTOR_A;
GPIO_InitTypeDef MOTOR_B;
GPIO_InitTypeDef MOTOR_C;
GPIO_InitTypeDef MOTOR_F;

TIM_TimeBaseInitTypeDef T1;
TIM_TimeBaseInitTypeDef T2;
TIM_TimeBaseInitTypeDef T3;
TIM_TimeBaseInitTypeDef T4;
TIM_TimeBaseInitTypeDef T5;

/*void runMotor(char ingredient, int portion){
    if(ingredient == "m"){

    }
    else if(ingredient == "p"){

    }
    else if(ingredient == "c"){

    }
    else if(ingredient == "o"){

    }

    return;
}*/

void motorSetup(void){
    //Initialize system and ensure core clock is up to date
         SystemInit();
         SystemCoreClockUpdate();

         //Enable GPIO Clock
         RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); //A
         RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); //B
         RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE); //C
         RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE); //F

         //Enable Timers
         RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //enable Timer 2 for Motor 1
         RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //enable Timer 3 for Motor 2
         RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //enable Timer 6 for Motor 3
         RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //enable Timer 7 for Motor 4
         RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); //enable Timer 14 for Motor 5

         //Initialize MOTORS
         MOTOR_A.GPIO_Pin = (Motor5Step | Motor5Dir);
         MOTOR_B.GPIO_Pin = (Motor3Step | Motor3Dir | Motor3M0 | Motor3M1 | Motor3M2 | Motor4Step | Motor4Dir | Motor4M0 | Motor4M1 | Motor4M2 | Motor5M0 | Motor5M1 | Motor5M2);
         MOTOR_C.GPIO_Pin = (Motor1Step | Motor1Dir | Motor1M0 | Motor1M1 | Motor1M2 | Motor1EN | Motor2Step | Motor2Dir | Motor2M0 | Motor2M2 | Motor2EN | Motor3EN);
         MOTOR_F.GPIO_Pin = (Motor2M1 | Motor4EN | Motor5EN);

         //Set pins to OUTPUT
         MOTOR_A.GPIO_Mode = GPIO_Mode_OUT;
         MOTOR_B.GPIO_Mode = GPIO_Mode_OUT;
         MOTOR_C.GPIO_Mode = GPIO_Mode_OUT;
         MOTOR_F.GPIO_Mode = GPIO_Mode_OUT;

         //Complete Initialization
         GPIO_Init(MotorGPIOA, &MOTOR_A);
         GPIO_Init(MotorGPIOB, &MOTOR_B);
         GPIO_Init(MotorGPIOC, &MOTOR_C);
         GPIO_Init(MotorGPIOF, &MOTOR_F);

         //Motor GPIOC Motor1EN
         GPIO_SetBits(MotorGPIOC, Motor1EN);
         GPIO_SetBits(MotorGPIOC, Motor2EN);
         GPIO_SetBits(MotorGPIOC, Motor3EN);
         GPIO_SetBits(MotorGPIOF, Motor4EN);
         GPIO_SetBits(MotorGPIOF, Motor5EN);


         //Set bits for DIR for all 5 motors
         GPIO_SetBits(MotorGPIOC, Motor1Dir);
         GPIO_SetBits(MotorGPIOC, Motor2Dir);
         GPIO_SetBits(MotorGPIOB, Motor3Dir);
         GPIO_SetBits(MotorGPIOB, Motor4Dir);
         GPIO_SetBits(MotorGPIOA, Motor5Dir);


         //Initialize and enable timer Motor1
         T1.TIM_ClockDivision = TIM_CKD_DIV1;
         T1.TIM_CounterMode = TIM_CounterMode_Up;
         T1.TIM_Period = 0xFFFF;
         T1.TIM_Prescaler = 1;
         T1.TIM_RepetitionCounter = 0;
         TIM_TimeBaseInit(TIM2, &T1);
         TIM_Cmd(TIM2, ENABLE);


         //Initialize and enable timer Motor2
         T2.TIM_ClockDivision = TIM_CKD_DIV1;
         T2.TIM_CounterMode = TIM_CounterMode_Up;
         T2.TIM_Period = 0xFFFF;
         T2.TIM_Prescaler = 2;
         T2.TIM_RepetitionCounter = 0;
         TIM_TimeBaseInit(TIM3, &T2);
         TIM_Cmd(TIM3, ENABLE);


         //Initialize and enable timer Motor3
         T3.TIM_ClockDivision = TIM_CKD_DIV1;
         T3.TIM_CounterMode = TIM_CounterMode_Up;
         T3.TIM_Period = 0xFFFF;
         T3.TIM_Prescaler = 2;
         T3.TIM_RepetitionCounter = 0;
         TIM_TimeBaseInit(TIM6, &T3);
         TIM_Cmd(TIM6, ENABLE);


         //Initialize and enable timer Motor4
         T4.TIM_ClockDivision = TIM_CKD_DIV1;
         T4.TIM_CounterMode = TIM_CounterMode_Up;
         T4.TIM_Period = 0xFFFF;
         T4.TIM_Prescaler = 2;
         T4.TIM_RepetitionCounter = 0;
         TIM_TimeBaseInit(TIM1, &T4);
         TIM_Cmd(TIM1, ENABLE);


         //Initialize and enable timer Motor5
         T5.TIM_ClockDivision = TIM_CKD_DIV1;
         T5.TIM_CounterMode = TIM_CounterMode_Up;
         T5.TIM_Period = 0xFFFF;
         T5.TIM_Prescaler = 2;
         T5.TIM_RepetitionCounter = 0;
         TIM_TimeBaseInit(TIM14, &T5);
         TIM_Cmd(TIM14, ENABLE);
         return;
}

int main(void)
{
    motorSetup();

     uint16_t CurrentTimerVal1 = 0;
     uint16_t CurrentTimerVal2 = 0;
     uint16_t CurrentTimerVal3 = 0;
     uint16_t CurrentTimerVal4 = 0;
     uint16_t CurrentTimerVal5 = 0;

     GPIO_ResetBits(MotorGPIOC, Motor1EN);

     int j;
     int k;
     //while(1)
     for (k=0; k < 1; k++){

         for (j=0; j <= 31000; j++) //j <= 55000 for peanuts, j <= 30000 for M&Ms for 1 flap dispensing
         {
         //Store current timer value in variable
              CurrentTimerVal1 = TIM_GetCounter(TIM2);
              //CurrentTimerVal2 = TIM_GetCounter(TIM3);
              //CurrentTimerVal3 = TIM_GetCounter(TIM6);
              //CurrentTimerVal4 = TIM_GetCounter(TIM1);
              //CurrentTimerVal5 = TIM_GetCounter(TIM14);

              if(CurrentTimerVal1> ToggleValue){
                   GPIO_SetBits(MotorGPIOC, Motor1Step);
              }
              else{
                   GPIO_ResetBits(MotorGPIOC, Motor1Step);
              }

            /*  if(CurrentTimerVal2> ToggleValue){
                   GPIO_SetBits(MotorGPIOC, Motor2Step);
              }
              else{
                   GPIO_ResetBits(MotorGPIOC, Motor2Step);
              }
              if(CurrentTimerVal3> ToggleValue){
                   GPIO_SetBits(MotorGPIOB, Motor3Step);
              }
              else{
                   GPIO_ResetBits(MotorGPIOB, Motor3Step);
              }
              if(CurrentTimerVal4> ToggleValue){
                   GPIO_SetBits(MotorGPIOB, Motor4Step);
              }
              else{
                   GPIO_ResetBits(MotorGPIOB, Motor4Step);
              }
              if(CurrentTimerVal5> ToggleValue){
                   GPIO_SetBits(MotorGPIOA, Motor5Step);
              }
              else{
                   GPIO_ResetBits(MotorGPIOA, Motor5Step);
              }
    */
              //j++;

              //if (j == 150000){ //alternate motor direction, probs a better way to do this
              if(k == 0 || k == 3){
                  GPIO_ResetBits(MotorGPIOC, Motor1Dir);
              }
              else if(k == 1 || k == 2){
                  GPIO_SetBits(MotorGPIOC, Motor1Dir);
              }
                  //GPIO_ResetBits(MotorGPIOC, Motor2Dir);
    //        	  GPIO_ResetBits(MotorGPIOB, Motor3Dir);
    //        	  GPIO_ResetBits(MotorGPIOB, Motor4Dir);
    //        	  GPIO_ResetBits(MotorGPIOA, Motor5Dir);
              //}
             /* else if (j == 300000){
                  GPIO_SetBits(MotorGPIOC, Motor1Dir);
                  //GPIO_SetBits(MotorGPIOC, Motor2Dir);
    //        	  GPIO_SetBits(MotorGPIOB, Motor3Dir);
    //        	  GPIO_SetBits(MotorGPIOB, Motor4Dir);
    //        	  GPIO_SetBits(MotorGPIOA, Motor5Dir);
                  j = 0;
              }*/
         }
         if(k != 1){
             micro_wait(100000);
         }
     }
     GPIO_SetBits(MotorGPIOC, Motor1EN);
}
