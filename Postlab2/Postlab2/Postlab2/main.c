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
#include "USART/USART.h"

uint16_t ADC_val = 0;
uint16_t ADC_let = 0;
uint16_t dig1 = 0;
uint16_t dig2 = 0;
uint16_t dig3 = 0;

uint16_t ADC_val2 = 0;
uint16_t ADC_let2 = 0;
uint16_t dig1_2 = 0;
uint16_t dig2_2 = 0;
uint16_t dig3_2 = 0;
uint16_t dig4_2 = 0;

int main(void)
{
    /* Replace with your application code */
	ADC_init();
	initLCD8bits();
	UART_init();
	
    while (1) 
    {
		ADC_val = ADC_read(0, 0);
		ADC_let = (ADC_val * 500UL) / 1023UL;
		dig1 = ADC_let/100;
		dig2 = (ADC_let % 100) /10;
		dig3 = ADC_let % 10;
		
		ADC_val2 = ADC_read(1, 0);
		ADC_let2 = (ADC_val2 * 1023UL) / 1023UL;
		dig1_2 = ADC_let2 / 1000;
		dig2_2 = (ADC_let2 / 100) % 10;
		dig3_2 = (ADC_let2 / 10) % 10;
		dig4_2 = ADC_let2 % 10;
		
		LCD_Set_Cursor8bit(1,1);
		LCD_Write_String8bit("S1:");
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
		
		LCD_Set_Cursor8bit(7,1);
		LCD_Write_String8bit("S2:");
		LCD_Set_Cursor8bit(7,2);
		LCD_Write_Char8bit(dig1_2 + '0');
		LCD_Set_Cursor8bit(8,2);
		LCD_Write_Char8bit(dig2_2 + '0');
		LCD_Set_Cursor8bit(9,2);
		LCD_Write_Char8bit(dig3_2 + '0');
		LCD_Set_Cursor8bit(10,2);
		LCD_Write_Char8bit(dig4_2 + '0');
    }
}

