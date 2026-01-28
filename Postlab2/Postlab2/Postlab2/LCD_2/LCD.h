/*
 * LCD.h
 *
 * Created: 1/22/2026 3:56:28 PM
 *  Author: adria
 */ 

#define F_CPU 16000000UL
#ifndef LCD_H_
#define LCD_H_
#include <avr/io.h>
#include <util/delay.h>

void initLCD8bits(void);
void LCD_CMD8bit(char a);
void LCD_Port4bit(char a);
void LCD_Write_Char8bit(char c);
void LCD_Write_String8bit(char *a);
void LCD_Shift_Right4bit(void);
void LCD_Shift_Left4bit(void);
void LCD_Set_Cursor8bit(char c, char f);

#endif /* LCD_H_ */