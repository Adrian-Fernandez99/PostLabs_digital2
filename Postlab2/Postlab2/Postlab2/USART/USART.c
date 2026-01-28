/*
 * CFile1.c
 *
 * Created: 1/28/2026 12:15:26 AM
 *  Author: adria
 */ 
#define F_CPU 16000000UL
#include "USART.h"
#include <avr/io.h>
#include <util/delay.h>


void UART_init()
{
	DDRD |= (1 << DDD1);	// Configuración de pines rx y tx
	DDRD &= ~(1 << DDD0);
	UCSR0A = 0;				// Configuración del serial
	UCSR0B |= (1 << RXCIE0) | (1<< RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	UBRR0 = 103;	// BAUD RATE a 9600
}

void write_char(uint8_t direct, char caracter)
{
	while ((UCSR0A & (1 << UDRE0)) == 0);
	if (direct == 0)
	{
		PORTB = caracter;
	}
	else if (direct == 1)
	{
		UDR0 = caracter;
	}
}

void write_str(char* texto)
{
	for(uint8_t i = 0; *(texto+i) != '\0'; i++)
	{
		write_char(1, *(texto+i));
	}
}