/*
 * SPI.h
 *
 * Created: 1/29/2026 1:40:20 AM
 *  Author: adria
 */ 


#ifndef SPI_H_
#define SPI_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Se definen nuevas familias
typedef enum			// Fases del reloj
{
	SPI_CLK_FEDGE		= 0b00000000,
	SPI_CLK_LEDGE		= 0b00000100
}SPI_CLK_PHASE;

typedef enum			// División en el clk de comunicación
{
	SPI_MST_OSC_DIV2	= 0b01010000,
	SPI_MST_OSC_DIV4	= 0b01010001,
	SPI_MST_OSC_DIV8	= 0b01010010,
	SPI_MST_OSC_DIV16	= 0b01010011,
	SPI_MST_OSC_DIV32	= 0b01010100,
	SPI_MST_OSC_DIV64	= 0b01010101,
	SPI_MST_OSC_DIV128	= 0b01010110,
	SPI_SLAVE_SS		= 0b01000000
}SPI_TYPE;

typedef enum			// Orden de los datos
{
	SPI_DT_ORD_MSB		= 0b00000000,
	SPI_DT_ORD_LSB		= 0b00100000
}SPI_DATA_ORDER;

typedef enum			// Polaridad del reloj
{
	SPI_CLK_IDLE_H		= 0b00001000,
	SPI_CLK_IDLE_L		= 0b00000000
}SPI_CLK_POLARITY;

// Prototipos de funciones
void SPINIT(SPI_TYPE sType, SPI_DATA_ORDER sDataOrder, SPI_CLK_POLARITY sClkPol, SPI_CLK_PHASE sClkPhase);
void SPI_Write(uint8_t dat);
unsigned SPI_Data_Ready();
uint8_t SPI_READ(uint8_t tempo);

#endif /* SPI_H_ */