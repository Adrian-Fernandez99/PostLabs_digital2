/*
Universidad del Valle de Guatemala
IE2023 : Programación de Microcontroladores

postlab1.c

Created: 1/21/2026 10:59:47 PM
Author : Adrián Fernández

Descripción: Simulador de carreras. Se presionan botones
para avanzar a los concursantes y cuando llegan a la meta
indican quien ganó.
 */ 

// Librerias
#include <avr/io.h>
#include <avr/interrupt.h>

#include "Display/Display.h"

// Se definen variables

volatile uint8_t valor_timer0 = 200;	// Variable del timer0
volatile uint8_t display1 = 5;			// Variable del display1
volatile uint8_t conteo = 0;			// Variable del para iniciar el conteo regresivo
volatile uint8_t inicio = 0;			// Variable del para iniciar el juego
uint8_t tmr_ovr = 0;					// Variable para verificar el numero de overflows del timer
volatile uint8_t mark_1 = 0;			// Variable para el marcador del jugador uno
volatile uint8_t mark_2 = 0;			// Variable para el marcador del jugador dos

void setup();
uint8_t decaton(uint8_t marca, uint8_t pos);

int main(void)
{
	/* Replace with your application code */
	setup();
	
	while (1)
	{
		PORTD = display(1, display1);
		PORTC = decaton(mark_1, 0);
		
	}
}


void setup()
{
	cli();
	
	// Se configuran puertos
	DDRD |= 0xFF;		// Puerto D es salida
	PORTD |= 0x00;
	
	DDRC |= 0x0F;		// Puerto C es salida
	PORTC |= (1 << PORTC4);
	
	DDRB |= 0x0F;		// La mitad del puerto B es salida y la otra entrada
	PORTB |= ((1 << PORTB4) | (1 << PORTB5));
	
	// Se configuran las interrupciones
	PCMSK0 |= (1 << PORTB4) | (1 << PORTB5);
	PCICR |= (1 << PCIE0);
	
	CLKPR	= (1 << CLKPCE); // Habilitar cambio en el prescaler
	CLKPR	= (1 << CLKPS2); // Setea presc a 16 para 1Mhz
	
	TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 1024
	TIMSK0 = (1 << TOIE0);
	TCNT0 = valor_timer0;
	
	ADCSRA	|= (1 << ADSC);
	
	UCSR0B	=	0x00;										// Apaga serial


	sei();
}

uint8_t decaton(uint8_t marca, uint8_t pos)
{
	if (marca == 0)
	{
		pos = 0x00;
	}
	else if (marca == 1)
	{
		pos = 0x01;
	}
	else if (marca == 2)
	{
		pos = 0x03;
	}
	else if (marca == 3)
	{
		pos = 0x07;
	}
	else if (marca == 4)
	{
		pos = 0x0F;
	}
	return pos;
}

// Interrupt routines
// Interrupción de pinchange
ISR(PCINT0_vect)
{
	cli();
	// Si el pin está encendido en el pin 4 inicia el juego
	if (inicio == 0)
	{
		if (!(PINB & (1 << PORTB4)))
		{
			if (conteo == 0)
			{
				conteo = 1;
			}
		}
	}
	else if (inicio == 1)
	{
		if (!(PINB & (1 << PORTB5)))
		{
			PORTB |= (1 << PORTB3);
			mark_1 ++;
			if (mark_1 == 4)
			{
				inicio = 0;
			}
		}
		else if (!(PIND & (1 << PORTD4)))
		{
			PORTB |= (1 << PORTB2);
			mark_2 ++;
		}
	}

	
	sei();
}

ISR(TIMER0_OVF_vect)
{
	cli();
	if (tmr_ovr == 250)
	{
		if (conteo == 1)
		{
			if (display1 == 0)
			{
				inicio = 1;
			}
			else
			{
				display1 = display1 - 1;
			}
		}
		tmr_ovr = 0;
	}
	else
	{
		tmr_ovr ++;
	}
	
	TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 1024
	TIMSK0 = (1 << TOIE0);
	TCNT0 = valor_timer0;
	
	sei();
}