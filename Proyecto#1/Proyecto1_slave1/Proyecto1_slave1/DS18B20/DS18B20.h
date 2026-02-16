/*
 * DS18B20.h
 *
 * Created: 2/15/2026 11:02:31 PM
 *  Author: adria
 */ 


#ifndef DS18B20_H_
#define DS18B20_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t DS_Reset(void);
void DS_WriteBit(uint8_t bit);
uint8_t DS_ReadBit(void);
void DS_WriteByte(uint8_t data);
uint8_t DS_ReadByte(void);
void DS_init(void);
int16_t DS_readTemp(void);


#endif /* DS18B20_H_ */