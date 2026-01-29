/*
 * Lab2.c
 *
 * Created: 1/22/2026 3:55:38 PM
 * Author : adria
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Se incluyen librerias
#include "LCD_2/LCD.h"
#include "ADC/ADC.h"
#include "USART/USART.h"
							
// Se generan variables para el ADC (S1)
uint16_t ADC_val = 0;		// Valor ADC de entrada
uint16_t ADC_let = 0;		// Valor de ADC convertido
uint16_t dig1 = 0;			// Valores de ADC en digitos
uint16_t dig2 = 0;
uint16_t dig3 = 0;

// Se generan variables para el ADC (S2)
uint16_t ADC_val2 = 0;		// Valor ADC de entrada
uint16_t ADC_let2 = 0;		// Valor de ADC convertido
uint16_t dig1_2 = 0;		// Valores de ADC en digitos
uint16_t dig2_2 = 0;
uint16_t dig3_2 = 0;
uint16_t dig4_2 = 0;

// se genera una variable para el contador
uint8_t contador = 0;

int main(void)
{	
    cli ();					// Se deshabilitan interrupciones
	ADC_init();				// Se inicia el ADC
	initLCD8bits();			// Se inicia el LCD
	UART_init();			// Se inicia el UART
	sei();					// Se habilitan las interrupciones
	
    while (1) 
    {
		ADC_val = ADC_read(0, 0);						// Se lee el ADC en el pin 0
		ADC_let = (ADC_val * 500UL) / 1023UL;			// Se convierte a valor de 0 a 5.
		dig1 = ADC_let/100;								// Se obtienen unidades y decimales
		dig2 = (ADC_let % 100) /10;						
		dig3 = ADC_let % 10;
		
		ADC_val2 = ADC_read(1, 0);						// Se lee el ADC en el pin 1
		ADC_let2 = (ADC_val2 * 1023UL) / 1023UL;		// Se mantiene valor.
		dig1_2 = ADC_let2 / 1000;						// Se obtienen unidades, decimas, centesimas y milesimas
		dig2_2 = (ADC_let2 / 100) % 10;
		dig3_2 = (ADC_let2 / 10) % 10;
		dig4_2 = ADC_let2 % 10;
		
		LCD_Set_Cursor8bit(1,1);						// Se setea el cursos en el sitio donde se quiere escribir
		LCD_Write_String8bit("S1:");					// Se escribe el primer marcador
		LCD_Set_Cursor8bit(1,2);						// Se cambia de fila
		LCD_Write_Char8bit(dig1 + '0');					// Se escribe el primer digito convertido a ASCII
		LCD_Set_Cursor8bit(2,2);						// Se repite... 
		LCD_Write_Char8bit('.');
		LCD_Set_Cursor8bit(3,2);
		LCD_Write_Char8bit(dig2 + '0');
		LCD_Set_Cursor8bit(4,2);
		LCD_Write_Char8bit(dig3 + '0');
		LCD_Set_Cursor8bit(5,2);
		LCD_Write_Char8bit('V');
		
		LCD_Set_Cursor8bit(7,1);						// Se setea el cursos en el sitio donde se quiere escribir
		LCD_Write_String8bit("S2:");					// Se escribe el primer marcador
		LCD_Set_Cursor8bit(7,2);						// Se cambia de fila
		LCD_Write_Char8bit(dig1_2 + '0');				// Se escribe el primer digito convertido a ASCII
		LCD_Set_Cursor8bit(8,2);						// Se repite... 
		LCD_Write_Char8bit(dig2_2 + '0');
		LCD_Set_Cursor8bit(9,2);
		LCD_Write_Char8bit(dig3_2 + '0');
		LCD_Set_Cursor8bit(10,2);
		LCD_Write_Char8bit(dig4_2 + '0');
		
		LCD_Set_Cursor8bit(12,1);						// Se setea el cursos en el sitio donde se quiere escirbir
		LCD_Write_String8bit("S3:");					// Se escribe el primer marcador
		LCD_Set_Cursor8bit(12,2);						// Se cambia de fila
		LCD_Write_Char8bit(contador + '0');				// Se escribe el contador convertido a ASCII
														
		write_str("\n S1: ");							// Se carga a la terminal cada uno de los ADC
		write_char(1, dig1 + '0');						// Se usa la misma logica anterior
		write_char(1, '.');								// Donde se va escribiendo digito por digito
		write_char(1, dig2 + '0');
		write_char(1, dig3 + '0');
		write_str(" || S2: ");
		write_char(1, dig1_2 + '0');
		write_char(1, dig2_2 + '0');
		write_char(1, dig3_2 + '0');
		write_char(1, dig4_2 + '0');
    }
}

// Interrupt routines
ISR(USART_RX_vect)
{
	cli();
	
	char temporal = UDR0;								// Se lee el UART
	if (temporal == '+')								// Si es +
	{
		contador = contador + 1;						// Aumenta el contador por uno
		if (contador == 10)								// Si pasa a 10 regresa a 0
		{
			contador = 0;
		}
	}
	else if (temporal == '-')							// Decrementa el contador por uno
	{
		if (contador == 0)								// Si el contador llega a 0 regresa a 10
		{
			contador = 10;
		}
		contador = contador - 1;
	}
 	else
	 {
		 write_str(" \n Simbolo no valido");			// Si no se ingresa "+" o "-" no se acepta
	 }
	
	
	sei();
}