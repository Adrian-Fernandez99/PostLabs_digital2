/*
	* Lab3.c
	*
	* Created: 1/29/2026 1:38:55 AM
	* Author : adria
	*/ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "SPI/SPI.h"
#include "ADC/ADC.h"
#include "USART/USART.h"

uint8_t valorSPI;
uint8_t ADC_val = 0;

uint8_t ADC_let = 0;		// Valor de ADC convertido
uint8_t dig1 = 0;			// Valores de ADC en digitos
uint8_t dig2 = 0;
uint8_t dig3 = 0;

uint8_t ADC_let2 = 0;		// Valor de ADC convertido
uint8_t dig1_2 = 0;			// Valores de ADC en digitos
uint8_t dig2_2 = 0;
uint8_t dig3_2 = 0;

uint8_t counter = 0;
uint16_t numero;   // acumulador
uint8_t leds = 0;

uint8_t escribir = 0;

void refreshPORT(uint8_t valor);
void init_master();
void init_slave();

// Maestro
int main(void)
{	
	cli();
	init_master();
	SPINIT(SPI_MST_OSC_DIV16, SPI_DT_ORD_MSB, SPI_CLK_IDLE_L, SPI_CLK_FEDGE);
	UART_init();
	sei();
	
	/* Replace with your application code */
	while (1) 
	{
		if (escribir == 0)
		{
			PORTC &= ~(1<<PORTC5);
			
			SPI_Write('c');
			SPI_Write(0x00);
			
			valorSPI = SPI_READ(0);
			
			ADC_let = (valorSPI * 255UL) / 255UL;			// Se convierte a valor de 0 a 5.
			dig1 = ADC_let/100;								// Se obtienen unidades y decimales
			dig2 = (ADC_let / 10) % 10;
			dig3 = (ADC_let % 10);
			
			write_str("\n S1: ");							// Se carga a la terminal cada uno de los ADC
			write_char(1, dig1 + '0');
			write_char(1, dig2 + '0');
			write_char(1, dig3 + '0');
			
			SPI_Write('a');
			SPI_Write(0x00);
			
			valorSPI = SPI_READ(0);
			
			ADC_let2 = (valorSPI * 255UL) / 255UL;			// Se convierte a valor de 0 a 5.
			dig1_2 = ADC_let2/100;								// Se obtienen unidades y decimales
			dig2_2 = (ADC_let2 / 10) % 10;
			dig3_2 = (ADC_let2 % 10);
			
			write_str(" ||	S2: ");							// Se carga a la terminal cada uno de los ADC
			write_char(1, dig1_2 + '0');
			write_char(1, dig2_2 + '0');
			write_char(1, dig3_2 + '0');
			
			PORTC |= (1<<PORTC5);
		}
		
		
		else if (escribir == 1)
		{
			if ((numero <= 255) & (numero >= 0))
			{
				PORTC &= ~(1<<PORTC5);
				
				SPI_Write(numero);
				
				write_str(" ||	S3: ");							// Se carga a la terminal cada uno de los ADC
				write_char(1, valorSPI);
				
				PORTC |= (1<<PORTC5);
				
				escribir = 0;
			}
		}
		
		if ((numero <= 255) & (numero >= 0))
		{
			refreshPORT(numero);
		}
		
		_delay_ms(100);
	}
}

// funciones
void init_master()
{
	DDRC |= (1<<DDC5);
	DDRD |= ((1<<DDD2) | (1<<DDD3) | (1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7));
	DDRB |= ((1<<DDB0) | (1<<DDB1));
	
	PORTC |= (1<<PORTC5);
	
	PORTB &= ~((1<<PORTB0) | (1<<PORTB1));
	PORTD &= ~((1<<PORTD2) | (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7));

}

void refreshPORT(uint8_t valor)
{
	if(valor & 0b10000000)
	{
		PORTB |= (1<<PORTB1);
	}
	else
	{
		PORTB &= ~(1<<PORTB1);
	}
	if(valor & 0b01000000)
	{
		PORTB |= (1<<PORTB0);
	}
	else
	{
		PORTB &= ~(1<<PORTB0);
	}
	if(valor & 0b00100000)
	{
		PORTD |= (1<<PORTD7);
	}
	else
	{
		PORTD &= ~(1<<PORTD7);
	}
	if(valor & 0b00010000)
	{
		PORTD |= (1<<PORTD6);
	}
	else
	{
		PORTD &= ~(1<<PORTD6);
	}
	if(valor & 0b00001000)
	{
		PORTD |= (1<<PORTD5);
	}
	else
	{
		PORTD &= ~(1<<PORTD5);
	}
	if(valor & 0b00000100)
	{
		PORTD |= (1<<PORTD4);
	}
	else
	{
		PORTD &= ~(1<<PORTD4);
	}
	if(valor & 0b00000010)
	{
		PORTD |= (1<<PORTD3);
	}
	else
	{
		PORTD &= ~(1<<PORTD3);
	}
	if(valor & 0b00000001)
	{
		PORTD |= (1<<PORTD2);
	}
	else
	{
		PORTD &= ~(1<<PORTD2);
	}
}

// Interrupt routines
ISR(USART_RX_vect)
{
	cli();
	
	char temp = UDR0;
	
	if (temp >= '0' && temp <= '9')
	{
		if (counter == 0)
		{
			numero = 0;
			numero = temp - '0';
			counter = 1;
		}
		else if (counter == 1)
		{
			numero = (numero * 10) + (temp - '0');
			counter = 2; 
		}
		else if (counter == 2)
		{
			numero = (numero * 10) + (temp - '0');
			escribir = 1;
			counter = 0;
		}
	}
	else
	{
		write_char(1, 'O');
	}
	
	sei();
}