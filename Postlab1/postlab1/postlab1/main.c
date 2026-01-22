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


// Se definen variables
const uint8_t TABLA7SEG[16] =
{
	0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x77, 0x7F, 0x4E, 0x7E, 0x4F, 0x47
};
//	0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    A,    B,    C,    D,    E,    F

volatile uint8_t valor_timer0 = 200;	// Variable del timer0
volatile uint8_t display1 = 5;			// Variable del display1
volatile uint8_t inicio = 0;			// Variable del para iniciar el juego
uint8_t conteo = 0;			// Variable del para iniciar el juego

void setup();

int main(void)
{
	/* Replace with your application code */
	setup();
	
	while (1)
	{
		PORTD = TABLA7SEG[display1];

		
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

// Interrupt routines
// Interrupción de pinchange
ISR(PCINT0_vect)
{
	cli();
	// Si el pin está encendido en el pin 4 inicia el juego
	if (!(PINB & (1 << PORTB4)))
	{
		PORTB |= (1 << PORTB3);
		if (inicio == 0)
		{
			inicio = 1;
		}
	}
	sei();
}

ISR(TIMER0_OVF_vect)
{
	cli();
	if (conteo == 250)
	{
		if (inicio == 1)
		{
			if (display1 == 0)
			{
				display1 = 5;
				inicio = 0;
			}
			else
			{
				display1 = display1 - 1;
			}
		}
		conteo = 0;
	}
	else
	{
		conteo ++;
	}
	
	TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 1024
	TIMSK0 = (1 << TOIE0);
	TCNT0 = valor_timer0;
	
	sei();
}