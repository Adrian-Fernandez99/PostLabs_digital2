/*
 * ADC.h
 *
 * Created: 1/22/2026 8:16:16 PM
 *  Author: adria
 */ 

#define F_CPU 16000000UL

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <util/delay.h>

void ADC_init();
uint16_t ADC_read(uint8_t PIN, uint16_t valorcito);

#endif /* ADC_H_ */