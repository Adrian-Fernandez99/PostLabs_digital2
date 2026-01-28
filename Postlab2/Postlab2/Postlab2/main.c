/*
 * Lab2.c
 *
 * Created: 1/22/2026 3:55:38 PM
 * Author : adria
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "LCD_2/LCD.h"
#include "ADC/ADC.h"

uint16_t ADC_val = 0;
uint16_t ADC_let = 0;
uint16_t dig1 = 0;
uint16_t dig2 = 0;
uint16_t dig3 = 0;

int main(void)
{
    /* Replace with your application code */
	ADC_init();
	initLCD8bits();
	
    while (1) 
    {
		ADC_val = ADC_read(0, 0);
		ADC_let = (ADC_val * 500UL) / 255UL;
		dig1 = ADC_let/100;
		dig2 = (ADC_let % 100) /10;
		dig3 = ADC_let % 10;
		
		
		
		LCD_Set_Cursor8bit(1,1);
		LCD_Write_String8bit("Voltaje S1:");
		LCD_Set_Cursor8bit(1,2);
		LCD_Write_Char8bit(dig1 + '0');
		LCD_Set_Cursor8bit(2,2);
		LCD_Write_Char8bit('.');
		LCD_Set_Cursor8bit(3,2);
		LCD_Write_Char8bit(dig2 + '0');
		LCD_Set_Cursor8bit(4,2);
		LCD_Write_Char8bit(dig3 + '0');
		LCD_Set_Cursor8bit(5,2);
		LCD_Write_Char8bit('V');
    }
}

