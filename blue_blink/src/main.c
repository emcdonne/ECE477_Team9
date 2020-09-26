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
			

int main(void)
{

	RCC-> AHBENR |= (1<<19);
	GPIOC-> MODER |= (1<<16);
	GPIOC-> ODR |= (1<<8);

	int i;

	while(1)
	{
	    for (i=0; i< 10000000; i++);
	    GPIOC-> ODR |= (1<<8);
	    for (i=0; i< 10000000; i++);
	    GPIOC-> ODR &= ~(1<<8);

	}
}
