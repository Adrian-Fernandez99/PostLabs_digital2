/*
 * LCD.c
 *
 * Created: 1/22/2026 3:56:18 PM
 *  Author: adria
 */ 
#define F_CPU 16000000UL
#include "LCD.h"
#include <avr/io.h>
#include <util/delay.h>

// Libreria tomada de ejemplo en clase.
// --------------------------- Funciones para modo 4bits --------------------------------------------
// Función para inicializar LCD en modo 4 bits
void initLCD8bits(void)
{
	DDRD |= 0xFF;		// Puerto D es salida
	PORTD |= 0x00;
	DDRB |= 0xFF;		// Puerto B es salida
	PORTB |= 0x00;
	
	_delay_ms(20);
	LCD_CMD8bit(0x30);
	_delay_ms(5);
	LCD_CMD8bit(0x30);
	_delay_us(150);
	LCD_CMD8bit(0x30);
	
	_delay_ms(20);
	//////////////////////////////////////////////////////////////////////
	// Function Set
	_delay_ms(5);
	LCD_CMD8bit(0x38);
	
	// Display ON/OFF
	_delay_ms(5);
	LCD_CMD8bit(0x0C);
	
	// Entry Mode
	_delay_ms(5);
	LCD_CMD8bit(0x06);
	
	// Clear Display
	_delay_ms(5);
	LCD_CMD8bit(0x01);
	_delay_ms(2);
	
}

// Función para enviar un comando
void LCD_CMD8bit(char a)
{
	// RS = 0; // => RS = 0 // Dato en el puerto lo va a interpretar como
	PORTB &= ~(1<<PORTB2);
	PORTD = (PORTD & 0x03) | (a << 2);
	PORTB = (PORTB & 0xFC) | (a >> 6);
	
	
	// EN = 1; // => E = 1
	PORTB |= (1<<PORTB3);
	_delay_ms(4);
	// EN = 0; // => E = 0
	PORTB &= ~(1<<PORTB3);
}

// Función para colocar en el puerto un valor
void LCD_Port4bit(char a)
{
	if (a & 1)
	// D4 = 1;
	PORTC |= (1<<PORTC2);
	else
	// D4 = 0;
	PORTC &= ~(1<<PORTC2);
	
	if (a & 2)
	// D5 = 1;
	PORTC |= (1<<PORTC3);
	else
	// D5 = 0;
	PORTC &= ~(1<<PORTC3);
	
	if (a & 4)
	// D6 = 1;
	PORTC |= (1<<PORTC4);
	else
	// D6 = 0;
	PORTC &= ~(1<<PORTC4);
	
	if (a & 8)
	// D7 = 1;
	PORTC |= (1<<PORTC5);
	else
	// D7 = 0;
	PORTC &= ~(1<<PORTC5);
}

// Función para enviar un caracter
void LCD_Write_Char8bit(char c)
{
	
	// RS = 1; // => RS = 1 // Dato en el puerto la va a interpretar como
	PORTB |= (1<<PORTB2);
	PORTD = (PORTD & 0x03) | (c << 2);
	PORTB = (PORTB & 0xFC) | (c >> 6);
	
	// EN = 1; // => E = 1
	PORTB |= (1<<PORTB3);
	_delay_ms(4);
	// EN = 0; // E => 0
	PORTB &= ~(1<<PORTB3);
}

// Función para enviar una cadena
void LCD_Write_String8bit(char *a)
{
	int i;
	for (i = 0; a[i] != '\0'; i++)
	LCD_Write_Char8bit(a[i]);
}

// Desplazamiento hacia la derecha
void LCD_Shift_Right4bit(void)
{
	LCD_CMD8bit(0x01);
	LCD_CMD8bit(0x0C);
}

// Desplazamiento hacia la izquierda
void LCD_Shift_Left4bit(void)
{
	LCD_CMD8bit(0x01);
	LCD_CMD8bit(0x08);
}

// Establecer el cursor
void LCD_Set_Cursor8bit(char c, char f)
{
	char temp;
	if (f == 1)
	{
		temp = 0x80 + c - 1;
		LCD_CMD8bit(temp);
	}
	else if (f == 2)
	{
		temp = 0xC0 + c - 1;
		LCD_CMD8bit(temp);
	}
}