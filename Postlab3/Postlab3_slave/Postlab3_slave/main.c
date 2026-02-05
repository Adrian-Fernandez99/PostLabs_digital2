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
uint8_t ADC_val2= 0;

uint8_t escuchar = 0;
uint8_t numero = 0;

void refreshPORT(uint8_t valor);
void init_slave();

//Esclavo
int main(void)
{
	cli();
	init_slave();
	SPINIT(SPI_SLAVE_SS, SPI_DT_ORD_MSB, SPI_CLK_IDLE_L, SPI_CLK_FEDGE);
	ADC_init();
	SPCR |= (1<<SPIE);
	sei();
	
	/* Replace with your application code */
	while (1)
	{
		
		ADC_val = ADC_read(0, 0);
		ADC_val2 = ADC_read(1, 0);
	
		refreshPORT(numero);
		
		_delay_ms(100);
	}
}

// funciones

void init_slave()
{
	DDRD |= ((1<<DDD2) | (1<<DDD3) | (1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7));
	DDRB |= ((1<<DDB0) | (1<<DDB1));
	
	PORTD &= ~((1<<PORTD2) | (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5) | (1<<PORTD6) | (1<<PORTD7));
	PORTB &= ~((1<<PORTB0) | (1<<PORTB1));
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

// interrupciones
ISR(SPI_STC_vect)
{
	uint8_t SPI_val = SPDR;
 	if (SPI_val == 'c')
 	{
	 	SPDR = ADC_val;
 	}
 	else if (SPI_val == 'a')
 	{
	 	SPDR = ADC_val2;
 	}
 	else
 	{
	 	numero = SPDR;
	 	SPDR = numero;
 	}
	
}