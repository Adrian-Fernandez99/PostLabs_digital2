/*
 * ULTRA.c
 *
 * Created: 2/10/2026 7:36:48 PM
 *  Author: Britany Barillas
 */ 
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include "ULTRA.h"

static volatile uint16_t duration_ticks = 0;
static volatile uint8_t distance_cm = 0;

void ULTRA_init(void)
{
	// PORTD3 salida
	DDRD |= (1 << DDD3);

	// PORTD4 entrada
	DDRD &= ~(1 << DDD4);

	PORTD &= ~(1 << PORTD3);

	// Timer1 en modo normal
	TCCR2A = 0;
	TCCR2B = 0;
}

void ULTRA_Measure(void)
{
	uint16_t overflow_count = 0;

	PORTD |= (1 << PORTD3);
	_delay_us(10);
	PORTD &= ~(1 << PORTD3);

	while (!(PIND & (1 << PORTD4)));

	TCNT2 = 0;
	overflow_count = 0;

	TCCR2B = (1 << CS21);   // prescaler 8

	while (PIND & (1 << PORTD4))
	{
		if (TCNT2 == 255)
		{
			overflow_count++;
			TCNT2 = 0;
		}
	}

	TCCR2B = 0;

	duration_ticks = (overflow_count * 256UL) + TCNT2;

	distance_cm = duration_ticks / 116;
}

uint8_t ULTRA_GetDistance(void)
{
	return distance_cm;
}