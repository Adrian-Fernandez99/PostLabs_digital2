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
	TCCR1A = 0;
	TCCR1B = 0;
}

void ULTRA_Measure(void)
{
	// Enviar pulso PORTD3 de 10 µs
	PORTD |= (1 << PORTD3);
	_delay_us(10);
	PORTD &= ~(1 << PORTD3);

	// Esperar flanco alto
	uint32_t timeout = 0;
	while (!(PIND & (1 << PORTD4)))
	{
		if (++timeout > 60000)
		return;
	}

	// Reiniciar Timer1
	TCNT1 = 0;

	// Prescaler 8 ? 0.5 µs por tick
	TCCR1B = (1 << CS11);

	// Esperar flanco bajo
	timeout = 0;
	while (PIND & (1 << PORTD4))
	{
		if (++timeout > 60000)
		break;
	}

	// Detener timer
	TCCR1B = 0;

	duration_ticks = TCNT1;

	// Conversión correcta:
	// 1 cm ? 58 µs
	// 1 tick = 0.5 µs
	// 58 µs = 116 ticks
	distance_cm = duration_ticks / 116;
}

uint8_t ULTRA_GetDistance(void)
{
	return distance_cm;
}