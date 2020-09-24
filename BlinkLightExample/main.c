/* 
 * File:   main.c
 * Author: breng
 *
 * Created on September 16, 2020, 7:15 PM
 */


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "leds.h"
#include "io_mapping.h"
#include "lcd.h"

#define    FCY    16000000UL 
#include <libpic30.h> 

extern void SYS_Initialize ( void ) ;
/*
 * 
 */
void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
}

int main(int argc, char** argv) {
    
    //SYS_Initialize();
    LED_Enable ( LED_BLINK_ALIVE );
    LED_Enable ( LED_BUTTON_PRESSED );
 
    LCD_Initialize ();
    //char c[50] = "SMAT - TEAM 9   Requests 100%";
    //            12345678901234567890123456789012
    char c[64] = "Here's a SMAT :)  Here's a SMAT :)         ";
    
    
    int x = 0;
    int delay = 50;
    while(1){
        LCD_ClearScreen();
        LCD_PutString(&c[x], sizeof(char) * 16);
        LED_On(LED_D3);
        __delay_ms(delay);  
        if(x > 16){
            x = 0;
        } else{
            x++;
        }
        LED_Off(LED_D3);
        
        __delay_ms(delay);
    }
    
    
    LED_Off(LED_D3);

    return (EXIT_SUCCESS);
}

