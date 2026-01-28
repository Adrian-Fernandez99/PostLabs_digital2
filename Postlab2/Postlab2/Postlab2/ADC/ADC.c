/*
 * ADC.c
 *
 * Created: 1/22/2026 8:16:06 PM
 *  Author: adria
 */ 
#include "ADC.h"
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void ADC_init()
{
	ADMUX = 0;
	ADMUX	|= (1 << REFS0);
	ADMUX	|= (1 << ADLAR);
	
	ADCSRA	= 0;
	ADCSRA	|= (1 << ADPS1) | (1 << ADPS0);
	ADCSRA	|= (1 << ADEN);
	
	ADCSRA	|= (1 << ADSC);
}

uint16_t ADC_read(uint8_t PIN, uint16_t valorcito)
{
	ADMUX = (ADMUX & 0xF0) | (PIN & 0x0F);	// Selecciona pin de lectura
	ADCSRA |= (1 << ADSC);                   // Inicia conversión
	while (ADCSRA & (1 << ADSC));            // Espera a que termine
	valorcito = ADCH;                        // Devuelve valor (10 bits)
	return valorcito;
}