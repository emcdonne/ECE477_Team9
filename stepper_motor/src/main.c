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
#define Motor1EN GPIO_Pin_8 //PC8

#define Motor2Step GPIO_Pin_1 //PB1
#define Motor2Dir GPIO_Pin_0 //PB0
#define Motor2EN GPIO_Pin_2 //PB2

#define Motor3Step GPIO_Pin_10 //PC10
#define Motor3Dir GPIO_Pin_9 //PC9
#define Motor3EN GPIO_Pin_11 //PC11

#define Motor4Step GPIO_Pin_3 //PA3
#define Motor4Dir GPIO_Pin_2 //PA2
#define Motor4EN GPIO_Pin_1 //PA1

#define Motor5Step GPIO_Pin_4 //PB4
#define Motor5Dir GPIO_Pin_3 //PB3
#define Motor5EN GPIO_Pin_5 //PB5

#define ToggleValue ((uint16_t) 3000)

#define MotorGPIOA GPIOA
#define MotorGPIOB GPIOB
#define MotorGPIOC GPIOC

void micro_wait(unsigned int);

//Initialize structs
GPIO_InitTypeDef MOTOR_A;
GPIO_InitTypeDef MOTOR_B;
GPIO_InitTypeDef MOTOR_C;

TIM_TimeBaseInitTypeDef T1;
TIM_TimeBaseInitTypeDef T2;
TIM_TimeBaseInitTypeDef T3;
TIM_TimeBaseInitTypeDef T4;
TIM_TimeBaseInitTypeDef T5;

void motorSetup(void){
    //Initialize system and ensure core clock is up to date
         SystemInit();
         SystemCoreClockUpdate();

         //Enable GPIO Clock
         RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); //A
         RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); //B
         RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE); //C

         //Enable Timers
         RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //enable Timer 2 for Motor 1
         RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //enable Timer 3 for Motor 2
         RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //enable Timer 6 for Motor 3
         RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //enable Timer 1 for Motor 4
         RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); //enable Timer 14 for Motor 5

         //Initialize MOTORS
         MOTOR_A.GPIO_Pin = (Motor4Step | Motor4Dir | Motor4EN);
         MOTOR_B.GPIO_Pin = (Motor5Step | Motor5Dir | Motor5EN | Motor2Step | Motor2Dir | Motor2EN);
         MOTOR_C.GPIO_Pin = (Motor1Step | Motor1Dir | Motor1EN | Motor3Step | Motor3Dir | Motor3EN);

         //Set pins to OUTPUT
         MOTOR_A.GPIO_Mode = GPIO_Mode_OUT;
         MOTOR_B.GPIO_Mode = GPIO_Mode_OUT;
         MOTOR_C.GPIO_Mode = GPIO_Mode_OUT;

         //Complete Initialization
         GPIO_Init(MotorGPIOA, &MOTOR_A);
         GPIO_Init(MotorGPIOB, &MOTOR_B);
         GPIO_Init(MotorGPIOC, &MOTOR_C);

         //Motor GPIOC Motor1EN
         GPIO_SetBits(MotorGPIOC, Motor1EN);
         GPIO_SetBits(MotorGPIOB, Motor2EN);
         GPIO_SetBits(MotorGPIOC, Motor3EN);
         GPIO_SetBits(MotorGPIOA, Motor4EN);
         GPIO_SetBits(MotorGPIOB, Motor5EN);


         //Set bits for DIR for all 5 motors
         GPIO_SetBits(MotorGPIOC, Motor1Dir);
         GPIO_SetBits(MotorGPIOB, Motor2Dir);
         GPIO_SetBits(MotorGPIOC, Motor3Dir);
         GPIO_SetBits(MotorGPIOA, Motor4Dir);
         //GPIO_ResetBits(MotorGPIOA, Motor4Dir);
         GPIO_SetBits(MotorGPIOB, Motor5Dir);


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
         T2.TIM_Prescaler = 1;
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

void mixingIdle(int wait){
    int i;
    for(i = 0; i < wait; i++){
        GPIO_SetBits(MotorGPIOC, Motor3Step);
        micro_wait(1000);
        GPIO_ResetBits(MotorGPIOC, Motor3Step);
        micro_wait(1000);
    }
}

void motorRun(int motorNum)
{
    uint16_t currentTimerVal = 0;
    int dispenseInterval = 0;

    //enable the motor driver for said motor
    if(motorNum == 1){ //motor 1 has M&Ms
        GPIO_ResetBits(MotorGPIOC, Motor1EN);
        dispenseInterval = 30000; //30000
    }
    else if(motorNum == 2){ //motor 2 has peanuts
        GPIO_ResetBits(MotorGPIOB, Motor2EN);
        dispenseInterval = 40000; //31000
    }
    else if(motorNum == 4){ //motor 4 has cheerios
        GPIO_ResetBits(MotorGPIOA, Motor4EN);
        dispenseInterval = 40000; //31000
    }
    else if(motorNum == 5){ //motor 5 has granola
        GPIO_ResetBits(MotorGPIOB, Motor5EN);
        dispenseInterval = 30000; //31000
    }
    else{
        return;
    }

    int k;
    int j;

    for (k=0; k < 1; k++){
        for (j=0; j <= dispenseInterval; j++) //j <= 55000 for peanuts, j <= 30000 for M&Ms for 1 flap dispensing
        {
            if(motorNum == 1){
                currentTimerVal = TIM_GetCounter(TIM3);
                if(currentTimerVal > ToggleValue){
                    GPIO_SetBits(MotorGPIOC, Motor1Step);
                    GPIO_SetBits(MotorGPIOC, Motor3Step);
                }
                else{
                    GPIO_ResetBits(MotorGPIOC, Motor1Step);
                    GPIO_ResetBits(MotorGPIOC, Motor3Step);
                }
                if(k == 0 || k == 3){
                    GPIO_ResetBits(MotorGPIOC, Motor1Dir);
                }
                else if(k == 1 || k == 2){
                    GPIO_SetBits(MotorGPIOC, Motor1Dir);
                }
            }
            else if(motorNum == 2){
                currentTimerVal = TIM_GetCounter(TIM3);
                if(currentTimerVal > ToggleValue){
                    GPIO_SetBits(MotorGPIOB, Motor2Step);
                    GPIO_SetBits(MotorGPIOC, Motor3Step);
                }
                else{
                    GPIO_ResetBits(MotorGPIOB, Motor2Step);
                    GPIO_ResetBits(MotorGPIOC, Motor3Step);
                }
                if(k == 0 || k == 3){
                    GPIO_ResetBits(MotorGPIOB, Motor2Dir);
                }
                else if(k == 1 || k == 2){
                    GPIO_SetBits(MotorGPIOB, Motor2Dir);
                }
            }
            else if(motorNum == 4){
                currentTimerVal = TIM_GetCounter(TIM3);
                if(currentTimerVal > ToggleValue){
                    GPIO_SetBits(MotorGPIOA, Motor4Step);
                    GPIO_SetBits(MotorGPIOC, Motor3Step);
                }
                else{
                    GPIO_ResetBits(MotorGPIOA, Motor4Step);
                    GPIO_ResetBits(MotorGPIOC, Motor3Step);
                }
                if(k == 0 || k == 3){
                    GPIO_ResetBits(MotorGPIOA, Motor4Dir);
                }
                else if(k == 1 || k == 2){
                    GPIO_SetBits(MotorGPIOA, Motor4Dir);
                }
            }
            else if(motorNum == 5){
                currentTimerVal = TIM_GetCounter(TIM3);
                if(currentTimerVal > ToggleValue){
                    GPIO_SetBits(MotorGPIOB, Motor5Step);
                    GPIO_SetBits(MotorGPIOC, Motor3Step);
                }
                else{
                    GPIO_ResetBits(MotorGPIOB, Motor5Step);
                    GPIO_ResetBits(MotorGPIOC, Motor3Step);
                }
                if(k == 0 || k == 3){
                    GPIO_ResetBits(MotorGPIOB, Motor5Dir);
                }
                else if(k == 1 || k == 2){
                    GPIO_SetBits(MotorGPIOB, Motor5Dir);
                }
            }
        }
        if(k != 1){
            micro_wait(100000);
        }
    }
    if(motorNum == 1){ //motor 1 has M&Ms
        GPIO_SetBits(MotorGPIOC, Motor1EN);
        //GPIO_SetBits(MotorGPIOC, Motor3EN);
    }
    else if(motorNum == 2){ //motor 2 has peanuts
        GPIO_SetBits(MotorGPIOB, Motor2EN);
    }
    else if(motorNum == 4){ //motor 4 has cheerios
        GPIO_SetBits(MotorGPIOA, Motor4EN);
    }
    else if(motorNum == 5){ //motor 5 has granola
        GPIO_SetBits(MotorGPIOB, Motor5EN);
    }

}

int main(void)
{
    motorSetup();

    //motorRun(1);
    //micro_wait(100000);
    //motorRun(2);
    //micro_wait(100000);
    /*while(1){
            motorRun();
            micro_wait(100000);
    }*/


    /*
    GPIO_ResetBits(MotorGPIOC, Motor3EN);
    uint16_t CurrentTimerVal3 = 0;
    while(1){
        CurrentTimerVal3 = TIM_GetCounter(TIM6);
        if(CurrentTimerVal3> ToggleValue){
            GPIO_SetBits(MotorGPIOC, Motor3Step);
        }
        else{
            GPIO_ResetBits(MotorGPIOC, Motor3Step);
        }
    }

    */

    GPIO_ResetBits(MotorGPIOC, Motor3EN);
    //GPIO_ResetBits(MotorGPIOC, Motor3Dir);
    //mixingIdle(1200);
    motorRun(1);
    //mixingIdle(600);
    /*motorRun(1);
    mixingIdle(600);
    motorRun(1);
    mixingIdle(600);
    motorRun(1);
    mixingIdle(600);
    motorRun(1);
        mixingIdle(600);*/

    /*motorRun(1);
    mixingIdle(600);
    motorRun(2);
    mixingIdle(600);
    motorRun(1);
    mixingIdle(600);
    motorRun(2);
    mixingIdle(600);*/
    GPIO_SetBits(MotorGPIOC, Motor3EN);

    /*
     uint16_t CurrentTimerVal1 = 0;
     uint16_t CurrentTimerVal2 = 0;
     uint16_t CurrentTimerVal3 = 0;
     uint16_t CurrentTimerVal4 = 0;
     uint16_t CurrentTimerVal5 = 0;

     GPIO_ResetBits(MotorGPIOC, Motor1EN);

     int j;
     int k;
     //while(1)
     for (k=0; k < 1; k++)
     {
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

              /*if(CurrentTimerVal2> ToggleValue){
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
              //else if (j == 300000){
                //  GPIO_SetBits(MotorGPIOC, Motor1Dir);
                  //GPIO_SetBits(MotorGPIOC, Motor2Dir);
    //        	  GPIO_SetBits(MotorGPIOB, Motor3Dir);
    //        	  GPIO_SetBits(MotorGPIOB, Motor4Dir);
    //        	  GPIO_SetBits(MotorGPIOA, Motor5Dir);
                  //j = 0;

         }
         if(k != 1){
             micro_wait(100000);
         }
    }
    GPIO_SetBits(MotorGPIOC, Motor1EN);
    */
}
