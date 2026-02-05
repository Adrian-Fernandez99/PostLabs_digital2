/*
 * SPI.c
 *
 * Created: 1/29/2026 1:40:12 AM
 *  Author: adria
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "SPI.h"

void SPINIT(SPI_TYPE sType, SPI_DATA_ORDER sDataOrder, SPI_CLK_POLARITY sClkPol, SPI_CLK_PHASE sClkPhase)
{
	// PB2 -> SS
	// PB3 -> MOSI
	// PB4 -> MISO
	// PB5 -> SCK
	
	if(sType & (1<<MSTR))		// Si esta en modo maestro
	{
		DDRB |= (1<<DDB2) | (1<<DDB3) | (1<<DDB5);	// MOSI, SCK, SS en negado
		DDRB &= ~(1<<DDB4);			// MISO
		SPCR |= (1<<MSTR);			// Se vuelve maestro
		
		uint8_t temp = sType & 0b00000111;
		switch(temp){	// Configuración de maestro
			case 0:		// Cuando es DIV2
				SPCR &= ~((1<<SPR0) | (1<<SPR1));
				SPSR |= (1<<SPI2X);
			break;
			case 1:		// Cuando es DIV4
				SPCR &= ~((1<<SPR0) | (1<<SPR1));
				SPSR &= ~(1<<SPI2X);
			break;
			case 2:		// Cuando es DIV8
				SPCR |= (1<<SPR0);
				SPCR &= ~(1<<SPR1);
				SPSR |= (1<<SPI2X);
			break;
			case 3:		// Cuando es DIV16
				SPCR |= (1<<SPR0);
				SPCR &= ~(1<<SPR1);
				SPSR &= ~(1<<SPI2X);
			break;
			case 4:		// Cuando es DIV32
				SPCR &= ~(1<<SPR0);
				SPCR |= (1<<SPR1);
				SPSR |= (1<<SPI2X);
			break;
			case 5:		// Cuando es DIV64
				SPCR &= ~(1<<SPR0);
				SPCR |= (1<<SPR1);
				SPSR &= ~(1<<SPI2X);
			break;
			case 6:		// Cuando es DIV128
			SPCR |= ((1<<SPR0) | (1<<SPR1));
			SPSR &= ~(1<<SPI2X);
			break;
		}
	}
	
	else						// Si es esclavo
	{
		DDRB |= (1<<DDB4);			// MISO
		DDRB &= ~((1<<DDB2) | (1<<DDB3) | (1<<DDB5));		// MOSI, SCK, SS
		SPCR &= ~(1<<MSTR);			// Se vuelve esclavo
	}
	
	// Se habilida el SPI, se da orden a la data, polairdad y fase del reloj
	SPCR |= ((1<<SPE) | sDataOrder | sClkPol | sClkPhase);
	
}

// static void SPI_REC_WAIT()
// {
// 	while (!(SPSR & (1<<SPIF)));	// Espera a que los datos se reciban
// }

void SPI_Write(uint8_t dat)			// Escribe información en el bus SPI
{
	SPDR = dat;
}

unsigned SPI_Data_Ready()			// Revisa que los datos estan listos para ser leidos
{
	if(SPSR & (1<<SPIF))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t SPI_READ(uint8_t tempo)				// Lee los datos recibidos
{
	while (!(SPSR & (1<<SPIF)));	// Espera que los datos se terminen de recibir
	tempo = SPDR;
	return(tempo);					// Regresa los datos obtenidos
}

